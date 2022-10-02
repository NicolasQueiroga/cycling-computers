#include "auxiliary/aux_time/aux_time.h"
#include "auxiliary/aux_lvgl/aux_lvgl.h"
#include "auxiliary/aux_lvgl/aux_screens.h"
#include "arm_math.h"
#include "aux_rtos.h"

// semaphores
volatile SemaphoreHandle_t xSemaphoreRTC, xSemaphoreRTCTrack, xSemaphoreChangeScreen, xSemaphoreNextUnity, xSemaphoreMeanVel, xSemaphoreTimer, xSemaphorePlay, xSemaphoreEnd, xSemaphoreSensor, xSemaphoreSensorTrack;
volatile SemaphoreHandle_t xSemaphoreMutex, xSemaphoreMutexWrite;

// queues
volatile QueueHandle_t xQueueAroSize, xQueueAroChange, xQueueNewAro, xQueueNewAro2, xQueueBut;

// getters & setters
SemaphoreHandle_t get_xSemaphoreRTC(void) { return xSemaphoreRTC; }
SemaphoreHandle_t get_xSemaphoreRTCTrack(void) { return xSemaphoreRTCTrack; }
SemaphoreHandle_t get_xSemaphoreChangeScreen(void) { return xSemaphoreChangeScreen; }
SemaphoreHandle_t get_xSemaphoreNextUnity(void) { return xSemaphoreNextUnity; }
SemaphoreHandle_t get_xSemaphoreInstVel(void) { return xSemaphoreMeanVel; }
SemaphoreHandle_t get_xSemaphoreTimer(void) { return xSemaphoreTimer; }
SemaphoreHandle_t get_xSemaphorePlay(void) { return xSemaphorePlay; }
SemaphoreHandle_t get_xSemaphoreEnd(void) { return xSemaphoreEnd; }
SemaphoreHandle_t get_xSemaphoreSensor(void) { return xSemaphoreSensor; }
SemaphoreHandle_t get_xSemaphoreSensorTrack(void) { return xSemaphoreSensorTrack; }
SemaphoreHandle_t get_xSemaphoreMutex(void) { return xSemaphoreMutex; }

QueueHandle_t get_xQueueAroSize(void) { return xQueueAroSize; }
QueueHandle_t get_but_queue(void) { return xQueueBut; }

// rtos functions
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName)
{
	printf("stack overflow %u %s\r\n", pxTask, (portCHAR *)pcTaskName);
	while (1)
		;
}
void vApplicationIdleHook(void) { pmc_sleep(SAM_PM_SMODE_SLEEP_WFI); }
void vApplicationTickHook(void) {}
void vApplicationMallocFailedHook(void) { configASSERT((volatile void *)NULL); }

// tasks
void task_lcd(void *pvParameters)
{
	lv_bike();
	char config_flag = 0;
	int radius = 22;
	char cm = 0;
	aroData aro;

	while (1)
	{
		if (xQueueReceive(xQueueAroChange, &(aro), 10))
		{
			radius = aro.size;
			cm = aro.cm;
		}

		if (xSemaphoreTake(xSemaphoreChangeScreen, 10 / portTICK_PERIOD_MS))
		{
			if (!config_flag)
			{
				vTaskSuspend(xTask_update_handle);
				vTaskSuspend(xTask_track_handle);
				xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
				lv_obj_clean(lv_scr_act());
				lv_bike_config();
				xSemaphoreGive(xSemaphoreMutexWrite);
				vTaskResume(xTask_update_config_handle);
			}
			else
			{
				vTaskSuspend(xTask_update_config_handle);
				aroData newAro = {radius, cm};
				xQueueSend(xQueueNewAro, &newAro, 0);
				xQueueSend(xQueueNewAro2, &newAro, 0);
				xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
				lv_obj_clean(lv_scr_act());
				lv_bike();
				xSemaphoreGive(xSemaphoreMutexWrite);
				vTaskResume(xTask_update_handle);
				vTaskResume(xTask_track_handle);
			}
			config_flag = !config_flag;
		}

		xSemaphoreTake(xSemaphoreMutex, portMAX_DELAY);
		lv_tick_inc(50);
		lv_task_handler();
		xSemaphoreGive(xSemaphoreMutex);
		vTaskDelay(50);
	}
}

