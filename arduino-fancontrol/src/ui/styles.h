#ifndef EEZ_LVGL_UI_STYLES_H
#define EEZ_LVGL_UI_STYLES_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Style: background
lv_style_t *get_style_background_MAIN_DEFAULT();
void add_style_background(lv_obj_t *obj);
void remove_style_background(lv_obj_t *obj);

// Style: FAN panel
lv_style_t *get_style_fan_panel_MAIN_DEFAULT();
void add_style_fan_panel(lv_obj_t *obj);
void remove_style_fan_panel(lv_obj_t *obj);

// Style: GPU panel
lv_style_t *get_style_gpu_panel_MAIN_DEFAULT();
lv_style_t *get_style_gpu_panel_SCROLLBAR_CHECKED();
lv_style_t *get_style_gpu_panel_SCROLLBAR_DEFAULT();
void add_style_gpu_panel(lv_obj_t *obj);
void remove_style_gpu_panel(lv_obj_t *obj);

// Style: CPU panel
lv_style_t *get_style_cpu_panel_MAIN_DEFAULT();
lv_style_t *get_style_cpu_panel_SCROLLBAR_CHECKED();
lv_style_t *get_style_cpu_panel_SCROLLBAR_DEFAULT();
void add_style_cpu_panel(lv_obj_t *obj);
void remove_style_cpu_panel(lv_obj_t *obj);

// Style: title text style
lv_style_t *get_style_title_text_style_MAIN_DEFAULT();
void add_style_title_text_style(lv_obj_t *obj);
void remove_style_title_text_style(lv_obj_t *obj);

// Style: load text style
lv_style_t *get_style_load_text_style_MAIN_DEFAULT();
void add_style_load_text_style(lv_obj_t *obj);
void remove_style_load_text_style(lv_obj_t *obj);

// Style: button text style
lv_style_t *get_style_button_text_style_MAIN_DEFAULT();
void add_style_button_text_style(lv_obj_t *obj);
void remove_style_button_text_style(lv_obj_t *obj);

// Style: CPU temp style
lv_style_t *get_style_cpu_temp_style_MAIN_DEFAULT();
void add_style_cpu_temp_style(lv_obj_t *obj);
void remove_style_cpu_temp_style(lv_obj_t *obj);

// Style: GPU temp style
lv_style_t *get_style_gpu_temp_style_MAIN_DEFAULT();
void add_style_gpu_temp_style(lv_obj_t *obj);
void remove_style_gpu_temp_style(lv_obj_t *obj);

// Style: fan in style
lv_style_t *get_style_fan_in_style_MAIN_DEFAULT();
void add_style_fan_in_style(lv_obj_t *obj);
void remove_style_fan_in_style(lv_obj_t *obj);

// Style: fan out style
lv_style_t *get_style_fan_out_style_MAIN_DEFAULT();
void add_style_fan_out_style(lv_obj_t *obj);
void remove_style_fan_out_style(lv_obj_t *obj);

// Style: rpm style
lv_style_t *get_style_rpm_style_MAIN_DEFAULT();
void add_style_rpm_style(lv_obj_t *obj);
void remove_style_rpm_style(lv_obj_t *obj);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_STYLES_H*/