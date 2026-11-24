#include "Display_ST7789.h"
#include "LVGL_Driver.h"
#include "ui.h"
#include <string>
#include "vars.h"

//sd card
#include "SD_Card.h"
#include <JPEGDEC.h>  // ESP32 最常用的硬件/软件混合优化 JPEG 解码库
JPEGDEC jpeg;
File mjpegFile;
#define MJPEG_BUFFER_SIZE 32768 // 20KB 缓冲区，足够装下 172x320 的单帧 JPEG
uint8_t mjpeg_buf[MJPEG_BUFFER_SIZE];
//


// 1. 在文件顶部加前向声明
// bool tjpgd_canvas_cb(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);
// 改成
bool tjpgd_output_cb(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);
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
bool sd_available = true;
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

lv_obj_t* canvas_obj = NULL;
static lv_color_t canvas_buf[320 * 172];

// void Video_Init() {
//     // Canvas 初始化
//     canvas_obj = objects.video_canvas;
//     lv_canvas_set_buffer(canvas_obj, canvas_buf, 320, 172, LV_IMG_CF_TRUE_COLOR);
    
//     TJpgDec.setCallback(tjpgd_canvas_cb);
//     TJpgDec.setJpgScale(1);
//     TJpgDec.setSwapBytes(true);
    
//     mjpegFile = SD.open("/background.mjpeg");
//     if (!mjpegFile) {
//         printf("Failed to open background.mjpeg!\r\n");
//     }
// }


