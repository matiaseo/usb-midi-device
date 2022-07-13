#ifndef USBMIDI_MIDI
#define USBMIDI_MIDI

#include <stdlib.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/desig.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "usbmidi/usb.hpp"

void button_send_event(usbd_device *usbd_dev, int pressed);

#endif