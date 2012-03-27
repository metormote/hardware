/*
 * rf.c
 *
 * Created: 2011-05-13 16:30:22
 *  Author: niklo
 */ 

#include <asf.h>
#include <util/delay.h>
#include "rf.h"

void rf_init(void)
{	
	//
	rf_reset(false);
	rf_sleep(false);
	rf_suspend(false);
}

int8_t rf_send(uint8_t* data, uint8_t size)
{
	USART_t *usart	= ANT_USART;
	uint8_t *p		= data;
  
	// put data...
	for (int i=0;i<size;i++)
	{
		// wait for empty data register...
		while (usart_data_register_is_empty(usart) == false)
		{}
	
		// wait for rts to be deasserted...
		while (gpio_pin_is_high(ANT_RTS))
		{}
		
		(usart)->DATA = *p;
		p++;
	}

	
	return STATUS_OK;
}

int8_t rf_receive(uint8_t* data, uint8_t size)
{
	USART_t *usart	= ANT_USART;
	uint8_t *p		= data;
	
	// get data...
	for (int i=0;i<size;i++)
	{
		// wait for empty data register...
		while (usart_rx_is_complete(usart) == false)
		{}
		
		*p = (usart)->DATA;
		p++;
	}
	*p=0x0;
	return STATUS_OK;
}


int8_t rf_reset(uint8_t state)
{
	if (state)
	{
		gpio_set_pin_low(ANT_nRESET);	
	}
	else
	{
		gpio_set_pin_high(ANT_nRESET);
	}
	
	return STATUS_OK;
}

int8_t rf_sleep(uint8_t state)
{
	if (state)
	{
		gpio_set_pin_high(ANT_SLEEP);	
	}
	else
	{
		gpio_set_pin_low(ANT_SLEEP);
	}
	
	return STATUS_OK;
}


int8_t rf_suspend(uint8_t state)
{
	if (state)
	{
		gpio_set_pin_high(ANT_SLEEP);
		gpio_set_pin_low(ANT_nSUSPEND);
		_delay_us(100);
		gpio_set_pin_high(ANT_nSUSPEND);		
	}
	else
	{
		gpio_set_pin_high(ANT_nSUSPEND);	// note: needed for init...
		gpio_set_pin_low(ANT_SLEEP);
	}
	return STATUS_OK;
}