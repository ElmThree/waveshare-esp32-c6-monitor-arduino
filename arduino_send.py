import serial
import time

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
time.sleep(2)  # 等 ESP32 启动

print("开始发送...")

while True:
    data = "$CPU:58,42,GPU:72,68,FAN:1200,1100,1450,1820\n"
    ser.write(data.encode())
    print(f"已发送: {data.strip()}")

    # 读取 ESP32 回传的确认
    response = ser.readline().decode().strip()
    if response:
        print(f"ESP32回应: {response}")

    time.sleep(1)
