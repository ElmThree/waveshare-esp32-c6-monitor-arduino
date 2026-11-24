#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

//
// Screens
//

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 172);
    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // backgroumd_image
            lv_obj_t *obj = lv_animimg_create(parent_obj);
            objects.backgroumd_image = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 172);
            static const lv_img_dsc_t *images[2] = {
                &img__4,
                &img__5,
            };
            lv_animimg_set_src(obj, (const void **)images, 2);
            lv_animimg_set_duration(obj, 20000);
            lv_animimg_set_repeat_count(obj, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(obj);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
        }
        {
            // video_canvas
            lv_obj_t *obj = lv_canvas_create(parent_obj);
            objects.video_canvas = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 172);
        }
        {
            // CPU
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cpu = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 160, 110);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_cpu_panel(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 51, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_title_text_style(obj);
                    lv_obj_set_style_x(obj, 51, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_y(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "CPU");
                }
                {
                    // cpu_temp
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.cpu_temp = obj;
                    lv_obj_set_pos(obj, -20, 14);
                    lv_obj_set_size(obj, 114, 54);
                    add_style_cpu_temp_style(obj);
                    lv_obj_set_style_x(obj, -15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_y(obj, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_min_width(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_max_width(obj, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 5, 17);
                    lv_obj_set_size(obj, 25, 30);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_robot_mono_64, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_x(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_y(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "°");
                }
                {
                    // usage cpu
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.usage_cpu = obj;
                    lv_obj_set_pos(obj, 33, 76);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_load_text_style(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_max_width(obj, 22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_min_width(obj, 22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 57, 76);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_load_text_style(obj);
                    lv_label_set_text_static(obj, "% load");
                }
                {
                    // cpu_load_bar
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.cpu_load_bar = obj;
                    lv_obj_set_pos(obj, -15, 94);
                    lv_obj_set_size(obj, 161, 3);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x89f870), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_clip_corner(obj, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_blend_mode(obj, LV_BLEND_MODE_NORMAL, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // GPU
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.gpu = obj;
            lv_obj_set_pos(obj, 160, 0);
            lv_obj_set_size(obj, 160, 110);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_gpu_panel(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // gpu_load
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.gpu_load = obj;
                    lv_obj_set_pos(obj, 50, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_title_text_style(obj);
                    lv_obj_set_style_x(obj, 51, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_y(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "GPU");
                }
                {
                    // gpu_temp
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.gpu_temp = obj;
                    lv_obj_set_pos(obj, -17, -23);
                    lv_obj_set_size(obj, 114, 54);
                    add_style_gpu_temp_style(obj);
                    lv_obj_set_style_min_width(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_max_width(obj, 120, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_x(obj, -15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_y(obj, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, -15, 17);
                    lv_obj_set_size(obj, 20, 20);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &ui_font_robot_mono_64, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_x(obj, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_y(obj, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "°");
                }
                {
                    // usage gpu
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.usage_gpu = obj;
                    lv_obj_set_pos(obj, 33, 76);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_load_text_style(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_max_width(obj, 22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_min_width(obj, 22, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 57, 76);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_load_text_style(obj);
                    lv_label_set_text_static(obj, "% load");
                }
                {
                    // gpu_load_bar
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.gpu_load_bar = obj;
                    lv_obj_set_pos(obj, -15, 94);
                    lv_obj_set_size(obj, 161, 3);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_color(obj, lv_color_hex(0x89f870), LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_radius(obj, 5, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_clip_corner(obj, false, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                    lv_obj_set_style_blend_mode(obj, LV_BLEND_MODE_NORMAL, LV_PART_INDICATOR | LV_STATE_DEFAULT);
                }
            }
        }
        {
            // CPUIN
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cpuin = obj;
            lv_obj_set_pos(obj, 0, 110);
            lv_obj_set_size(obj, 80, 62);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_fan_panel(obj);
            lv_obj_set_style_border_side(obj, LV_BORDER_SIDE_TOP, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj1 = obj;
                    lv_obj_set_pos(obj, -15, 2);
                    lv_obj_set_size(obj, 59, 23);
                    add_style_fan_in_style(obj);
                    lv_label_set_text(obj, "");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 44, 10);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_rpm_style(obj);
                    lv_label_set_text_static(obj, "RPM");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_button_text_style(obj);
                    lv_label_set_text_static(obj, "CPU FAN");
                }
            }
        }
        {
            // CPUOUT
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cpuout = obj;
            lv_obj_set_pos(obj, 80, 110);
            lv_obj_set_size(obj, 80, 62);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            add_style_fan_panel(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 3, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_button_text_style(obj);
                    lv_label_set_text_static(obj, "CASE FAN");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 43, 10);
                    lv_obj_set_size(obj, 24, 10);
                    add_style_rpm_style(obj);
                    lv_label_set_text_static(obj, "RPM");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj2 = obj;
                    lv_obj_set_pos(obj, -15, 2);
                    lv_obj_set_size(obj, 59, 23);
                    add_style_fan_out_style(obj);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            // GPUIN
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.gpuin = obj;
            lv_obj_set_pos(obj, 160, 110);
            lv_obj_set_size(obj, 80, 62);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            add_style_fan_panel(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 8, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_button_text_style(obj);
                    lv_label_set_text_static(obj, "GPU FAN");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 43, 10);
                    lv_obj_set_size(obj, 24, 10);
                    add_style_rpm_style(obj);
                    lv_label_set_text_static(obj, "RPM");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj3 = obj;
                    lv_obj_set_pos(obj, -15, 2);
                    lv_obj_set_size(obj, 59, 23);
                    add_style_fan_in_style(obj);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            // GPUOUT
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.gpuout = obj;
            lv_obj_set_pos(obj, 240, 110);
            lv_obj_set_size(obj, 80, 62);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            add_style_fan_panel(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 2, 28);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    add_style_button_text_style(obj);
                    lv_label_set_text_static(obj, "SIDE FAN");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 43, 10);
                    lv_obj_set_size(obj, 24, 10);
                    add_style_rpm_style(obj);
                    lv_label_set_text_static(obj, "RPM");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.obj4 = obj;
                    lv_obj_set_pos(obj, -15, 2);
                    lv_obj_set_size(obj, 59, 23);
                    add_style_fan_out_style(obj);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            // status_point
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.status_point = obj;
            lv_obj_set_pos(obj, 305, 10);
            lv_obj_set_size(obj, 5, 5);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x2df600), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 290, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
    {
        const char *new_val = get_var_cpu_temp();
        const char *cur_val = lv_label_get_text(objects.cpu_temp);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.cpu_temp;
            lv_label_set_text(objects.cpu_temp, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_cpu_load();
        const char *cur_val = lv_label_get_text(objects.usage_cpu);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.usage_cpu;
            lv_label_set_text(objects.usage_cpu, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_cpu_load_bar();
        int32_t cur_val = lv_bar_get_value(objects.cpu_load_bar);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.cpu_load_bar;
            lv_bar_set_value(objects.cpu_load_bar, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_gpu_temp();
        const char *cur_val = lv_label_get_text(objects.gpu_temp);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.gpu_temp;
            lv_label_set_text(objects.gpu_temp, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_gpu_load();
        const char *cur_val = lv_label_get_text(objects.usage_gpu);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.usage_gpu;
            lv_label_set_text(objects.usage_gpu, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        int32_t new_val = get_var_gpu_load_bar();
        int32_t cur_val = lv_bar_get_value(objects.gpu_load_bar);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.gpu_load_bar;
            lv_bar_set_value(objects.gpu_load_bar, new_val, LV_ANIM_OFF);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_cpu_rpm();
        const char *cur_val = lv_label_get_text(objects.obj1);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj1;
            lv_label_set_text(objects.obj1, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_case_rpm();
        const char *cur_val = lv_label_get_text(objects.obj2);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj2;
            lv_label_set_text(objects.obj2, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_gpu_rpm();
        const char *cur_val = lv_label_get_text(objects.obj3);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj3;
            lv_label_set_text(objects.obj3, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_side_rpm();
        const char *cur_val = lv_label_get_text(objects.obj4);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj4;
            lv_label_set_text(objects.obj4, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_chip_temp();
        const char *cur_val = lv_label_get_text(objects.obj0);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.obj0;
            lv_label_set_text(objects.obj0, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 1) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
    { "RobotMono_64", &ui_font_robot_mono_64 },
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_main();
}