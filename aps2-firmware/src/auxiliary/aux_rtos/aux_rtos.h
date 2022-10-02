#ifndef __AUX_RTOS_H_
#define __AUX_RTOS_H_

#include "auxiliary/aux_hw/aux_hw.h"

#define TASK_LCD_STACK_SIZE (1024 * 6 / sizeof(portSTACK_TYPE))
#define TASK_LCD_STACK_PRIORITY (tskIDLE_PRIORITY)

#define TASK_ADC_STACK_SIZE (1024 * 10 / sizeof(portSTACK_TYPE))
#define TASK_ADC_STACK_PRIORITY (tskIDLE_PRIORITY)

#define TASK_PROC_STACK_SIZE (1024 * 10 / sizeof(portSTACK_TYPE))
#define TASK_PROC_STACK_PRIORITY (tskIDLE_PRIORITY)

#define TASK_TRACK_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_TRACK_STACK_PRIORITY (tskIDLE_PRIORITY)

#define TASK_CALC_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_CALC_STACK_PRIORITY (tskIDLE_PRIORITY)

#define TASK_BUT_STACK_SIZE (2048 / sizeof(portSTACK_TYPE))
#define TASK_BUT_STACK_PRIORITY (tskIDLE_PRIORITY)

typedef struct
{
	uint value;
} adcData;

typedef struct
{
	int size;
	char cm;
} aroData;

typedef struct _traject_duration
{
	int alarm_sec, alarm_h, alarm_min;
} traject_duration;

// getters & setters
SemaphoreHandle_t get_xSemaphoreRTC(void);
SemaphoreHandle_t get_xSemaphoreRTCTrack(void);
SemaphoreHandle_t get_xSemaphoreChangeScreen(void);
SemaphoreHandle_t get_xSemaphoreNextUnity(void);
SemaphoreHandle_t get_xSemaphoreInstVel(void);
SemaphoreHandle_t get_xSemaphoreTimer(void);
SemaphoreHandle_t get_xSemaphorePlay(void);
SemaphoreHandle_t get_xSemaphoreEnd(void);
SemaphoreHandle_t get_xSemaphoreSensor(void);
SemaphoreHandle_t get_xSemaphoreSensorTrack(void);
SemaphoreHandle_t get_xSemaphoreMutex(void);

QueueHandle_t get_xQueueAroSize(void);
QueueHandle_t get_but_queue(void);

xTaskHandle xTask_update_config_handle, xTask_update_handle, xTask_track_handle;

// rtos functions
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName);
void vApplicationIdleHook(void);
void vApplicationTickHook(void);
void vApplicationMallocFailedHook(void);
void xPortSysTickHandler(void);

// tasks
void task_lcd(void *pvParameters);
void task_update_clk(void *pvParameters);

// init rtos
void init_rtos(void);

#endif