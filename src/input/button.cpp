#include "input.hpp"

void initInputButton() {
	/* Button pin */
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
}