#include "input.hpp"

void initInputADC() {
    rcc_periph_clock_enable(RCC_ADC1); //enable ADC1 clock
    gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO0); // Set pin mode
	gpio_mode_setup(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO1);

    adc_power_off(ADC1);

	adc_disable_scan_mode(ADC1);
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_480CYC);

	adc_power_on(ADC1);
}

uint16_t readAnalogChannel(uint8_t channel) {
    uint8_t channels[16] = { channel };
	adc_set_regular_sequence(ADC1, 1, channels);
	adc_start_conversion_regular(ADC1);
	while (!adc_eoc(ADC1));
	uint16_t firstMeasurement = adc_read_regular(ADC1);
	adc_start_conversion_regular(ADC1);
	while (!adc_eoc(ADC1));
	uint16_t secondMeasurement = adc_read_regular(ADC1);
	return (firstMeasurement + secondMeasurement + 1) >> 1; // Average and round
}