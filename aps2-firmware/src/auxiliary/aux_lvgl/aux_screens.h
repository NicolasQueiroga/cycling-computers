/*
 * aux_screens.h
 *
 * Created: 6/6/2022 5:02:41 PM
 *  Author: franc
 */

#ifndef AUX_SCREENS_H_
#define AUX_SCREENS_H_

#include "aux_lvgl.h"

// getters
lv_obj_t *getLabelClock(void);
lv_obj_t *getLabelClockSec(void);
lv_obj_t *getLabelAceleracao(void);
lv_obj_t *getLabeltempoTrajeto(void);
lv_obj_t *getLabelDistanciaTrajeto(void);
lv_obj_t *getLabelDistanciaTrajetoDecimal(void);
lv_obj_t *getLabelvelocidadeMediaTrajeto(void);
lv_obj_t *getLabelvelocidadeMediaTrajetoDecimal(void);
lv_obj_t *getLabelvelocidadeInstantanea(void);
lv_obj_t *getLabelvelocidadeInstantaneaDecimal(void);
lv_obj_t *getrectangleRunning(void);
lv_obj_t *getLabelAroSize(void);
lv_obj_t *getLabelAroUnidade(void);
lv_obj_t *getButtonStartStop(void);
lv_obj_t *getLabelStartStop(void);

//setters
void set_aroSize(int size);

void set_aroUnidade(char *unidade);

// screen
void lv_bike(void);
void lv_bike_config(void);

#endif /* AUX_SCREENS_H_ */