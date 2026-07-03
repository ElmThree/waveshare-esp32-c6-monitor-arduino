#include "styles.h"
#include "images.h"
#include "fonts.h"

#include "ui.h"
#include "screens.h"

//
// Style: CPU panel
//

void init_style_cpu_panel_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_border_color(style, lv_color_hex(0x1d1d1d));
    lv_style_set_bg_color(style, lv_color_hex(0x000000));
    lv_style_set_radius(style, 0);
    lv_style_set_border_width(style, 1);
    lv_style_set_text_color(style, lv_color_hex(0x5cc051));
    lv_style_set_bg_opa(style, 180);
};

lv_style_t *get_style_cpu_panel_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_cpu_panel_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_cpu_panel_SCROLLBAR_CHECKED(lv_style_t *style) {
    lv_style_set_line_width(style, 2);
    lv_style_set_line_color(style, lv_color_hex(0xe90e0e));
};

lv_style_t *get_style_cpu_panel_SCROLLBAR_CHECKED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_cpu_panel_SCROLLBAR_CHECKED(style);
    }
    return style;
};

void init_style_cpu_panel_SCROLLBAR_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0x5cc051));
};

lv_style_t *get_style_cpu_panel_SCROLLBAR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_cpu_panel_SCROLLBAR_DEFAULT(style);
    }
    return style;
};

void add_style_cpu_panel(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_cpu_panel_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_cpu_panel_SCROLLBAR_CHECKED(), LV_PART_SCROLLBAR | LV_STATE_CHECKED);
    lv_obj_add_style(obj, get_style_cpu_panel_SCROLLBAR_DEFAULT(), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
};

void remove_style_cpu_panel(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_cpu_panel_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_cpu_panel_SCROLLBAR_CHECKED(), LV_PART_SCROLLBAR | LV_STATE_CHECKED);
    lv_obj_remove_style(obj, get_style_cpu_panel_SCROLLBAR_DEFAULT(), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
};

//
// Style: GPU panel
//

void init_style_gpu_panel_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_border_color(style, lv_color_hex(0x1d1d1d));
    lv_style_set_bg_color(style, lv_color_hex(0x000000));
    lv_style_set_radius(style, 0);
    lv_style_set_border_width(style, 1);
    lv_style_set_pad_top(style, 13);
    lv_style_set_pad_bottom(style, 13);
    lv_style_set_text_color(style, lv_color_hex(0xe02929));
    lv_style_set_bg_opa(style, 180);
};

lv_style_t *get_style_gpu_panel_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_gpu_panel_MAIN_DEFAULT(style);
    }
    return style;
};

void init_style_gpu_panel_SCROLLBAR_CHECKED(lv_style_t *style) {
    lv_style_set_line_width(style, 2);
    lv_style_set_line_color(style, lv_color_hex(0xe90e0e));
};

lv_style_t *get_style_gpu_panel_SCROLLBAR_CHECKED() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_gpu_panel_SCROLLBAR_CHECKED(style);
    }
    return style;
};

void init_style_gpu_panel_SCROLLBAR_DEFAULT(lv_style_t *style) {
    lv_style_set_bg_opa(style, 255);
    lv_style_set_bg_color(style, lv_color_hex(0xb61515));
};

lv_style_t *get_style_gpu_panel_SCROLLBAR_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_gpu_panel_SCROLLBAR_DEFAULT(style);
    }
    return style;
};

void add_style_gpu_panel(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_gpu_panel_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(obj, get_style_gpu_panel_SCROLLBAR_CHECKED(), LV_PART_SCROLLBAR | LV_STATE_CHECKED);
    lv_obj_add_style(obj, get_style_gpu_panel_SCROLLBAR_DEFAULT(), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
};

void remove_style_gpu_panel(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_gpu_panel_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(obj, get_style_gpu_panel_SCROLLBAR_CHECKED(), LV_PART_SCROLLBAR | LV_STATE_CHECKED);
    lv_obj_remove_style(obj, get_style_gpu_panel_SCROLLBAR_DEFAULT(), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
};

//
// Style: FAN panel
//

void init_style_fan_panel_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_line_width(style, 2);
    lv_style_set_line_color(style, lv_color_hex(0xff0000));
    lv_style_set_bg_color(style, lv_color_hex(0x272b32));
};

lv_style_t *get_style_fan_panel_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_fan_panel_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_fan_panel(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_fan_panel_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_fan_panel(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_fan_panel_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: back text style
//

void init_style_back_text_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0x6b7280));
};

