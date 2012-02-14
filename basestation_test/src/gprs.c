/*
 * gprs.c
 *
 * Created: 2011-05-13 16:31:00
 *  Author: niklo
 */ 

#include <asf.h>
#include <util/delay.h>
#include "gprs.h"


int8_t gprs_init(void)
{
	gpio_set_pin_low(GPRS_ON_OFF);
	gpio_set_pin_low(GPRS_RESET);
	
	return STATUS_OK;
}

int8_t gprs_send(uint8_t* data, uint8_t size)
{
	USART_t *usart	= GPRS_USART;
	uint8_t *p		= data;

	
	// put data...
	for (int i=0;i<size;i++)
	{
		// wait for empty data register...
		while (usart_data_register_is_empty(usart) == false)
		{
		}
	
		
		(usart)->DATA = *p;
		p++;
	}

	
	return STATUS_OK;
}

int8_t gprs_receive(uint8_t* data, uint8_t size)
{
	USART_t *usart	= GPRS_USART;
	uint8_t *p		= data;
	
	// BUG: should end on received CR...
	// get data...
	for (int i=0; i<size; i++)
	{
		// wait for empty data register...
		while (usart_rx_is_complete(usart) == false)
		{
		}
	
		*p = (usart)->DATA;
		p++;
	}
	
	return STATUS_OK;
}


int8_t gprs_reset()
{
	// TODO: implement...
	gpio_set_pin_high(GPRS_RESET);
	_delay_ms(200); // NOTE_ F_CLK needs to be set properly!!!
	gpio_set_pin_low(GPRS_RESET);
	return STATUS_OK;
}

int8_t gprs_on(void)
{
	gpio_set_pin_high(GPRS_ON_OFF);
	_delay_ms(100); // NOTE_ F_CLK needs to be set properly!!!
	gpio_set_pin_low(GPRS_ON_OFF);
	
	return STATUS_OK;
}

int8_t gprs_off(void)
{
	gpio_set_pin_high(GPRS_ON_OFF);
	_delay_ms(4000); // NOTE_ F_CLK needs to be set properly!!!
	gpio_set_pin_low(GPRS_ON_OFF);
	
	return STATUS_OK;
}