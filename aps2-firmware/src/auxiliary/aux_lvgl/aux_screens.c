/*
 * aux_screens.c
 *
 * Created: 6/6/2022 5:01:55 PM
 *  Author: franc
 */

#include "aux_screens.h"

// globals
int aroSizeValue = 22;
char *aroUnidadeValue = "pol";

void set_aroSize(int size){
	aroSizeValue = size;
}

void set_aroUnidade(char *unidade){
	aroUnidadeValue = unidade;
}

// labels
static lv_obj_t *labelSettings;
static lv_obj_t *labelReset;
static lv_obj_t *labelPlusButton;
static lv_obj_t *labelMinusButton;
static lv_obj_t *labelnextButton1;
static lv_obj_t *labelnextButton2;

LV_IMG_DECLARE(logo);
LV_IMG_DECLARE(logo_label);
LV_IMG_DECLARE(clock);
LV_IMG_DECLARE(path);
LV_IMG_DECLARE(rim);
LV_IMG_DECLARE(speedometer);

LV_FONT_DECLARE(lv_font_montserrat_34);
LV_FONT_DECLARE(lv_font_montserrat_14);

lv_obj_t *START_STOP;
lv_obj_t *getButtonStartStop() { return START_STOP; }

lv_obj_t *labelStartStop;
lv_obj_t *getLabelStartStop() { return labelStartStop; }

lv_obj_t *labelClock;
lv_obj_t *getLabelClock() { return labelClock; }

lv_obj_t *labelClockSec;
lv_obj_t *getLabelClockSec() { return labelClockSec; }

lv_obj_t *aceleracao;
lv_obj_t *getLabelAceleracao() { return aceleracao; }

lv_obj_t *tempoTrajeto;
lv_obj_t *getLabeltempoTrajeto() { return tempoTrajeto; }

lv_obj_t *distanciaTrajeto;
lv_obj_t *getLabelDistanciaTrajeto() { return distanciaTrajeto; }

lv_obj_t *distanciaTrajetoDecimal;
lv_obj_t *getLabelDistanciaTrajetoDecimal() { return distanciaTrajetoDecimal; }

lv_obj_t *velocidadeMediaTrajeto;
lv_obj_t *getLabelvelocidadeMediaTrajeto() { return velocidadeMediaTrajeto; }

lv_obj_t *velocidadeMediaTrajetoDecimal;
lv_obj_t *getLabelvelocidadeMediaTrajetoDecimal() { return velocidadeMediaTrajetoDecimal; }

lv_obj_t *velocidadeInstantanea;
lv_obj_t *getLabelvelocidadeInstantanea() { return velocidadeInstantanea; }

lv_obj_t *velocidadeInstantaneaDecimal;
lv_obj_t *getLabelvelocidadeInstantaneaDecimal() { return velocidadeInstantaneaDecimal; }

lv_obj_t *rectangleRunning;
lv_obj_t *getrectangleRunning() { return rectangleRunning; }

lv_obj_t *aroSize;
lv_obj_t *getLabelAroSize() { return aroSize; }

lv_obj_t *aroUnidade;
lv_obj_t *getLabelAroUnidade() { return aroUnidade; }

