#include "aux_time.h"
#include "auxiliary/aux_rtos/aux_rtos.h"
#include "auxiliary/aux_lvgl/aux_screens.h"

// global variables

// inits
void RTC_init(Rtc *rtc, uint32_t id_rtc, Calendar t, uint32_t irq_type)
{
    // PMC configure
    pmc_enable_periph_clk(ID_RTC);

    // default RTC configuration -> 24-hour mode
    rtc_set_hour_mode(rtc, 0);

    // set date and time manually
    rtc_set_date(rtc, t.year, t.month, t.day, t.week);
    rtc_set_time(rtc, t.hour, t.minute, t.second);

    // configure RTC interrupts
    NVIC_DisableIRQ(id_rtc);
    NVIC_ClearPendingIRQ(id_rtc);
    NVIC_SetPriority(id_rtc, 4);
    NVIC_EnableIRQ(id_rtc);

    // activates interruption via alarm
    rtc_enable_interrupt(rtc, irq_type);
}

void TC_init(Tc *TC, int ID_TC, int TC_CHANNEL, int freq)
{
    uint32_t ul_div;
    uint32_t ul_tcclks;
    uint32_t ul_sysclk = sysclk_get_cpu_hz();

    /* Configura o PMC */
    pmc_enable_periph_clk(ID_TC);

    /** Configura o TC para operar em  freq hz e interrup�c�o no RC compare */
    tc_find_mck_divisor(freq, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
    tc_init(TC, TC_CHANNEL, ul_tcclks | TC_CMR_CPCTRG);
    tc_write_rc(TC, TC_CHANNEL, (ul_sysclk / ul_div) / freq);

    /* Configura NVIC*/
    NVIC_SetPriority(ID_TC, 4);
    NVIC_EnableIRQ((IRQn_Type)ID_TC);
    tc_enable_interrupt(TC, TC_CHANNEL, TC_IER_CPCS);
}

void RTT_init(float freqPrescale, uint32_t IrqNPulses, uint32_t rttIRQSource)
{

    uint16_t pllPreScale = (int)(((float)32768) / freqPrescale);

    rtt_sel_source(RTT, false);
    rtt_init(RTT, pllPreScale);

    if (rttIRQSource & RTT_MR_ALMIEN)
    {
        uint32_t ul_previous_time;
        ul_previous_time = rtt_read_timer_value(RTT);
        while (ul_previous_time == rtt_read_timer_value(RTT))
            ;
        rtt_write_alarm_time(RTT, IrqNPulses + ul_previous_time);
    }

    /* config NVIC */
    NVIC_DisableIRQ(RTT_IRQn);
    NVIC_ClearPendingIRQ(RTT_IRQn);
    NVIC_SetPriority(RTT_IRQn, 4);
    NVIC_EnableIRQ(RTT_IRQn);

    /* Enable RTT interrupt */
    if (rttIRQSource & (RTT_MR_RTTINCIEN | RTT_MR_ALMIEN))
        rtt_enable_interrupt(RTT, rttIRQSource);
    else
        rtt_disable_interrupt(RTT, RTT_MR_RTTINCIEN | RTT_MR_ALMIEN);
}

// handlers
void RTC_Handler(void)
{
    uint32_t ul_status = rtc_get_status(RTC);

    if ((ul_status & RTC_SR_SEC) == RTC_SR_SEC)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(get_xSemaphoreRTC(), &xHigherPriorityTaskWoken);
        xSemaphoreGiveFromISR(get_xSemaphoreRTCTrack(), &xHigherPriorityTaskWoken);
    }

    if ((ul_status & RTC_SR_ALARM) == RTC_SR_ALARM)
    {
    }

    rtc_clear_status(RTC, RTC_SCCR_SECCLR);
    rtc_clear_status(RTC, RTC_SCCR_ALRCLR);
    rtc_clear_status(RTC, RTC_SCCR_ACKCLR);
    rtc_clear_status(RTC, RTC_SCCR_TIMCLR);
    rtc_clear_status(RTC, RTC_SCCR_CALCLR);
    rtc_clear_status(RTC, RTC_SCCR_TDERRCLR);
}

void TC0_Handler(void)
{
    volatile uint32_t status = tc_get_status(TC0, 0);

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(get_xSemaphoreTimer(), &xHigherPriorityTaskWoken);
}

void TC1_Handler(void)
{
    volatile uint32_t status = tc_get_status(TC0, 1);

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(get_xSemaphoreInstVel(), &xHigherPriorityTaskWoken);
}

void TC2_Handler(void)
{
    volatile uint32_t status = tc_get_status(TC0, 2);
    pin_toggle(LED1_PIO, LED1_PIO_IDX_MASK);
}

void TC3_Handler(void)
{
    volatile uint32_t status = tc_get_status(TC1, 0);
    pin_toggle(LED2_PIO, LED2_PIO_IDX_MASK);
}

void RTT_Handler(void)
{
    uint32_t ul_status;

    /* Get RTT status - ACK */
    ul_status = rtt_get_status(RTT);

    /* IRQ due to Time has changed */
    if ((ul_status & RTT_SR_RTTINC) == RTT_SR_RTTINC)
    {
    }
}