void Video_Init() {
    canvas_obj = objects.video_canvas;
    lv_canvas_set_buffer(canvas_obj, canvas_buf, 320, 172, LV_IMG_CF_TRUE_COLOR);
    
    mjpegFile = SD.open("/background10fps.mjpeg");
    if (!mjpegFile) {
        printf("SD卡不可用，使用动画背景\n");
        sd_available = false;
        lv_obj_add_flag(objects.video_canvas, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(objects.backgroumd_image, LV_OBJ_FLAG_HIDDEN);
    } else {
        printf("视频文件打开成功\n");
        sd_available = true;
        lv_obj_clear_flag(objects.video_canvas, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(objects.backgroumd_image, LV_OBJ_FLAG_HIDDEN);
    }
}

int jpegdec_cb(JPEGDRAW *pDraw) {
    uint16_t *src = pDraw->pPixels;
    lv_color_t *dst = canvas_buf + pDraw->y * 320 + pDraw->x;
    for (int row = 0; row < pDraw->iHeight; row++) {
        memcpy(dst, src, pDraw->iWidth * sizeof(uint16_t));
        dst += 320;
        src += pDraw->iWidth;
    }
    // 不在这里 invalidate
    return 1;
}

// 回调改成写 Canvas
// bool tjpgd_canvas_cb(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
//     for (int row = 0; row < h; row++) {
//         for (int col = 0; col < w; col++) {
//             uint16_t pixel = bitmap[row * w + col];
//             // 用 lv_color_make 正确构造颜色
//             uint8_t r = (pixel >> 11) & 0x1F;
//             uint8_t g = (pixel >> 5)  & 0x3F;
//             uint8_t b = (pixel)       & 0x1F;
//             canvas_buf[(y + row) * 320 + (x + col)] = lv_color_make(
//                 (r << 3) | (r >> 2),
//                 (g << 2) | (g >> 4),
//                 (b << 3) | (b >> 2)
//             );
//         }
//     }
//     lv_obj_invalidate(canvas_obj);
//     return true;
// }

// bool tjpgd_output_cb(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap) {
//     // 手动交换每个像素的字节序
//     for (int i = 0; i < w * h; i++) {
//         bitmap[i] = (bitmap[i] >> 8) | (bitmap[i] << 8);
//     }
//     LCD_addWindow(x, y, x + w - 1, y + h - 1, bitmap);
//     return true;
// }

// void Video_Play_Loop() {
//     static uint32_t last_frame = 0;
//     if (millis() - last_frame < 50) return;  // 100ms = 10fps
//     last_frame = millis();

//     //帧数统计
//     static uint32_t fps_count = 0;
//     static uint32_t fps_timer = 0;
//     fps_count++;
//     if (millis() - fps_timer >= 1000) {
//         printf("FPS: %d\n", fps_count);
//         fps_count = 0;
//         fps_timer = millis();
//     }
//     //

//     if (!mjpegFile || !mjpegFile.available()) {
//         if (mjpegFile) mjpegFile.seek(0);
//         return;
//     }

//     // 找帧头
//     while (mjpegFile.available() > 1) {
//         uint8_t b = mjpegFile.read();
//         if (b == 0xFF && mjpegFile.peek() == 0xD8) {
//             mjpegFile.seek(mjpegFile.position() - 1);
//             break;
//         }
//     }
//     uint32_t frame_start = mjpegFile.position();

//     // 找帧尾
//     while (mjpegFile.available() > 1) {
//         if (mjpegFile.read() == 0xFF && mjpegFile.peek() == 0xD9) {
//             mjpegFile.read();
//             break;
//         }
//     }
//     uint32_t frame_size = mjpegFile.position() - frame_start;

//     if (frame_size > 0 && frame_size < MJPEG_BUFFER_SIZE) {
//         mjpegFile.seek(frame_start);
//         mjpegFile.read(mjpeg_buf, frame_size);

//         if (jpeg.openRAM(mjpeg_buf, frame_size, jpegdec_cb)) {

//           jpeg.decode(0, 0, 0);
//           jpeg.close();
//           lv_obj_invalidate(canvas_obj);  // 整帧完成后才通知 LVGL
//         }
//         //debug
//         // static uint32_t dbg = 0;
//         // if (dbg++ % 50 == 0) {
//         //     printf("📊 解码帧: %d 字节\r\n", frame_size);
//         // }
//     }
// }

void Video_Play_Loop() {
    static uint32_t last_frame = 0;
    if (millis() - last_frame < 50) return;//1/50=0.02s
    last_frame = millis();

    // //debug fps计数器
    // static uint32_t fps_count = 0;
    // static uint32_t fps_timer = 0;
    // fps_count++;
    // if (millis() - fps_timer >= 1000) {
    //     printf("FPS: %d\n", fps_count);
    //     fps_count = 0;
    //     fps_timer = millis();
    // }
    // //

    if (!mjpegFile || !mjpegFile.available()) {
        if (mjpegFile) mjpegFile.seek(0);
        return;
    }

    // 找帧头
    while (mjpegFile.available() > 1) {
        uint8_t b = mjpegFile.read();
        if (b == 0xFF && mjpegFile.peek() == 0xD8) {
            mjpegFile.seek(mjpegFile.position() - 1);
            break;
        }
    }
    uint32_t frame_start = mjpegFile.position();

    // 找帧尾
    while (mjpegFile.available() > 1) {
        if (mjpegFile.read() == 0xFF && mjpegFile.peek() == 0xD9) {
            mjpegFile.read();
            break;
        }
    }
    uint32_t frame_size = mjpegFile.position() - frame_start;

    if (frame_size > 0 && frame_size < MJPEG_BUFFER_SIZE) {
        mjpegFile.seek(frame_start);
        
        uint32_t t1 = millis();
        mjpegFile.read(mjpeg_buf, frame_size);
        uint32_t t2 = millis();
        
        if (jpeg.openRAM(mjpeg_buf, frame_size, jpegdec_cb)) {
            jpeg.decode(0, 0, 0);
            jpeg.close();
            // lv_obj_invalidate(canvas_obj);
            lv_area_t canvas_area;
            lv_obj_get_coords(canvas_obj, &canvas_area);
            _lv_inv_area(lv_disp_get_default(), &canvas_area);
        }
        uint32_t t3 = millis();
        // lv_obj_invalidate(canvas_obj);

        //Timer_Loop();  // 强制立即刷新
        uint32_t t4 = millis();
        
        // //debug
        // static uint32_t dbg = 0;
        // if (dbg++ % 20 == 0) {
        //     printf("SD: %dms 解码: %dms 渲染: %dms\n", t2-t1, t3-t2, t4-t3);
        // }
        // //
    }
}

//sd card end

void setup()
{       
  LCD_Init();
  Lvgl_Init();
  ui_init();
  SD_Init();
  Video_Init();
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
        if (line == "OFF") {
          // 主机端正常退出：把时间戳推回 10 秒前，让下面的超时逻辑立即关屏
          last_data_time = millis() - 10001;
          line = "";           // 清空后，下面的 8 段校验会自然忽略这一行
        }
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
          //lv_obj_clear_flag(objects.backgroumd_image, LV_OBJ_FLAG_HIDDEN);
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
    //lv_obj_add_flag(objects.backgroumd_image, LV_OBJ_FLAG_HIDDEN);

    lv_obj_set_style_bg_color(objects.status_point, lv_color_hex(0xff0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    set_var_cpu_temp("no");
    set_var_cpu_load("--");
    set_var_gpu_temp("sig");
    set_var_gpu_load("--");
    set_var_cpu_rpm("pls");
    set_var_case_rpm("chk");
    set_var_gpu_rpm("your");
    set_var_side_rpm("conn");
    set_var_cpu_load_bar(0);   // 断连时进度条归零，否则会停在上次的值
    set_var_gpu_load_bar(0);
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

// ====== video play =====

if (is_connected) {
    static uint32_t last_sd_check = 0;
    if (millis() - last_sd_check > 5000) {
        sd_available = SD.exists("/background10fps.mjpeg");
        if (!sd_available && mjpegFile) {
            mjpegFile.close();
            mjpegFile = File();
        }
        last_sd_check = millis();
    }

    if (sd_available) {
        lv_obj_add_flag(objects.backgroumd_image, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(objects.video_canvas, LV_OBJ_FLAG_HIDDEN);
        Video_Play_Loop();
    } else {
        lv_obj_clear_flag(objects.backgroumd_image, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(objects.video_canvas, LV_OBJ_FLAG_HIDDEN);
    }
}


  Timer_Loop();
  //ui_tick();
  static uint32_t last_ui = 0;
  if (millis() - last_ui > 100) {  // 改成 100ms 调用一次
      ui_tick();
      last_ui = millis();
  }

  delay(5);
}