#include "pollTimer.hpp"

volatile uint32_t systemTimerCount = 0;
volatile uint32_t wakeTime = 0;

void setDelayTime(uint32_t delay) {
    wakeTime = systemTimerCount + delay;
}

bool isTimeToPoll() {
    return systemTimerCount >= wakeTime;
}

void sys_tick_handler(void) {
    systemTimerCount++;
}

void initSystick(void) {
	/* clock rate / 1000 to get 1mS interrupt rate */
	systick_set_reload(168000);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_counter_enable();
	/* this done last */
	systick_interrupt_enable();
}