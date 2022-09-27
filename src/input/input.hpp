#ifndef INPUT_HEADER
#define INPUT_HEADER

#include <stdlib.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/desig.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

// #include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/adc.h>

void initInputButton();
void initInputADC();
uint16_t readAnalogChannel(uint8_t channel);

#endif
