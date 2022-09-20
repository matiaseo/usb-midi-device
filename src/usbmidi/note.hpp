#ifndef USBMIDI_NOTE
#define USBMIDI_NOTE

#include <stdlib.h>
#include <stdint.h>

struct Note {
	uint8_t id;
	uint8_t velocity;

    Note(uint8_t noteNumber, uint8_t velocityValue) :
        id(noteNumber & 0x7f), velocity(velocityValue & 0x7f) {}

    Note operator+ (uint8_t delta) const {
        return Note(static_cast<uint8_t>(id + delta) & 0x7f, velocity);
    }

    Note operator* (float percent) const {
        return Note(id, static_cast<uint8_t>(128.0 * percent) & 0x7f);
    }
};

#endif