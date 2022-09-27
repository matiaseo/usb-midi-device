#ifndef POLL_TIMER_HEADER
#define POLL_TIMER_HEADER

#include <stdlib.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

void setDelayTime(uint32_t delay);
bool isTimeToPoll();
void initSystick(void);
void sys_tick_handler(void);

#endif