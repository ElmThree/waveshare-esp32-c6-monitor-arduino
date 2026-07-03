#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN = 1,
    _SCREEN_ID_LAST = 1
};

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *backgroumd_image;
    lv_obj_t *cpu;
    lv_obj_t *cpu_temp;
    lv_obj_t *usage_cpu;
    lv_obj_t *cpu_load_bar;
    lv_obj_t *gpu;
    lv_obj_t *gpu_load;
    lv_obj_t *gpu_temp;
    lv_obj_t *usage_gpu;
    lv_obj_t *gpu_load_bar;
    lv_obj_t *cpuin;
    lv_obj_t *cpuout;
    lv_obj_t *gpuin;
    lv_obj_t *gpuout;
    lv_obj_t *status_point;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
} objects_t;

extern objects_t objects;

void create_screen_main();
void tick_screen_main();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/