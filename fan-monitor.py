import serial
import time
import requests

SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 115200
API_URL = "http://localhost:11987/status"

CPU_UID = "2b97e5f4256c55b70a6a5ff700ca21a066bffe03cd237c2c802f3db93694cf03"
GPU_UID = "7442ece48a26c31e110bbb1d21577e4a7b4613be5cce8adee1fe2d0832d11696"
FAN_UID = "8b76273cb2328ccea47d925147ddb6ab7acacd5ebee2f4fd1e814716d9470129"

def get_all_status():
    r = requests.get(API_URL, timeout=2)
    return {d["uid"]: d["status_history"][-1] for d in r.json()["devices"]}

def find_channel(channels, name):
    for ch in channels:
        if ch["name"] == name:
            return ch
    return None

def find_temp(temps, name):
    for t in temps:
        if t["name"] == name:
            return t
    return None

def main():
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    time.sleep(2)
    print("开始发送...")

    while True:
        try:
            status = get_all_status()

            # CPU 温度(temp1=Package) 和负载
            cpu = status[CPU_UID]
            cpu_temp = int(find_temp(cpu["temps"], "temp1")["temp"])
            cpu_load = int(find_channel(cpu["channels"], "CPU Load")["duty"])

            # GPU 温度(temp1=edge) 和负载
            gpu = status[GPU_UID]
            gpu_temp = int(find_temp(gpu["temps"], "temp1")["temp"])
            gpu_load = int(find_channel(gpu["channels"], "GPU Load")["duty"])

            # 风扇转速
            fan = status[FAN_UID]
            cpu_rpm  = find_channel(fan["channels"], "fan2")["rpm"]  # CPU-FAN
            case_rpm = find_channel(fan["channels"], "fan1")["rpm"]  # CASE-FAN
            #gpu_rpm  = find_channel(fan["channels"], "fan2")["rpm"]  # GPU-FAN ← 这里需要确认是哪个fan编号
            side_rpm = find_channel(fan["channels"], "fan7")["rpm"]  # GRAPHDOWN-FAN

            gpu_fan  = status[GPU_UID]
            gpu_rpm  = find_channel(gpu_fan["channels"], "fan1")["rpm"]  # amdgpu Fan1
            data = f"$CPU:{cpu_temp},{cpu_load},GPU:{gpu_temp},{gpu_load},FAN:{cpu_rpm},{case_rpm},{gpu_rpm},{side_rpm}\n"
            ser.write(data.encode())

            response = ser.readline().decode().strip()
            # print(f"发送: {data.strip()}")
            # print(f"回应: {response}")

        except Exception as e:
            print(f"错误: {e}")

        time.sleep(1)

if __name__ == "__main__":
    main()