void task_clock(void *pvParameters)
{
	/* RTC init */
	Calendar rtc_initial = {2022, 5, 20, 12, 15, 45, 1};
	RTC_init(RTC, ID_RTC, rtc_initial, RTC_SR_SEC | RTC_SR_ALARM);
	uint32_t current_hour, current_min, current_sec;

	while (1)
	{
		if (xSemaphoreTake(xSemaphoreRTC, 1000 / portTICK_PERIOD_MS))
		{
			rtc_get_time(RTC, &current_hour, &current_min, &current_sec);
			xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
			lv_label_set_text_fmt(getLabelClock(), "%02d:%02d", current_hour, current_min);
			lv_label_set_text_fmt(getLabelClockSec(), "%02d", current_sec);
			xSemaphoreGive(xSemaphoreMutexWrite);
		}
	}
}

void task_config(void *pvParameters)
{
	char *s;
	int aro_size_change;
	int radius = 22;
	char cm = 0;
	while (1)
	{
		if (xSemaphoreTake(xSemaphoreNextUnity, 10 / portTICK_PERIOD_MS))
		{
			s = lv_label_get_text(getLabelAroUnidade());
			if (s[0] == 'p')
			{
				xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
				lv_label_set_text_fmt(getLabelAroUnidade(), "cm");
				cm = !cm;
				set_aroUnidade("cm");
				xSemaphoreGive(xSemaphoreMutexWrite);
			}
			else
			{
				xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
				lv_label_set_text_fmt(getLabelAroUnidade(), "pol");
				cm = 0;
				set_aroUnidade("pol");
				xSemaphoreGive(xSemaphoreMutexWrite);
			}
			aroData aro = {radius, cm};
			xQueueSend(xQueueAroChange, &aro, 0);
		}
		if (xQueueReceive(xQueueAroSize, &(aro_size_change), 10))
		{
			xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
			radius = atoi(lv_label_get_text(getLabelAroSize())) + aro_size_change;
			lv_label_set_text_fmt(getLabelAroSize(), "%02d", radius);
			set_aroSize(radius);
			xSemaphoreGive(xSemaphoreMutexWrite);
			aroData aro = {radius, cm};
			xQueueSend(xQueueAroChange, &aro, 0);
		}
		vTaskDelay(100);
	}
}

void task_update(void *pvParameters)
{
	int radius = 22;
	char cm = 0;
	volatile double radius_m = 22 / 100.0;
	volatile double last_time = 0;
	volatile double dt, tempo_s;
	volatile double v, w;
	volatile int pulsos;
	volatile double last_vel = 0;
	volatile double a = 0;
	aroData aro;

	RTT_init(100, 0, RTT_MR_RTTINCIEN);

	while (1)
	{
		if (xQueueReceive(xQueueNewAro, &(aro), 10))
		{
			radius = aro.size;
			cm = aro.cm;
		}

		if (xSemaphoreTake(xSemaphoreSensor, 10000))
		{
			pulsos = rtt_read_timer_value(RTT);
			tempo_s = (float)pulsos / 100.0;

			dt = tempo_s - last_time;
			last_time = tempo_s;

			if (!cm)
			{
				radius_m = (float)(radius * 2.54) / 100.0;
			}
			else
			{
				radius_m = (float)radius / 100.0;
			}
			w = (float)2 * PI / dt;
			v = (float)(radius_m * w) * 3.6;
		}
		else
		{
			v = 0.0;
		}
		a = v - last_vel;
		last_vel = v;

		// display velocidade
		xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
		lv_label_set_text_fmt(getLabelvelocidadeInstantanea(), "%02d", (int)v);
		lv_label_set_text_fmt(getLabelvelocidadeInstantaneaDecimal(), ",%01d", (int)((v - (int)v) * 10));
		xSemaphoreGive(xSemaphoreMutexWrite);

		// display aceleracao
		if (a < 0.5 && a > -0.5)
		{
			xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
			lv_obj_set_style_text_color(getLabelAceleracao(), lv_palette_main(LV_PALETTE_GREY), LV_STATE_DEFAULT);
			lv_label_set_text_fmt(getLabelAceleracao(), LV_SYMBOL_MINUS);
			xSemaphoreGive(xSemaphoreMutexWrite);
		}
		else if (a <= -0.5)
		{
			xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
			lv_obj_set_style_text_color(getLabelAceleracao(), lv_palette_main(LV_PALETTE_RED), LV_STATE_DEFAULT);
			lv_label_set_text_fmt(getLabelAceleracao(), LV_SYMBOL_DOWN);
			xSemaphoreGive(xSemaphoreMutexWrite);
		}
		else
		{
			xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
			lv_obj_set_style_text_color(getLabelAceleracao(), lv_palette_main(LV_PALETTE_BLUE), LV_STATE_DEFAULT);
			lv_label_set_text_fmt(getLabelAceleracao(), LV_SYMBOL_UP);
			xSemaphoreGive(xSemaphoreMutexWrite);
		}
	}
}

