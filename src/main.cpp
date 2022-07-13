/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2014 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "usbmidi/usb.hpp"
#include "usbmidi/midi.hpp"
#include "input/input.hpp"

void button_poll(usbd_device *usbd_dev)
{
	static uint32_t button_state = 0;
	/* This is a simple shift based debounce. It's simplistic because
	 * although this implements debounce adequately it does not have any
	 * noise suppression. It is also very wide (32-bits) because it can
	 * be polled in a very tight loop (no debounce timer).
	 */
	uint32_t old_button_state = button_state;
	button_state = (button_state << 1) | (GPIOA_IDR & 1);
	if ((0 == button_state) != (0 == old_button_state)) {
		button_send_event(usbd_dev, !!button_state);
	}
}

int main(void)
{
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

	usbd_device *usbd_dev = initUsbDevice();

	initInput();

	while (1) {
		usbd_poll(usbd_dev);
		button_poll(usbd_dev);
	}
}