void lv_bike(void)
{
	static lv_point_t line_points[] = {{10, 35}, {230, 35}};

	lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_style_bg_color(lv_scr_act(), lv_color_white(), LV_PART_MAIN);

	/*STYLES*/

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_bg_color(&style, lv_color_white());
	lv_style_set_text_color(&style, lv_color_black());
	lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_NONE));
	lv_style_set_shadow_width(&style, 0);
	lv_style_set_border_width(&style, 0);

	static lv_style_t style2;
	lv_style_init(&style2);
	lv_style_set_bg_color(&style2, lv_palette_main(LV_PALETTE_LIGHT_BLUE));
	lv_style_set_text_color(&style2, lv_color_white());
	lv_style_set_shadow_width(&style2, 0);
	lv_style_set_border_width(&style2, 0);
	lv_style_set_radius(&style2, 5);

	static lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, 1);
	lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_GREY));

	/*DRAWING*/

	lv_obj_t *line1;
	line1 = lv_line_create(lv_scr_act());
	lv_line_set_points(line1, line_points, 2); /*Set the points*/
	lv_obj_add_style(line1, &style_line, 0);   /*Set the points*/
	lv_obj_align(line1, LV_ALIGN_TOP_LEFT, 0, 0);

	lv_obj_t *rectangle1 = lv_obj_create(lv_scr_act());
	lv_obj_set_width(rectangle1, 220);
	lv_obj_set_height(rectangle1, 60);
	lv_obj_set_style_border_color(rectangle1, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_set_style_border_width(rectangle1, 1, 0);
	lv_obj_set_style_radius(rectangle1, 5, 0);
	lv_obj_align_to(rectangle1, line1, LV_ALIGN_OUT_BOTTOM_MID, 5, 10);

	lv_obj_t *rectangle2 = lv_obj_create(lv_scr_act());
	lv_obj_set_width(rectangle2, 220);
	lv_obj_set_height(rectangle2, 160);
	lv_obj_set_style_border_color(rectangle2, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_set_style_border_width(rectangle2, 1, 0);
	lv_obj_set_style_radius(rectangle2, 5, 0);
	lv_obj_align_to(rectangle2, rectangle1, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

	rectangleRunning = lv_obj_create(lv_scr_act());
	lv_obj_set_width(rectangleRunning, 220);
	lv_obj_set_height(rectangleRunning, 5);
	lv_obj_set_style_radius(rectangleRunning, 2, 0);
	lv_obj_set_style_border_color(rectangleRunning, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_set_style_border_width(rectangleRunning, 1, 0);
	// lv_obj_set_style_bg_color(rectangleRunning, lv_palette_main(LV_PALETTE_BLUE), 0);
	lv_obj_align_to(rectangleRunning, rectangle2, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

	/*IMAGES*/

	lv_obj_t *logo1 = lv_img_create(lv_scr_act());
	lv_img_set_src(logo1, &logo);
	lv_obj_align(logo1, LV_ALIGN_TOP_RIGHT, -5, 5);

	lv_obj_t *logo2 = lv_img_create(lv_scr_act());
	lv_img_set_src(logo2, &logo_label);
	lv_obj_align(logo2, LV_ALIGN_BOTTOM_LEFT, 5, -5);

	lv_obj_t *clock1 = lv_img_create(lv_scr_act());
	lv_img_set_src(clock1, &clock);
	lv_obj_align_to(clock1, rectangle2, LV_ALIGN_BOTTOM_MID, 0, -35);

	lv_obj_t *speedometer1 = lv_img_create(lv_scr_act());
	lv_img_set_src(speedometer1, &speedometer);
	lv_obj_align_to(speedometer1, rectangle2, LV_ALIGN_TOP_RIGHT, -50, 0);

	lv_obj_t *path1 = lv_img_create(lv_scr_act());
	lv_img_set_src(path1, &path);
	lv_obj_align_to(path1, rectangle2, LV_ALIGN_TOP_LEFT, 0, 0);

	/*LABELS*/
	labelClock = lv_label_create(lv_scr_act());
	lv_obj_align(labelClock, LV_ALIGN_TOP_LEFT, 10, 5);
	lv_obj_set_style_text_color(labelClock, lv_color_black(), LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(labelClock, LV_TEXT_ALIGN_RIGHT, LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelClock, "%02d:%02d", 10, 40);

	labelClockSec = lv_label_create(lv_scr_act());
	lv_obj_align_to(labelClockSec, labelClock, LV_ALIGN_OUT_RIGHT_MID, 2, 9);
	lv_obj_set_style_text_font(labelClockSec, &lv_font_montserrat_14, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(labelClockSec, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelClockSec, "%02d", 50);

	aceleracao = lv_label_create(lv_scr_act());
	lv_obj_align_to(aceleracao, rectangle1, LV_ALIGN_RIGHT_MID, -3, -5);
	lv_obj_set_style_text_color(aceleracao, lv_palette_main(LV_PALETTE_GREY), LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(aceleracao, &lv_font_montserrat_34, 0);
	lv_label_set_text_fmt(aceleracao, LV_SYMBOL_MINUS);

	tempoTrajeto = lv_label_create(lv_scr_act());
	lv_obj_align_to(tempoTrajeto, rectangle2, LV_ALIGN_BOTTOM_MID, -7, 0);
	lv_obj_set_style_text_color(tempoTrajeto, lv_color_black(), LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(tempoTrajeto, LV_TEXT_ALIGN_CENTER, LV_STATE_DEFAULT);
	lv_label_set_text_fmt(tempoTrajeto, "%02d:%02d", 0, 0);

	distanciaTrajeto = lv_label_create(lv_scr_act());
	lv_obj_align_to(distanciaTrajeto, path1, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_obj_set_style_text_color(distanciaTrajeto, lv_color_black(), LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(distanciaTrajeto, LV_TEXT_ALIGN_RIGHT, LV_STATE_DEFAULT);
	lv_label_set_text_fmt(distanciaTrajeto, "%02d", 0);

	distanciaTrajetoDecimal = lv_label_create(lv_scr_act());
	lv_obj_align_to(distanciaTrajetoDecimal, distanciaTrajeto, LV_ALIGN_OUT_RIGHT_MID, 2, 9);
	lv_obj_set_style_text_font(distanciaTrajetoDecimal, &lv_font_montserrat_14, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(distanciaTrajetoDecimal, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(distanciaTrajetoDecimal, ",%01d", 0);

	lv_obj_t *distanciaTrajetoUnidade = lv_label_create(lv_scr_act());
	lv_obj_align_to(distanciaTrajetoUnidade, distanciaTrajeto, LV_ALIGN_OUT_BOTTOM_MID, 15, 0);
	lv_obj_set_style_text_font(distanciaTrajetoUnidade, &lv_font_montserrat_14, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(distanciaTrajetoUnidade, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(distanciaTrajetoUnidade, "km");

	velocidadeMediaTrajeto = lv_label_create(lv_scr_act());
	lv_obj_align_to(velocidadeMediaTrajeto, speedometer1, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
	lv_obj_set_style_text_color(velocidadeMediaTrajeto, lv_color_black(), LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(velocidadeMediaTrajeto, LV_TEXT_ALIGN_RIGHT, LV_STATE_DEFAULT);
	lv_label_set_text_fmt(velocidadeMediaTrajeto, "%02d", 0);

	velocidadeMediaTrajetoDecimal = lv_label_create(lv_scr_act());
	lv_obj_align_to(velocidadeMediaTrajetoDecimal, velocidadeMediaTrajeto, LV_ALIGN_OUT_RIGHT_MID, 2, 9);
	lv_obj_set_style_text_font(velocidadeMediaTrajetoDecimal, &lv_font_montserrat_14, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(velocidadeMediaTrajetoDecimal, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(velocidadeMediaTrajetoDecimal, ",%01d", 0);

	lv_obj_t *velocidadeMediaTrajetoUnidade = lv_label_create(lv_scr_act());
	lv_obj_align_to(velocidadeMediaTrajetoUnidade, velocidadeMediaTrajeto, LV_ALIGN_OUT_BOTTOM_MID, 15, 0);
	lv_obj_set_style_text_font(velocidadeMediaTrajetoUnidade, &lv_font_montserrat_14, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(velocidadeMediaTrajetoUnidade, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(velocidadeMediaTrajetoUnidade, "km/h");

	velocidadeInstantanea = lv_label_create(lv_scr_act());
	lv_obj_align_to(velocidadeInstantanea, rectangle1, LV_ALIGN_LEFT_MID, 12, -7);
	lv_obj_set_style_text_align(velocidadeInstantanea, LV_TEXT_ALIGN_RIGHT, LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(velocidadeInstantanea, &lv_font_montserrat_34, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(velocidadeInstantanea, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(velocidadeInstantanea, "%02d", 0);

	velocidadeInstantaneaDecimal = lv_label_create(lv_scr_act());
	lv_obj_align_to(velocidadeInstantaneaDecimal, velocidadeInstantanea, LV_ALIGN_OUT_RIGHT_MID, 2, -4);
	lv_obj_set_style_text_font(velocidadeInstantaneaDecimal, &lv_font_montserrat_14, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(velocidadeInstantaneaDecimal, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(velocidadeInstantaneaDecimal, ",%01d", 0);

	lv_obj_t *velocidadeInstantaneaUnidade = lv_label_create(lv_scr_act());
	lv_obj_align_to(velocidadeInstantaneaUnidade, velocidadeInstantanea, LV_ALIGN_OUT_RIGHT_MID, 2, 12);
	lv_obj_set_style_text_font(velocidadeInstantaneaUnidade, &lv_font_montserrat_14, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(velocidadeInstantaneaUnidade, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(velocidadeInstantaneaUnidade, "km/h");

	/*BUTTONS*/

	lv_obj_t *settings;
	create_btn(lv_scr_act(), config_handler, &settings, LV_EVENT_CLICKED, 30, 30, &style);
	lv_obj_align(settings, LV_ALIGN_BOTTOM_RIGHT, -5, -5);
	create_label_for_btn(settings, labelSettings, LV_SYMBOL_SETTINGS);

	create_btn(lv_scr_act(), play_handler, &START_STOP, LV_EVENT_CLICKED, 60, 80, &style2);
	lv_obj_align_to(START_STOP, rectangle2, LV_ALIGN_BOTTOM_RIGHT, 10, 10);
	lv_obj_set_style_text_color(START_STOP, lv_color_white(), 0);

	labelStartStop = lv_label_create(START_STOP);
	lv_label_set_text(labelStartStop, LV_SYMBOL_PLAY);
	lv_obj_center(labelStartStop);

	lv_obj_t *RESET;
	create_btn(lv_scr_act(), end_handler, &RESET, LV_EVENT_CLICKED, 60, 80, &style2);
	lv_obj_set_style_bg_color(RESET, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_set_style_border_color(RESET, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_align_to(RESET, rectangle2, LV_ALIGN_BOTTOM_LEFT, -10, 10);
	lv_obj_set_style_text_color(RESET, lv_color_white(), 0);
	create_label_for_btn(RESET, labelReset, LV_SYMBOL_TRASH);
}

void lv_bike_config(void)
{
	static lv_point_t line_points[] = {{10, 35}, {230, 35}};

	lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_style_bg_color(lv_scr_act(), lv_color_white(), LV_PART_MAIN);

	/*STYLES*/

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_bg_color(&style, lv_color_white());
	lv_style_set_text_color(&style, lv_color_black());
	lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_NONE));
	lv_style_set_shadow_width(&style, 0);
	lv_style_set_border_width(&style, 0);

	static lv_style_t style2;
	lv_style_init(&style2);
	lv_style_set_bg_color(&style2, lv_palette_main(LV_PALETTE_LIGHT_BLUE));
	lv_style_set_text_color(&style2, lv_color_white());
	lv_style_set_shadow_width(&style2, 0);
	lv_style_set_border_width(&style2, 0);
	lv_style_set_radius(&style2, 5);

	static lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, 1);
	lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_GREY));

	/*DRAWING*/

	lv_obj_t *line1;
	line1 = lv_line_create(lv_scr_act());
	lv_line_set_points(line1, line_points, 2); /*Set the points*/
	lv_obj_add_style(line1, &style_line, 0);   /*Set the points*/
	lv_obj_align(line1, LV_ALIGN_TOP_LEFT, 0, 0);

	lv_obj_t *rectangle2 = lv_obj_create(lv_scr_act());
	lv_obj_set_width(rectangle2, 220);
	lv_obj_set_height(rectangle2, 200);
	lv_obj_set_style_border_color(rectangle2, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_set_style_border_width(rectangle2, 1, 0);
	lv_obj_set_style_radius(rectangle2, 5, 0);
	lv_obj_align(rectangle2, LV_ALIGN_LEFT_MID, 10, 0);

	/*IMAGES*/

	lv_obj_t *logo1 = lv_img_create(lv_scr_act());
	lv_img_set_src(logo1, &logo);
	lv_obj_align(logo1, LV_ALIGN_TOP_RIGHT, -5, 5);

	lv_obj_t *logo2 = lv_img_create(lv_scr_act());
	lv_img_set_src(logo2, &logo_label);
	lv_obj_align(logo2, LV_ALIGN_BOTTOM_LEFT, 5, -5);

	lv_obj_t *aro1 = lv_img_create(lv_scr_act());
	lv_img_set_src(aro1, &rim);
	lv_obj_align_to(aro1, rectangle2, LV_ALIGN_TOP_MID, 30, -5);

	/*LABELS*/
	labelClock = lv_label_create(lv_scr_act());
	lv_obj_align(labelClock, LV_ALIGN_TOP_LEFT, 10, 5);
	lv_obj_set_style_text_color(labelClock, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelClock, "%02d:%02d", 10, 40);

	labelClockSec = lv_label_create(lv_scr_act());
	lv_obj_align_to(labelClockSec, labelClock, LV_ALIGN_OUT_RIGHT_MID, 2, 9);
	lv_obj_set_style_text_font(labelClockSec, &lv_font_montserrat_14, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(labelClockSec, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelClockSec, "%02d", 50);

	lv_obj_t *labelAro = lv_label_create(lv_scr_act());
	lv_obj_align_to(labelAro, rectangle2, LV_ALIGN_TOP_MID, -25, -2);
	lv_obj_set_style_text_color(labelAro, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(labelAro, "ARO");

	aroSize = lv_label_create(lv_scr_act());
	lv_obj_align_to(aroSize, rectangle2, LV_ALIGN_TOP_MID, 0, 60);
	lv_obj_set_style_text_align(aroSize, LV_TEXT_ALIGN_RIGHT, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(aroSize, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(aroSize, "%d", aroSizeValue);

	aroUnidade = lv_label_create(lv_scr_act());
	lv_obj_align_to(aroUnidade, rectangle2, LV_ALIGN_TOP_MID, 0, 120);
	lv_obj_set_style_text_align(aroUnidade, LV_TEXT_ALIGN_RIGHT, LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(aroUnidade, lv_color_black(), LV_STATE_DEFAULT);
	lv_label_set_text_fmt(aroUnidade, aroUnidadeValue);

	/*BUTTONS*/

	lv_obj_t *settings;
	create_btn(lv_scr_act(), config_handler, &settings, LV_EVENT_CLICKED, 30, 30, &style);
	lv_obj_align(settings, LV_ALIGN_BOTTOM_RIGHT, -5, -5);
	create_label_for_btn(settings, labelSettings, LV_SYMBOL_SAVE);

	lv_obj_t *plusButton;
	create_btn(lv_scr_act(), add_aro_handler, &plusButton, LV_EVENT_CLICKED, 30, 30, &style);
	lv_obj_align_to(plusButton, rectangle2, LV_ALIGN_TOP_RIGHT, -15, 50);
	lv_obj_set_style_text_color(plusButton, lv_color_white(), 0);
	lv_obj_set_style_bg_color(plusButton, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_set_width(plusButton, 45);
	lv_obj_set_height(plusButton, 45);
	create_label_for_btn(plusButton, labelPlusButton, LV_SYMBOL_PLUS);

	lv_obj_t *minusButton;
	create_btn(lv_scr_act(), sub_aro_handler, &minusButton, LV_EVENT_CLICKED, 30, 30, &style);
	lv_obj_align_to(minusButton, rectangle2, LV_ALIGN_TOP_LEFT, 0, 50);
	lv_obj_set_style_text_color(minusButton, lv_color_white(), 0);
	lv_obj_set_style_bg_color(minusButton, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_set_width(minusButton, 45);
	lv_obj_set_height(minusButton, 45);
	create_label_for_btn(minusButton, labelMinusButton, LV_SYMBOL_MINUS);

	lv_obj_t *nextButton1;
	create_btn(lv_scr_act(), next_unidade_handler, &nextButton1, LV_EVENT_CLICKED, 30, 30, &style);
	lv_obj_align_to(nextButton1, rectangle2, LV_ALIGN_TOP_RIGHT, -15, 115);
	lv_obj_set_style_text_color(nextButton1, lv_color_white(), 0);
	lv_obj_set_style_bg_color(nextButton1, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_set_width(nextButton1, 45);
	lv_obj_set_height(nextButton1, 45);
	create_label_for_btn(nextButton1, labelnextButton1, LV_SYMBOL_RIGHT);

	lv_obj_t *nextButton2;
	create_btn(lv_scr_act(), next_unidade_handler, &nextButton2, LV_EVENT_CLICKED, 30, 30, &style);
	lv_obj_align_to(nextButton2, rectangle2, LV_ALIGN_TOP_LEFT, 0, 115);
	lv_obj_set_style_text_color(nextButton2, lv_color_white(), 0);
	lv_obj_set_style_bg_color(nextButton2, lv_palette_main(LV_PALETTE_GREY), 0);
	lv_obj_set_width(nextButton2, 45);
	lv_obj_set_height(nextButton2, 45);
	create_label_for_btn(nextButton2, labelnextButton2, LV_SYMBOL_LEFT);
}