void task_track(void *pvParameters)
{
	int counter = 0;
	double dist = 0;
	volatile double dist_km = 0;
	_Bool running = 0;
	int radius = 22;
	double radius_m;
	char cm = 0;
	double v, w;
	_Bool dois_pontos = 1;
	aroData aro;
	traject_duration td = {0, 0, 0};

	while (1)
	{
		if (xQueueReceive(xQueueNewAro2, &(aro), 10))
		{
			radius = aro.size;
			cm = aro.cm;
		}

		if (xSemaphoreTake(xSemaphorePlay, 10 / portTICK_PERIOD_MS))
		{
			running = !running;
			if (running)
			{
				xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
				lv_obj_set_style_border_width(getrectangleRunning(), 0, 0);
				lv_obj_set_style_bg_color(getrectangleRunning(), lv_palette_main(LV_PALETTE_BLUE), 0);
				lv_label_set_text(getLabelStartStop(), LV_SYMBOL_PAUSE);
				xSemaphoreGive(xSemaphoreMutexWrite);
			}
			else
			{
				xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
				lv_obj_set_style_border_width(getrectangleRunning(), 1, 0);
				lv_obj_set_style_bg_color(getrectangleRunning(), lv_color_white(), 0);
				lv_label_set_text(getLabelStartStop(), LV_SYMBOL_PLAY);
				xSemaphoreGive(xSemaphoreMutexWrite);
			}
		}

		if (xSemaphoreTake(xSemaphoreEnd, 10 / portTICK_PERIOD_MS))
		{
			running = 0;
			counter = 0;
			td.alarm_sec = 0;
			td.alarm_min = 0;
			td.alarm_h = 0;
			dist = 0;
			xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
			lv_label_set_text_fmt(getLabeltempoTrajeto(), "%02d:%02d", 0, 0);
			lv_label_set_text_fmt(getLabelvelocidadeMediaTrajeto(), "%02d", 0);
			lv_label_set_text_fmt(getLabelvelocidadeMediaTrajetoDecimal(), ",%01d", 0);
			lv_label_set_text_fmt(getLabelDistanciaTrajeto(), "%02d", 0);
			lv_label_set_text_fmt(getLabelDistanciaTrajetoDecimal(), ",%01d", 0);
			lv_obj_set_style_border_width(getrectangleRunning(), 1, 0);
			lv_obj_set_style_bg_color(getrectangleRunning(), lv_color_white(), 0);
			lv_label_set_text(getLabelStartStop(), LV_SYMBOL_PLAY);
			xSemaphoreGive(xSemaphoreMutexWrite);
		}

		if (running)
		{
			if (xSemaphoreTake(xSemaphoreRTCTrack, 10 / portTICK_PERIOD_MS))
			{
				counter++;
				if (td.alarm_sec == 59 && td.alarm_min != 59)
				{
					td.alarm_min += 1;
					td.alarm_sec = 0;
				}
				if (td.alarm_min == 59 && td.alarm_sec == 59)
				{
					td.alarm_h += 1;
					td.alarm_min = 0;
					td.alarm_sec = 0;
				}
				else
					td.alarm_sec += 1;
				xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
				if (dois_pontos)
				{
					lv_label_set_text_fmt(getLabeltempoTrajeto(), "%02d %02d", td.alarm_h, td.alarm_min);
				}
				else
				{
					lv_label_set_text_fmt(getLabeltempoTrajeto(), "%02d:%02d", td.alarm_h, td.alarm_min);
				}
				v = (float)(dist / counter) * 3.6;
				lv_label_set_text_fmt(getLabelvelocidadeMediaTrajeto(), "%02d", (int)v);
				lv_label_set_text_fmt(getLabelvelocidadeMediaTrajetoDecimal(), ",%01d", (int)((v - (int)v) * 10));
				xSemaphoreGive(xSemaphoreMutexWrite);
				dois_pontos = !dois_pontos;
			}
			if (xSemaphoreTake(xSemaphoreSensorTrack, 10 / portTICK_PERIOD_MS))
			{
				if (!cm)
				{
					radius_m = (float)(radius * 2.54) / 100.0;
				}
				else
				{
					radius_m = (float)radius / 100.0;
				}
				xSemaphoreTake(xSemaphoreMutexWrite, portMAX_DELAY);
				dist += (float)2 * PI * radius_m;
				dist_km = (float)dist / 1000.0;
				lv_label_set_text_fmt(getLabelDistanciaTrajeto(), "%02d", (int)dist_km);
				lv_label_set_text_fmt(getLabelDistanciaTrajetoDecimal(), ",%01d", (int)(((float)dist_km - (int)dist_km) * 10));
				xSemaphoreGive(xSemaphoreMutexWrite);
			}
		}
	}
}

