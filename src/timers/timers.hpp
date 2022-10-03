#ifndef TIMERS_HEADER
#define TIMERS_HEADER

#include <stdlib.h>
#include <string>
#include <map>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

void initSystick(void);

struct Timers {
	private:
		std::map<std::string, uint32_t> wakeTimes;

	public:
		void setDelayTime(const std::string & timerName, uint32_t delay);
		bool isItTime(const std::string & timerName);
};

#endif