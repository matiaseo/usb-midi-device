#ifndef USBMIDI_MIDI
#define USBMIDI_MIDI

#include <stdlib.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/desig.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "usbmidi/usb.hpp"
#include "usbmidi/note.hpp"

using namespace std;

void button_send_event(usbd_device *usbd_dev, int pressed);

class MidiController {
private:
    usbd_device * usbDevice;
    void sendNoteData(char * noteData) const {
        while(usbd_ep_write_packet(usbDevice, 0x81, noteData, 4) == 0);
    }
public:
    MidiController(usbd_device * device) : usbDevice(device) {}

    void sendNoteEvent(Note note, bool value = 1) const {
        char buf[4] = { 0x08, /* USB framing: virtual cable 0, note on */
                0x80, /* MIDI command: note on, channel 1 */
                note.id,
                note.velocity
        };

        buf[0] |= value;
        buf[1] |= value << 4;

        sendNoteData(buf);
    }
};

#endif