void task_but(void *pvParameters)
{
	char queue;
	_Bool but1_trigger = 0;
	_Bool but2_trigger = 0;
	TC_init(TC0, ID_TC2, 2, 5);
	TC_init(TC1, ID_TC3, 0, 5);
	while (1)
	{
		if (xQueueReceive(xQueueBut, &queue, (TickType_t)500))
		{
			if (queue == '1')
			{
				but1_trigger = !but1_trigger;
				but1_trigger ? tc_start(TC0, 2) : tc_stop(TC0, 2);
				if (but1_trigger == 0)
					pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
			}
			else if (queue == '2')
			{
				but2_trigger = !but2_trigger;
				but2_trigger ? tc_start(TC1, 0) : tc_stop(TC1, 0);
				if (but2_trigger == 0)
					pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
			}
		}
	}
}

// init rtos
void init_rtos(void)
{
	// create semaphores
	xSemaphoreRTC = xSemaphoreCreateBinary();
	xSemaphoreRTCTrack = xSemaphoreCreateBinary();
	xSemaphoreChangeScreen = xSemaphoreCreateBinary();
	xSemaphoreNextUnity = xSemaphoreCreateBinary();
	xSemaphoreMeanVel = xSemaphoreCreateBinary();
	xSemaphoreTimer = xSemaphoreCreateBinary();
	xSemaphoreSensor = xSemaphoreCreateBinary();
	xSemaphoreSensorTrack = xSemaphoreCreateBinary();
	xSemaphorePlay = xSemaphoreCreateBinary();
	xSemaphoreEnd = xSemaphoreCreateBinary();
	xSemaphoreMutex = xSemaphoreCreateMutex();
	xSemaphoreMutexWrite = xSemaphoreCreateMutex();

	// create Queue
	xQueueAroSize = xQueueCreate(100, sizeof(int));
	xQueueAroChange = xQueueCreate(10, sizeof(aroData));
	xQueueNewAro = xQueueCreate(10, sizeof(aroData));
	xQueueNewAro2 = xQueueCreate(10, sizeof(aroData));
	xQueueBut = xQueueCreate(32, sizeof(char));

	// create tasks
	xTaskCreate(task_but, "BUT", TASK_BUT_STACK_SIZE, NULL, TASK_BUT_STACK_PRIORITY, NULL);
	xTaskCreate(task_lcd, "lcd", TASK_LCD_STACK_SIZE, NULL, TASK_LCD_STACK_PRIORITY, NULL);
	xTaskCreate(task_clock, "clock", TASK_LCD_STACK_SIZE, NULL, TASK_LCD_STACK_PRIORITY, NULL);
	xTaskCreate(task_update, "update", TASK_LCD_STACK_SIZE, NULL, TASK_LCD_STACK_PRIORITY, &xTask_update_handle);
	xTaskCreate(task_config, "update_config", TASK_LCD_STACK_SIZE, NULL, TASK_LCD_STACK_PRIORITY, &xTask_update_config_handle);
	xTaskCreate(task_track, "track", TASK_TRACK_STACK_SIZE, NULL, TASK_TRACK_STACK_PRIORITY, &xTask_track_handle);

	vTaskSuspend(xTask_update_config_handle);

	vTaskStartScheduler();
}