lv_style_t *get_style_back_text_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_back_text_style_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_back_text_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_back_text_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_back_text_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_back_text_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: back little text style
//

void init_style_back_little_text_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0x6b7280));
    lv_style_set_text_font(style, &lv_font_montserrat_12);
};

lv_style_t *get_style_back_little_text_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_back_little_text_style_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_back_little_text_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_back_little_text_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_back_little_text_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_back_little_text_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: but panel style
//

void init_style_but_panel_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_font(style, &lv_font_montserrat_10);
    lv_style_set_text_color(style, lv_color_hex(0x979797));
};

lv_style_t *get_style_but_panel_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_but_panel_style_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_but_panel_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_but_panel_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_but_panel_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_but_panel_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: CPU temp style
//

void init_style_cpu_temp_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0x3fb07b));
};

lv_style_t *get_style_cpu_temp_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_cpu_temp_style_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_cpu_temp_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_cpu_temp_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_cpu_temp_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_cpu_temp_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: GPU temp style
//

void init_style_gpu_temp_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0x3fb07b));
};

lv_style_t *get_style_gpu_temp_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_gpu_temp_style_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_gpu_temp_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_gpu_temp_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_gpu_temp_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_gpu_temp_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: fan in style
//

void init_style_fan_in_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0x3593ba));
    lv_style_set_min_width(style, 20);
    lv_style_set_text_align(style, LV_TEXT_ALIGN_RIGHT);
    lv_style_set_transform_height(style, 50);
    lv_style_set_text_font(style, &lv_font_montserrat_20);
    lv_style_set_transform_width(style, 0);
};

lv_style_t *get_style_fan_in_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_fan_in_style_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_fan_in_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_fan_in_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_fan_in_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_fan_in_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
// Style: fan out style
//

void init_style_fan_out_style_MAIN_DEFAULT(lv_style_t *style) {
    lv_style_set_text_color(style, lv_color_hex(0xba3030));
    lv_style_set_min_width(style, 20);
    lv_style_set_text_align(style, LV_TEXT_ALIGN_RIGHT);
    lv_style_set_transform_height(style, 50);
    lv_style_set_text_font(style, &lv_font_montserrat_20);
    lv_style_set_transform_width(style, 0);
    lv_style_set_img_opa(style, 0);
    lv_style_set_bg_img_opa(style, 0);
};

lv_style_t *get_style_fan_out_style_MAIN_DEFAULT() {
    static lv_style_t *style;
    if (!style) {
        style = (lv_style_t *)lv_mem_alloc(sizeof(lv_style_t));
        lv_style_init(style);
        init_style_fan_out_style_MAIN_DEFAULT(style);
    }
    return style;
};

void add_style_fan_out_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_add_style(obj, get_style_fan_out_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

void remove_style_fan_out_style(lv_obj_t *obj) {
    (void)obj;
    lv_obj_remove_style(obj, get_style_fan_out_style_MAIN_DEFAULT(), LV_PART_MAIN | LV_STATE_DEFAULT);
};

//
//
//

void add_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*AddStyleFunc)(lv_obj_t *obj);
    static const AddStyleFunc add_style_funcs[] = {
        add_style_cpu_panel,
        add_style_gpu_panel,
        add_style_fan_panel,
        add_style_back_text_style,
        add_style_back_little_text_style,
        add_style_but_panel_style,
        add_style_cpu_temp_style,
        add_style_gpu_temp_style,
        add_style_fan_in_style,
        add_style_fan_out_style,
    };
    add_style_funcs[styleIndex](obj);
}

void remove_style(lv_obj_t *obj, int32_t styleIndex) {
    typedef void (*RemoveStyleFunc)(lv_obj_t *obj);
    static const RemoveStyleFunc remove_style_funcs[] = {
        remove_style_cpu_panel,
        remove_style_gpu_panel,
        remove_style_fan_panel,
        remove_style_back_text_style,
        remove_style_back_little_text_style,
        remove_style_but_panel_style,
        remove_style_cpu_temp_style,
        remove_style_gpu_temp_style,
        remove_style_fan_in_style,
        remove_style_fan_out_style,
    };
    remove_style_funcs[styleIndex](obj);
}