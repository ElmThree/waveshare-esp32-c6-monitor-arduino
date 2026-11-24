# ESP32-C6 PC Monitor — CPU/GPU 温度与风扇转速副屏

一块基于微雪 ESP32-C6 的桌面小屏：PC 端脚本从 [CoolerControl](https://gitlab.com/coolercontrol/coolercontrol) 读出 CPU / GPU 温度、负载和四路风扇转速，通过 USB 串口推给 ESP32，ESP32 用 LVGL 画成一块仪表盘。插上 SD 卡时还可以播放 MJPEG 动画当背景。

无网络、无配对，纯串口单向数据 + 心跳检测。

---

## 1. 硬件

微雪 ESP32-C6 开发板，板载 1.47" 172×320 ST7789 SPI 屏和 microSD 插槽。引脚定义见 `Display_ST7789.h` / `SD_Card.h`：

| 功能 | GPIO | 说明 |
|---|---|---|
| LCD SCLK | 7 | SPI 时钟 |
| LCD MOSI | 6 | SPI 数据（屏与 SD 共用） |
| LCD/SPI MISO | 5 | SD 回读 |
| LCD CS | 14 | |
| LCD DC | 15 | |
| LCD RST | 21 | |
| LCD BL | 22 | PWM 背光（1 kHz / 10 bit） |
| SD CS | 4 | 与屏共用 SPI 总线 |

- 屏：320×172 横屏，`Offset_Y = 34`（面板物理是 172×320 竖屏，靠偏移裁成横屏）。
- 屏 SPI 跑 80 MHz，SD 跑 40 MHz。
- 板子没有 PSRAM，所有缓冲都吃片上 SRAM，所以背景帧缓冲（110 KB）是静态分配的、UI 内存只给了 48 KB。

---

## 2. 数据流

```
CoolerControl daemon (HTTP :11987)
        │  /status  (Bearer token)
        ▼
fan-monitor带token.py  ── 每秒一条 ASCII 行 ──▶  /dev/ttyACM0  115200
                                                    │
                                                    ▼
                                        ESP32-C6 固件（本目录）
                                         ├─ 解析 8 个字段
                                         ├─ 写入 LVGL 的 EEZ 变量
                                         └─ ui_tick() 刷新标签 / 进度条
```

ESP32 只做显示，不参与风扇控制。风扇曲线仍然归 CoolerControl 管。

---

## 3. 目录结构

```
.
├── waveshare-esp32-c6-monitor-arduino.ino   # 主固件：初始化、串口协议、MJPEG 播放、心跳/超时
├── Display_ST7789.cpp/.h      # ST7789 SPI 驱动 + PWM 背光
├── LVGL_Driver.cpp/.h         # LVGL 显示/时基对接，Timer_Loop()
├── SD_Card.cpp/.h             # SD 挂载与文件查找
├── screens.c/.h               # EEZ Studio 导出的界面（实际编译的就是这份）
├── styles.c/.h  images.c/.h   # 同上：样式与图像描述表
├── vars.h                     # EEZ 全局变量 get/set 声明
├── ui.c/.h                    # ui_init() / ui_tick()
├── ui_font_robot_mono_64.c    # 大号数字字体
├── ui_image__4.c / __5.c      # 无 SD 时的两帧循环背景（320×172, TRUE_COLOR_ALPHA）
├── font/                      # DSEG7 TTF 源字体
├── fan-monitor带token.py      # ★ 实际在用的主机端脚本（带 CoolerControl token）
├── fan-monitor.py             # 早期版本，无 token
├── arduino_send.py            # 假数据测试脚本，不连 CoolerControl
├── arduino-fancontrol/        # EEZ Studio 工程目录（.eez-project + 自己的导出）
└── UI/                        # 更早的一份 SquareLine 风格导出，已废弃
```

要点：**Arduino 只编译根目录下的 `.ino/.c/.cpp`**。`arduino-fancontrol/` 和 `UI/` 是设计工具工程，不参与编译，两边的导出已经和根目录的代码不同步——要改界面请回 EEZ Studio 改工程再导出覆盖根目录，别直接改子目录。

---

## 4. 串口协议

固定 **115200 8N1**，每条消息一行，以 `\n` 结尾。

```
$CPU:<cpu_temp>,<cpu_load>,GPU:<gpu_temp>,<gpu_load>,FAN:<cpu_rpm>,<case_rpm>,<gpu_rpm>,<side_rpm>\n
```

例：

```
$CPU:58,42,GPU:72,68,FAN:1200,1100,1450,1820
```

| 序号 | 字段 | 来源（CoolerControl） |
|---|---|---|
| 0 | CPU 温度 | CPU 设备 `temps` 中 `temp1`（Package） |
| 1 | CPU 负载 % | CPU 设备 `channels` 中 `CPU Load` 的 duty |
| 2 | GPU 温度 | GPU 设备 `temps` 中 `temp1`（edge） |
| 3 | GPU 负载 % | GPU 设备 `channels` 中 `GPU Load` 的 duty |
| 4 | CPU 风扇 RPM | 风扇设备 `fan2`（CPU-FAN） |
| 5 | 机箱风扇 RPM | 风扇设备 `fan1`（CASE-FAN） |
| 6 | GPU 风扇 RPM | GPU 设备 `fan1`（amdgpu） |
| 7 | 侧板风扇 RPM | 风扇设备 `fan7`（GRAPHDOWN-FAN） |

固件侧的规矩：

- `CPU:` `GPU:` `FAN:` 前缀会被剥掉，然后按逗号切分，**必须正好 8 段**才更新界面，否则整行丢弃。
- 每收到一条有效数据：背光升到 40%（`ledcWrite` 10 bit 下写 400，实测约 39%），状态灯转绿（`0x5DCAA5`）。
- 收到 `$OFF`（主机端退出时发的）立即按超时处理：文字变占位、背光关闭，主机重连后自动恢复。
- 3 秒没数据：状态灯转红，各字段变成占位文字，拼起来是 `no sig pls chk your conn`（"no signal, please check your connection"，被人为拆到 8 个字段里）。
- 再等 7 秒（累计 10 秒无数据）：关背光。
- 芯片自身温度每秒读一次（`temperature_sensor`，20–100 ℃ 量程），显示在右上角小字。

---

## 5. 屏幕布局

```
┌───────────────────────┬───────────────────────┐  ┌ 芯片温度  ● 状态灯
│  CPU      58°         │  GPU      72°         │  │ (右上角小字 5×5 圆点)
│  ▓▓▓▓░░░░  42% load   │  ▓▓▓▓▓░░░  68% load   │  │
├───────────┬───────────┼───────────┬───────────┤
│ CPU FAN   │ CASE FAN  │ GPU FAN   │ SIDE FAN  │
│  1200 RPM │  1100 RPM │  1450 RPM │  1820 RPM │
└───────────┴───────────┴───────────┴───────────┘
```

- CPU 面板 (0,0) 160×110，GPU 面板 (160,0)。
- 四个风扇块各 80×62，y=110。
- 背景层：`video_canvas`（SD 卡 MJPEG）与 `backgroumd_image`（两帧 20 秒循环动画）互斥显示，有 SD 就走 canvas。

---

## 6. 编译与烧录

### 依赖

| 组件 | 本机版本 | 备注 |
|---|---|---|
| ESP32 Arduino core | `4.0.0-alpha1-cn` | 建议用官方稳定 3.x 复现，alpha 有编译行为差异 |
| LVGL | 8.3.10 | EEZ 工程标称 8.4.0 |
| JPEGDEC | 1.8.4 | MJPEG 解码 |
| `lv_conf.h` | `~/Arduino/libraries/lvgl/src/lv_conf.h` | 关键项见下 |

`lv_conf.h` 里必须保证：

```c
#define LV_COLOR_DEPTH   16
#define LV_COLOR_16_SWAP 0
#define LV_MEM_SIZE      (48U * 1024U)
#define LV_USE_CANVAS    1
```

LVGL 的 `lv_conf_internal.h` 用 `__has_include("lv_conf.h")` 找配置，所以把 `lv_conf.h` 放在 `libraries/lvgl/src/` 下是有效的（本机现状）。放到库里就要改库目录，重新装库会丢——最好迁到 sketch 目录或 `LV_CONF_PATH`。

### 烧录

Arduino IDE 2：开发板选 **ESP32C6 Dev Module**，端口选 CDC 出来的 `/dev/ttyACM0`，直接上传。

注意目录名必须和主 `.ino` 同名（当前是 `waveshare-esp32-c6-monitor-arduino`），否则 Arduino IDE 会要求把 sketch 放进同名子目录里。

或命令行：

```bash
arduino-cli compile --fqbn esp32:esp32:esp32c6 .
arduino-cli upload  --fqbn esp32:esp32:esp32c6 -p /dev/ttyACM0 .
```

### SD 卡背景（可选）

把拼接好的 MJPEG 放到卡根目录，命名必须是 **`/background10fps.mjpeg`**。每帧 320×172，帧之间就是连续的 JPEG，靠 `FFD8`…`FFD9` 定位，单帧上限 32 KB，渲染上限约 20 fps（`Video_Play_Loop` 里 50 ms 节流）。

```bash
ffmpeg -i input.mp4 \
  -vf "scale=320:172,fps=10" \
  -c:v mjpeg -q:v 6 -pix_fmt yuvj420p \
  -f mjpeg background10fps.mjpeg
```

固件每 5 秒检查一次这个文件还在不在：卡被拔掉会自动回退到内置的两帧动画，插回来又能继续播。

---

## 7. 主机端

```bash
pip install pyserial requests
python fan-monitor带token.py
```

脚本行为：

- 自动扫描串口，挑描述里带 `USB / UART / CH340 / CP210 / CDC` 的那个，找不到就每 2 秒重试——所以插拔不用重启脚本。
- 每秒拉一次 `http://localhost:11987/status`，取 `devices[].status_history` 的最后一条。
- 串口异常时自动重连。

脚本的 token / 设备 UID / 通道名都写在文件顶部，但**每一项都能用环境变量覆盖**，换机器或换显卡不必改代码：

| 环境变量 | 对应 |
|---|---|
| `CC_TOKEN` | CoolerControl 访问令牌 |
| `CC_API_URL` | 默认 `http://localhost:11987/status` |
| `CC_CPU_UID` / `CC_GPU_UID` / `CC_FAN_UID` | 三个设备的 UID |
| `CC_SERIAL` | 指定串口；不设则自动扫描 |

设备 UID 换机器必须重新取：

```bash
curl -s -H "Authorization: Bearer $CC_TOKEN" http://localhost:11987/status \
  | python -c 'import json,sys
for d in json.load(sys.stdin)["devices"]:
    print(d["uid"], d.get("name", ""))'
```

Token 在 CoolerControl 的「设置 → 守护进程/Access token」里生成，填进 `CC_TOKEN`。旧版 CoolerControl 不需要鉴权，这就是 `fan-monitor.py` 存在的意义。

通道名取不到的字段会发 `--`（界面显示文字、进度条按 0 处理），不会因为某个风扇没接就整轮崩掉。

想开机自启，可以写个 user unit：

```ini
# ~/.config/systemd/user/fan-monitor.service
[Unit]
Description=ESP32 fan monitor bridge
After=network.target

[Service]
ExecStart=/usr/bin/python /home/elmthree/Desktop/waveshare-esp32-c6-monitor-arduino/fan-monitor带token.py
Restart=always
RestartSec=3

[Install]
WantedBy=default.target
```

---

## 8. 已知问题 / 待办

1. **三份 UI 导出不同步**：根目录（实际编译）、`arduino-fancontrol/`（EEZ 工程）、`UI/`（旧 SquareLine 导出）内容不一致，改界面很容易改错地方。
2. **LVGL 版本差**：工程按 8.4.0 导出，本机装的是 8.3.10。目前能编能跑，但升级时要留意 API 变化。
3. **上电背光偏暗**：`Backlight_Init()` 写的是 raw duty 100（10 bit 下约 10%），收到第一条数据才升到 40%，所以插上 USB 后会先暗一下。
4. **GPU 风扇数据源不一致**：另外三路来自风扇设备，GPU 那路取自 GPU 设备自身的 `channels`。换显卡后通道名（`fan1`）可能变，可在脚本顶部改。
5. **通道名写死在脚本里**：`temp1` / `fan1` / `fan7` 这类名字换了主板或风扇接头就得改，没有配置文件。
6. **单视图无切屏**：`ui_tick()` 只跑当前屏，多屏工程要自己扩。
