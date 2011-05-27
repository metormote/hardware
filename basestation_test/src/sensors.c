/*
 * sensors.c
 *
 * Created: 2011-05-27 09:55:47
 *  Author: niklo
 */ 

#include <asf.h>
#include "sensors.h"

int8_t sensors_init(void)
{
	// TODO: 
	// - configure ADC
	// - enable ADC

/*	
	adc_config config;
	config.
	adc_write_configuration(ADCA, &config)
	adc_enable(ADCA);
*/	
	return STATUS_OK;		
}

int8_t sensors_read_vbat(float* value)
{
	return STATUS_OK;		
}

int8_t sensors_read_vusb(float* value)
{
	return STATUS_OK;		
}

int8_t sensors_read_temp(float* value)
{
	return STATUS_OK;		
}

