#include "aux_lvgl.h"
#include "auxiliary/aux_rtos/aux_rtos.h"
#include "auxiliary/aux_time/aux_time.h"

// buffers
static lv_disp_draw_buf_t disp_buf;
static lv_color_t buf_1[LV_HOR_RES_MAX * LV_VER_RES_MAX];
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;

// global variables

// auxiliary functions

// need to allign outside
void create_btn(lv_obj_t *parent, lv_event_cb_t event_handler, lv_obj_t **btn, lv_event_code_t filter,
                int width, int height,
                lv_style_t *style)
{
    *btn = lv_btn_create(parent);
    lv_obj_add_event_cb(*btn, event_handler, filter, NULL);
    lv_obj_set_size(*btn, width, height);
    lv_obj_add_style(*btn, style, 0);
}

void create_label_for_btn(
    lv_obj_t *parent, lv_obj_t *label,
    const char *text)
{
    label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_center(label);
}

// need to set font, font collor and text outside
void create_label(lv_obj_t *parent, lv_obj_t **label, lv_align_t align, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    *label = lv_label_create(parent);
    lv_obj_align(*label, align, x_ofs, y_ofs);
}

// event handlers
void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
        LV_LOG_USER("Clicked");
    else if (code == LV_EVENT_VALUE_CHANGED)
        LV_LOG_USER("Toggled");
}

void play_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(get_xSemaphorePlay(), &xHigherPriorityTaskWoken);
    }
}

void end_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(get_xSemaphoreEnd(), &xHigherPriorityTaskWoken);
    }
}

void config_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(get_xSemaphoreChangeScreen(), &xHigherPriorityTaskWoken);
    }
}

void next_unidade_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(get_xSemaphoreNextUnity(), &xHigherPriorityTaskWoken);
    }
}

void add_aro_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        int aro_increase = 1;
        xQueueSend(get_xQueueAroSize(), (void *)&aro_increase, 10);
    }
}

void sub_aro_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        int aro_decrease = -1;
        xQueueSend(get_xQueueAroSize(), (void *)&aro_decrease, 10);
    }
}

// setup
void my_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    ili9341_set_top_left_limit(area->x1, area->y1);
    ili9341_set_bottom_right_limit(area->x2, area->y2);
    ili9341_copy_pixels_to_screen(color_p, (area->x2 + 1 - area->x1) * (area->y2 + 1 - area->y1));
    lv_disp_flush_ready(disp_drv);
}

void my_input_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    int px, py, pressed;
    if (readPoint(&px, &py))
        data->state = LV_INDEV_STATE_PRESSED;
    else
        data->state = LV_INDEV_STATE_RELEASED;

    data->point.x = px;
    data->point.y = py;
}

void configure_lvgl(void)
{
    lv_init();
    lv_disp_draw_buf_init(&disp_buf, buf_1, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX);

    lv_disp_drv_init(&disp_drv);       /*Basic initialization*/
    disp_drv.draw_buf = &disp_buf;     /*Set an initialized buffer*/
    disp_drv.flush_cb = my_flush_cb;   /*Set a flush callback to draw to the display*/
    disp_drv.hor_res = LV_HOR_RES_MAX; /*Set the horizontal resolution in pixels*/
    disp_drv.ver_res = LV_VER_RES_MAX; /*Set the vertical resolution in pixels*/
    disp_drv.sw_rotate = 1;

    lv_disp_t *disp;
    disp = lv_disp_drv_register(&disp_drv); /*Register the driver and save the created display objects*/
    lv_disp_set_rotation(disp, LV_DISP_ROT_270);

    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_input_read;
    lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv);
}
