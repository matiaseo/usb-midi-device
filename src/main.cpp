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
#include "timers/timers.hpp"

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

Note calculateNote(uint16_t inputValue, uint16_t inputVelocity) {
	return Note { static_cast<uint8_t>((inputValue>>5)&127), inputVelocity & 127 };
}

int main(void)
{
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
	usbd_device *usbd_dev = initUsbDevice();
	MidiController midiController {usbd_dev};

	Timers timers;

	// initInputButton();
	initInputADC();
	initSystick();

	// Note currentNote{100,50};
	Note targetNote{100,50};
	Note kick{40,0};
	Note snare{60,0};

	timers.setDelayTime("startUpDelay", 1000);
	while(!timers.isItTime("startUpDelay")) usbd_poll(usbd_dev);

	uint32_t bpm = 3000;

	while (1) {
		usbd_poll(usbd_dev);
		if(timers.isItTime("PollADC")) {
			timers.setDelayTime("PollADC", 1);
			// bpm = (readAnalogChannel(0) >> 2)+100;
			// targetNote = calculateNote(readAnalogChannel(1), 100);
			uint16_t kickValue = (readAnalogChannel(0)>>5)&127 ;
			uint16_t snareValue = (readAnalogChannel(1)>>5)&127;
			kick = Note{ 40 , kick.velocity > kickValue? kick.velocity : kickValue };
			snare = Note{ 60 , snare.velocity > snareValue? snare.velocity : snareValue };
		}
		// if(timers.isItTime("sendNote")){
		// 	timers.setDelayTime("sendNote", bpm);
		// 	midiController.sendNoteEvent(currentNote, false);
		// 	midiController.sendNoteEvent(currentNote+3, false);
		// 	midiController.sendNoteEvent(currentNote+7, false);
		// 	currentNote = targetNote;
		// 	midiController.sendNoteEvent(currentNote, true);
		// 	midiController.sendNoteEvent(currentNote+3, true);
		// 	midiController.sendNoteEvent(currentNote+7, true);
		// }
		// if(timers.isItTime("sendNote")){
		// 	timers.setDelayTime("sendNote", bpm);
		// 	midiController.sendNoteEvent(currentNote, false);
		// 	midiController.sendNoteEvent(kick, false);
		// 	midiController.sendNoteEvent(snare, false);
		// 	currentNote = targetNote;
		// 	midiController.sendNoteEvent(currentNote, true);
		// 	midiController.sendNoteEvent(kick, true);
		// 	midiController.sendNoteEvent(snare, true);
		// 	// kick = kick * 0;
		// 	// snare = snare * 0;
		// }
		if(timers.isItTime("sendKick") && kick.velocity > 70) {
			kick = kick * ((kick.velocity - 64) << 3);
			midiController.sendNoteEvent(kick, true);
			timers.setDelayTime("sendKick", 10);
			kick = kick * 0;
			midiController.sendNoteEvent(kick, false);
		}
		if(timers.isItTime("sendSnare") && snare.velocity > 5) {
			snare = snare * ((snare.velocity - 5) << 5);
			midiController.sendNoteEvent(snare, true);
			timers.setDelayTime("sendSnare", 10);
			snare = snare * 0;
			midiController.sendNoteEvent(snare, false);
		}
	}
}