#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_CPU_TEMP = 0,
    FLOW_GLOBAL_VARIABLE_CPU_LOAD = 1,
    FLOW_GLOBAL_VARIABLE_GPU_TEMP = 2,
    FLOW_GLOBAL_VARIABLE_GPU_LOAD = 3,
    FLOW_GLOBAL_VARIABLE_CPU_RPM = 4,
    FLOW_GLOBAL_VARIABLE_CASE_RPM = 5,
    FLOW_GLOBAL_VARIABLE_GPU_RPM = 6,
    FLOW_GLOBAL_VARIABLE_SIDE_RPM = 7,
    FLOW_GLOBAL_VARIABLE_CPU_LOAD_BAR = 8,
    FLOW_GLOBAL_VARIABLE_GPU_LOAD_BAR = 9,
    FLOW_GLOBAL_VARIABLE_CHIP_TEMP = 10
};

// Native global variables

extern const char *get_var_cpu_temp();
extern void set_var_cpu_temp(const char *value);
extern const char *get_var_cpu_load();
extern void set_var_cpu_load(const char *value);
extern const char *get_var_gpu_temp();
extern void set_var_gpu_temp(const char *value);
extern const char *get_var_gpu_load();
extern void set_var_gpu_load(const char *value);
extern const char *get_var_cpu_rpm();
extern void set_var_cpu_rpm(const char *value);
extern const char *get_var_case_rpm();
extern void set_var_case_rpm(const char *value);
extern const char *get_var_gpu_rpm();
extern void set_var_gpu_rpm(const char *value);
extern const char *get_var_side_rpm();
extern void set_var_side_rpm(const char *value);
extern float get_var_cpu_load_bar();
extern void set_var_cpu_load_bar(float value);
extern float get_var_gpu_load_bar();
extern void set_var_gpu_load_bar(float value);
extern const char *get_var_chip_temp();
extern void set_var_chip_temp(const char *value);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/