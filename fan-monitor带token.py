"""把 CoolerControl 的状态推给 ESP32-C6 副屏。

本机相关的东西（token、设备 UID、串口）都能用环境变量覆盖，换机器不必改代码：

    export CC_TOKEN=cc_xxxxxxxx
    export CC_CPU_UID=...   CC_GPU_UID=...   CC_FAN_UID=...
    export CC_SERIAL=/dev/ttyACM0          # 不设则自动扫描
    python fan-monitor带token.py
"""

import atexit
import os
import signal
import time

import requests
import serial
import serial.tools.list_ports

BAUD_RATE = 115200

API_URL = os.environ.get("CC_API_URL", "http://localhost:11987/status")
API_TOKEN = os.environ.get(
    "CC_TOKEN", "cc_5b32bd2446144a1a8f123319bfdc3ae8")

CPU_UID = os.environ.get(
    "CC_CPU_UID", "2b97e5f4256c55b70a6a5ff700ca21a066bffe03cd237c2c802f3db93694cf03")
GPU_UID = os.environ.get(
    "CC_GPU_UID", "7442ece48a26c31e110bbb1d21577e4a7b4613be5cce8adee1fe2d0832d11696")
FAN_UID = os.environ.get(
    "CC_FAN_UID", "8b76273cb2328ccea47d925147ddb6ab7acacd5ebee2f4fd1e814716d9470129")

# 通道名与温度探针名，换硬件时对一下
CPU_TEMP = "temp1"
CPU_LOAD = "CPU Load"
GPU_TEMP = "temp1"
GPU_LOAD = "GPU Load"
FAN_CPU = "fan2"      # CPU-FAN
FAN_CASE = "fan1"     # CASE-FAN
FAN_GPU = "fan1"      # 取自 GPU 设备自己的 channels（amdgpu）
FAN_SIDE = "fan7"     # GRAPHDOWN-FAN

DEFAULT = "--"


def pick_value(items, name, key):
    """在 channels / temps 列表里按名字取值，取不到返回 DEFAULT。"""
    for item in items or []:
        if item.get("name") == name:
            value = item.get(key)
            return DEFAULT if value is None else value
    return DEFAULT


def as_int(value):
    """转成整数，失败就原样返回占位符（固件那边显示成文字，进度条按 0 处理）。"""
    try:
        return int(round(float(value)))
    except (TypeError, ValueError):
        return DEFAULT


def find_esp_port():
    """扫描所有串口，找到 ESP32 的 CDC 口。"""
    for p in serial.tools.list_ports.comports():
        if any(k in (p.description or "") for k in
               ("USB", "UART", "CH340", "CP210", "CDC")):
            print(f"找到设备: {p.device} - {p.description}")
            return p.device
    return None


def connect():
    """尝试连接，失败就等待重试，方便热插拔。"""
    fixed = os.environ.get("CC_SERIAL")
    while True:
        port = fixed or find_esp_port()
        if port:
            try:
                ser = serial.Serial(port, BAUD_RATE, timeout=1)
                time.sleep(2)          # 等 ESP32 启动/复位完成
                print(f"已连接: {port}")
                return ser
            except Exception as e:
                print(f"连接失败: {e}")
        else:
            print("未找到设备，等待重试...")
        time.sleep(2)


def get_all_status():
    headers = {"Authorization": f"Bearer {API_TOKEN}"}
    r = requests.get(API_URL, headers=headers, timeout=2)
    r.raise_for_status()
    return {d["uid"]: (d.get("status_history") or [{}])[-1]
            for d in r.json()["devices"]}


def build_payload(status):
    cpu, gpu, fan = status.get(CPU_UID), status.get(GPU_UID), status.get(FAN_UID)
    missing = [n for n, d in (("CPU", cpu), ("GPU", gpu), ("FAN", fan)) if not d]
    if missing:
        raise RuntimeError(
            f"设备 UID 对不上：缺 {', '.join(missing)}。用 README 里的一行命令重新取 UID")

    fields = [
        as_int(pick_value(cpu.get("temps"), CPU_TEMP, "temp")),
        as_int(pick_value(cpu.get("channels"), CPU_LOAD, "duty")),
        as_int(pick_value(gpu.get("temps"), GPU_TEMP, "temp")),
        as_int(pick_value(gpu.get("channels"), GPU_LOAD, "duty")),
        as_int(pick_value(fan.get("channels"), FAN_CPU, "rpm")),
        as_int(pick_value(fan.get("channels"), FAN_CASE, "rpm")),
        as_int(pick_value(gpu.get("channels"), FAN_GPU, "rpm")),
        as_int(pick_value(fan.get("channels"), FAN_SIDE, "rpm")),
    ]
    return "$CPU:{},{},GPU:{},{},FAN:{},{},{},{}\n".format(*fields)


def shutdown(ser):
    """退出时让副屏关屏。固件支持 $OFF 后会立刻熄灭；旧固件会自行超时关屏。"""
    try:
        ser.write(b"$OFF\n")
        ser.flush()
        time.sleep(0.1)
        ser.close()
        print("已发送关屏指令")
    except Exception:
        pass


def main():
    ser = connect()
    atexit.register(lambda: shutdown(ser))
    for sig in (signal.SIGTERM, signal.SIGINT):
        signal.signal(sig, lambda s, f: exit(0))
    print("开始发送...")

    while True:
        try:
            ser.write(build_payload(get_all_status()).encode())
            time.sleep(0.1)
            while ser.in_waiting:
                line = ser.readline().decode(errors="replace").strip()
                if line:
                    print(f"ESP32: {line}")

        except serial.SerialException as e:
            print(f"串口断开: {e}")
            try:
                ser.close()
            except Exception:
                pass
            ser = connect()

        except Exception as e:
            print(f"错误: {e}")

        time.sleep(1)


if __name__ == "__main__":
    main()
