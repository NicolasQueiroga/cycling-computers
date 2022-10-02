#ifndef __AUX_LVGL_H_
#define __AUX_LVGL_H_

#include "auxiliary/aux_hw/aux_hw.h"
#include <string.h>
#include "lvgl.h"
#include "ili9341.h"
#include "touch/touch.h"

#define LV_HOR_RES_MAX (320)
#define LV_VER_RES_MAX (240)

// auxiliary functions
void create_btn(lv_obj_t *parent, lv_event_cb_t event_handler, lv_obj_t **btn, lv_event_code_t filter, int width, int height, lv_style_t *style);
void create_label_for_btn(lv_obj_t *parent, lv_obj_t *label, const char *text);
void create_label(lv_obj_t *parent, lv_obj_t **label, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs);

// event handlers
void event_handler(lv_event_t *e);
void play_handler(lv_event_t *e);
void end_handler(lv_event_t *e);
void config_handler(lv_event_t *e);
void next_unidade_handler(lv_event_t *e);
void add_aro_handler(lv_event_t *e);
void sub_aro_handler(lv_event_t *e);

// setup
void my_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
void my_input_read(lv_indev_drv_t *drv, lv_indev_data_t *data);
void configure_lvgl(void);

#endif