#include "timers.hpp"

volatile uint32_t systemTimerCount = 0;

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

void Timers::setDelayTime(const std::string & timerName, uint32_t delay) {
	wakeTimes[timerName] = systemTimerCount + delay;
}

bool Timers::isItTime(const std::string & timerName) {
	return systemTimerCount >= wakeTimes[timerName];
}