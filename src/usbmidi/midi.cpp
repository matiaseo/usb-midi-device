#include "midi.hpp"

void button_send_event(usbd_device *usbd_dev, int pressed)
{
	char buf[4] = { 0x08, /* USB framing: virtual cable 0, note on */
			0x80, /* MIDI command: note on, channel 1 */
			60,   /* Note 60 (middle C) */
			64,   /* "Normal" velocity */
	};

	buf[0] |= pressed;
	buf[1] |= pressed << 4;

	while (usbd_ep_write_packet(usbd_dev, 0x81, buf, sizeof(buf)) == 0);
}
