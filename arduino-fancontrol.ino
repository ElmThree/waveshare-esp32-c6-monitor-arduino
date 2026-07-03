#include "Display_ST7789.h"
#include "LVGL_Driver.h"
#include "ui.h"
#include <string>
#include "vars.h"

//sd card
#include "SD_Card.h"
#include <TJpgDec.h>  // ESP32 最常用的硬件/软件混合优化 JPEG 解码库
File mjpegFile;
#define MJPEG_BUFFER_SIZE 20480 // 20KB 缓冲区，足够装下 172x320 的单帧 JPEG
uint8_t mjpeg_buf[MJPEG_BUFFER_SIZE];
//


std::string cpu_temp;
std::string cpu_load;
std::string gpu_temp;
std::string gpu_load;

std::string cpu_rpm;
std::string case_rpm;
std::string gpu_rpm;
std::string side_rpm;
float cpu_load_bar;
float gpu_load_bar;


uint32_t last_data_time = 0;
bool is_connected = true;
String inputBuffer = "";

//chip temp
#ifdef __cplusplus
extern "C" {
#endif
#include "driver/temperature_sensor.h"
#ifdef __cplusplus
}
#endif
//

temperature_sensor_handle_t temp_sensor = NULL;
std::string chip_temp;

extern "C" const char *get_var_cpu_temp() {
    return cpu_temp.c_str();
}
extern "C" void set_var_cpu_temp(const char *value) {
    cpu_temp = value;
}

extern "C" const char *get_var_cpu_load() {
    return cpu_load.c_str();
}
extern "C" void set_var_cpu_load(const char *value) {
    cpu_load = value;
}

extern "C" const char *get_var_gpu_temp() {
    return gpu_temp.c_str();
}
extern "C" void set_var_gpu_temp(const char *value) {
    gpu_temp = value;
}

extern "C" const char *get_var_gpu_load() {
    return gpu_load.c_str();
}
extern "C" void set_var_gpu_load(const char *value) {
    gpu_load = value;
}

extern "C" const char *get_var_cpu_rpm() {
    return cpu_rpm.c_str();
}
extern "C" void set_var_cpu_rpm(const char *value) {
    cpu_rpm = value;
}

extern "C" const char *get_var_case_rpm() {
    return case_rpm.c_str();
}
extern "C" void set_var_case_rpm(const char *value) {
    case_rpm = value;
}

extern "C" const char *get_var_gpu_rpm() {
    return gpu_rpm.c_str();
}
extern "C" void set_var_gpu_rpm(const char *value) {
    gpu_rpm = value;
}

extern "C" const char *get_var_side_rpm() {
    return side_rpm.c_str();
}

extern "C" void set_var_side_rpm(const char *value) {
    side_rpm = value;
}

extern "C" float get_var_cpu_load_bar() {
    return cpu_load_bar;
}

extern "C" void set_var_cpu_load_bar(float value) {
    cpu_load_bar = value;
}

extern "C" float get_var_gpu_load_bar() {
    return gpu_load_bar;
}
extern "C" void set_var_gpu_load_bar(float value) {
    gpu_load_bar = value;
}

extern "C" const char *get_var_chip_temp() {
    return chip_temp.c_str();
}
extern "C" void set_var_chip_temp(const char *value) {
    chip_temp = value;
}

extern void Set_Backlight(uint8_t Light);

//sd card
File mjpegFile;
#define MJPEG_BUFFER_SIZE 20480 // 20KB 缓冲区，足够装下 172x320 的单帧 JPEG
uint8_t mjpeg_buf[MJPEG_BUFFER_SIZE];

// LVGL 的显示回调，或者是你的底层屏幕渲染函数
// TJpgDec 解码出像素后，会自动调用这个回调函数把它刷到屏幕上
bool tjpgd_output_cb(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
    // 这里的 LCD_addWindow 就是你之前驱动里的那个函数
    // 如果屏幕旋转了，注意横竖屏的坐标对齐
    LCD_addWindow(x, y, x + w - 1, y + h - 1, bitmap);
    return true;
}

void Video_Init() {
    // 1. 设置解码器的输出回调
    TJpgDec.setCallback(tjpgd_output_cb);
    // 2. 根据你的副屏微调缩放比例（1代表原图大小不缩放）
    TJpgDec.setJpgScale(1); 
    
    // 3. 打开卡里的视频文件
    mjpegFile = SD.open("/background.mjpeg");
    if (!mjpegFile) {
        printf("Failed to open background.mjpeg!\r\n");
    }
}

