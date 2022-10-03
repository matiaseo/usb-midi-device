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

    Note operator* (uint8_t velocity) const {
        return Note(id, velocity & 0x7f);
    }

    bool operator== (const Note& note) const {
        return id == note.id && velocity == note.velocity;
    }

    bool operator!= (const Note& note) const {
        return id != note.id && velocity != note.velocity;
    }
};

#endif