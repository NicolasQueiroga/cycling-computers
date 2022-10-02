#include "aux_hw.h"
#include "auxiliary/aux_lvgl/aux_lvgl.h"
#include "auxiliary/aux_rtos/aux_rtos.h"
#include "auxiliary/aux_time/aux_time.h"

// callbacks
void sensor_callback(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(get_xSemaphoreSensor(), &xHigherPriorityTaskWoken);
    xSemaphoreGiveFromISR(get_xSemaphoreSensorTrack(), &xHigherPriorityTaskWoken);
}

void but1_callback(void)
{
    char id = '1';
    xQueueSendFromISR(get_but_queue(), &id, 0);
}

void but2_callback(void)
{
    char id = '2';
    xQueueSendFromISR(get_but_queue(), &id, 0);
}

// aux functions
void configure_input(Pio *p_pio, uint32_t ul_id, const uint32_t ul_mask, const uint32_t ul_attribute,
                     const uint32_t ul_cut_off, uint32_t ul_attr, void (*p_handler)(uint32_t, uint32_t), int i_priority)
{
    pio_configure(p_pio, PIO_INPUT, ul_mask, ul_attribute);
    pio_set_debounce_filter(p_pio, ul_mask, ul_cut_off);
    pio_handler_set(p_pio,
                    ul_id,
                    ul_mask,
                    ul_attr,
                    p_handler);
    pio_enable_interrupt(p_pio, ul_mask);
    pio_get_interrupt_status(p_pio);
    NVIC_EnableIRQ(ul_id);
    NVIC_SetPriority(ul_id, i_priority);
}

void pin_toggle(Pio *pio, uint32_t mask) { pio_get_output_data_status(pio, mask) ? pio_clear(pio, mask) : pio_set(pio, mask); }

// init basic
void init(void)
{
    // setup
    board_init();
    sysclk_init();
    configure_lcd();
    configure_touch();
    configure_lvgl();

    // init clock
    pmc_enable_periph_clk(ID_PIOA);
    pmc_enable_periph_clk(ID_PIOB);
    pmc_enable_periph_clk(ID_PIOC);
    pmc_enable_periph_clk(ID_PIOD);

    // inputs
    configure_input(BUT1_PIO, BUT1_PIO_ID, BUT1_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE, 60, PIO_IT_FALL_EDGE, but1_callback, 4);
    configure_input(BUT2_PIO, BUT2_PIO_ID, BUT2_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE, 60, PIO_IT_FALL_EDGE, but2_callback, 4);
    configure_input(SENSOR_PIO, SENSOR_PIO_ID, SENSOR_PIO_IDX_MASK, PIO_PULLUP, 60, PIO_IT_FALL_EDGE, sensor_callback, 5);

    // led's
    pio_configure(LED1_PIO, PIO_OUTPUT_0, LED1_PIO_IDX_MASK, PIO_DEFAULT);
    pio_configure(LED2_PIO, PIO_OUTPUT_0, LED2_PIO_IDX_MASK, PIO_DEFAULT);
	
	pio_set(LED1_PIO, LED1_PIO_IDX_MASK);
	pio_set(LED2_PIO, LED2_PIO_IDX_MASK);
}

// setup
void configure_lcd(void)
{
    pio_configure_pin(LCD_SPI_MISO_PIO, LCD_SPI_MISO_FLAGS);
    pio_configure_pin(LCD_SPI_MOSI_PIO, LCD_SPI_MOSI_FLAGS);
    pio_configure_pin(LCD_SPI_SPCK_PIO, LCD_SPI_SPCK_FLAGS);
    pio_configure_pin(LCD_SPI_NPCS_PIO, LCD_SPI_NPCS_FLAGS);
    pio_configure_pin(LCD_SPI_RESET_PIO, LCD_SPI_RESET_FLAGS);
    pio_configure_pin(LCD_SPI_CDS_PIO, LCD_SPI_CDS_FLAGS);

    ili9341_init();
    ili9341_backlight_on();
}