void Video_Play_Loop() {
    if (!mjpegFile || !mjpegFile.available()) {
        // 如果文件播完了，立刻回到开头，实现机箱动态背景无限循环
        if (mjpegFile) mjpegFile.seek(0);
        return;
    }

    // --- 核心：寻找 MJPEG 的一帧 (寻找 JPEG 的帧头 0xFFD8) ---
    int32_t frame_start = -1;
    while (mjpegFile.available()) {
        if (mjpegFile.read() == 0xFF) {
            if (mjpegFile.peek() == 0xD8) {
                frame_start = mjpegFile.position() - 1;
                break;
            }
        }
    }

    if (frame_start < 0) return; // 没找到帧头

    // --- 核心：寻找这一帧的结尾 (寻找 JPEG 的帧尾 0xFFD9) ---
    int32_t frame_end = -1;
    while (mjpegFile.available()) {
        if (mjpegFile.read() == 0xFF) {
            if (mjpegFile.peek() == 0xD9) {
                mjpegFile.read(); // 把 D9 读掉
                frame_end = mjpegFile.position();
                break;
            }
        }
    }

    if (frame_end > frame_start) {
        uint32_t frame_size = frame_end - frame_start;
        
        // 防止异常大帧撑爆缓冲区
        if (frame_size < MJPEG_BUFFER_SIZE) {
            // 回到这一帧的开头，把整帧数据读入 RAM 缓冲区
            mjpegFile.seek(frame_start);
            mjpegFile.read(mjpeg_buf, frame_size);
            
            // 调用 TJpgDec 解码这一帧，它会自动触发 tjpgd_output_cb 投射到侧板屏幕上
            TJpgDec.drawJpg(0, 0, mjpeg_buf, frame_size);
        }
    }

    // 控制帧率：如果是 24 帧的视频，大概每帧间隔 40ms
    // 因为解码本身需要几毫秒，这里可以给个 30ms-35ms 的延时，或者用 millis() 动态控制
    delay(35); 
}
//sd card end

void setup()
{       
  LCD_Init();
  Lvgl_Init();
  ui_init();
  SD_Init();
  Serial.begin(115200);

  temperature_sensor_config_t temp_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(20, 100);
  temperature_sensor_install(&temp_config, &temp_sensor);
  temperature_sensor_enable(temp_sensor);

// 初始状态：灰色（未连接）
  //lv_obj_set_style_bg_opa(objects.status_point, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(objects.status_point, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
  last_data_time = millis();

}


void loop()
{
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      inputBuffer.trim();
      if (inputBuffer.startsWith("$")) {
        String line = inputBuffer.substring(1);
        line.replace("CPU:", "");
        line.replace("GPU:", "");
        line.replace("FAN:", "");

        int idx = 0;
        String tokens[8];
        while (line.length() > 0 && idx < 8) {
          int comma = line.indexOf(',');
          if (comma == -1) {
            tokens[idx++] = line;
            break;
          }
          tokens[idx++] = line.substring(0, comma);
          line = line.substring(comma + 1);
        }

        if (idx == 8) {  // 确保收到完整的8个字段再更新
          Set_Backlight(40); //打开背光 
          is_connected = true; 
          lv_obj_clear_flag(objects.backgroumd_image, LV_OBJ_FLAG_HIDDEN);
          set_var_cpu_temp(tokens[0].c_str());
          set_var_cpu_load(tokens[1].c_str());
          set_var_gpu_temp(tokens[2].c_str());
          set_var_gpu_load(tokens[3].c_str());
          set_var_cpu_rpm(tokens[4].c_str());
          set_var_case_rpm(tokens[5].c_str());
          set_var_gpu_rpm(tokens[6].c_str());
          set_var_side_rpm(tokens[7].c_str());
          set_var_cpu_load_bar(tokens[1].toFloat());
          set_var_gpu_load_bar(tokens[3].toFloat());
          last_data_time = millis();
          
          lv_obj_set_style_bg_color(objects.status_point, lv_color_hex(0x5DCAA5), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
      }
      inputBuffer = "";  // 处理完清空
    } else {
      inputBuffer += c;
      if (inputBuffer.length() > 200) {
        inputBuffer = "";  // 防止缓冲区无限增长
      }
    }
  }

  if (millis() - last_data_time > 3000 && is_connected) {
    is_connected = false;
        // 1. 关闭背景动画（最省电）
    lv_obj_add_flag(objects.backgroumd_image, LV_OBJ_FLAG_HIDDEN);

    lv_obj_set_style_bg_color(objects.status_point, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    set_var_cpu_temp("no");
    set_var_cpu_load("--");
    set_var_gpu_temp("sig");
    set_var_gpu_load("--");
    set_var_cpu_rpm("pls");
    set_var_case_rpm("chk");
    set_var_gpu_rpm("your");
    set_var_side_rpm("conn");
  }
  if (millis() - last_data_time > 10000 && !is_connected) {
    Set_Backlight(0); // 关背光
    lv_obj_set_style_bg_color(objects.status_point, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
  }

  static uint32_t last_chip_temp_read = 0;
  if (millis() - last_chip_temp_read > 1000) {
      float t = 0;
      temperature_sensor_get_celsius(temp_sensor, &t);
      set_var_chip_temp(String((int)t).c_str());
      last_chip_temp_read = millis();
  }

  Timer_Loop();
  ui_tick();
  delay(5);
}