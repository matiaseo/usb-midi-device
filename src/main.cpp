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
#include "timer/pollTimer.hpp"

bool button_poll()//(MidiController midiController)
{
	static uint32_t button_state = 0;
	/* This is a simple shift based debounce. It's simplistic because
	 * although this implements debounce adequately it does not have any
	 * noise suppression. It is also very wide (32-bits) because it can
	 * be polled in a very tight loop (no debounce timer).
	 */
	uint32_t old_button_state = button_state;
	button_state = (button_state << 1) | (GPIOA_IDR & 1);
	return ((0 == button_state) != (0 == old_button_state));
}

Note getNote() {
	static uint8_t noteOffset = 0;
	static uint8_t velocityOffset = 0;
	velocityOffset = (velocityOffset+16) % 32;
	noteOffset = (noteOffset+5 + (velocityOffset? 0 : 2)) % (128-36);
	return Note { 24, 0 } * ((32+velocityOffset)/128.0) + noteOffset;
}

Note calculateNote(uint16_t inputValue, uint16_t inputVelocity) {
	return Note { static_cast<uint8_t>((inputValue>>5)&127), inputVelocity & 127 };
}

int main(void)
{
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
	usbd_device *usbd_dev = initUsbDevice();
	MidiController midiController {usbd_dev};

	// initInputButton();
	initInputADC();
	initSystick();

	Note currentNote = getNote();
	Note targetNote{0,0};
	setDelayTime(1000);
	uint32_t bpm;

	while (1) {
		usbd_poll(usbd_dev);
		if(isTimeToPoll()) {
			targetNote = calculateNote(readAnalogChannel(0), 100);
			bpm = (readAnalogChannel(1) >> 2)+32;
			setDelayTime(bpm);
			// if(targetNote != currentNote) {
				midiController.sendNoteEvent(currentNote, false);
				midiController.sendNoteEvent(currentNote+3, false);
				midiController.sendNoteEvent(currentNote+7, false);
				currentNote = targetNote;
				midiController.sendNoteEvent(currentNote, true);
				midiController.sendNoteEvent(currentNote+3, true);
				midiController.sendNoteEvent(currentNote+7, true);
			// }
		}
	}
}