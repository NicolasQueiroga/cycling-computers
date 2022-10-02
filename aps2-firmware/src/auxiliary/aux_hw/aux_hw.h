#ifndef __AUX_HW_H_
#define __AUX_HW_H_

#include "conf_board.h"
#include "asf.h"

#define SENSOR_PIO PIOB
#define SENSOR_PIO_ID ID_PIOB
#define SENSOR_PIO_IDX 3
#define SENSOR_PIO_IDX_MASK (1u << SENSOR_PIO_IDX)

#define LED1_PIO PIOD
#define LED1_PIO_ID ID_PIOD
#define LED1_PIO_IDX 28
#define LED1_PIO_IDX_MASK (1u << LED1_PIO_IDX)

#define LED2_PIO PIOC
#define LED2_PIO_ID ID_PIOC
#define LED2_PIO_IDX 17
#define LED2_PIO_IDX_MASK (1u << LED2_PIO_IDX)

#define BUT1_PIO PIOA
#define BUT1_PIO_ID ID_PIOA
#define BUT1_PIO_IDX 0
#define BUT1_PIO_IDX_MASK (1u << BUT1_PIO_IDX)

#define BUT2_PIO PIOC
#define BUT2_PIO_ID ID_PIOC
#define BUT2_PIO_IDX 30
#define BUT2_PIO_IDX_MASK (1u << BUT2_PIO_IDX)

// aux functions
void configure_input(Pio *p_pio, uint32_t ul_id, const uint32_t ul_mask, const uint32_t ul_attribute,
                     const uint32_t ul_cut_off, uint32_t ul_attr, void (*p_handler)(uint32_t, uint32_t), int i_priority);
void pin_toggle(Pio *pio, uint32_t mask);

// callbacks
void sensor_callback(void);
void but1_callback(void);
void but2_callback(void);

// init basic
void init(void);

// setup
void configure_lcd(void);

#endif