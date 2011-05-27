/*
 * gprs.c
 *
 * Created: 2011-05-13 16:31:00
 *  Author: niklo
 */ 

#include <asf.h>
#include "gprs.h"

enum status_code gprs_send(uint8_t* data, uint8_t size)
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

enum status_code gprs_receive(uint8_t* data, uint8_t size)
{
	USART_t *usart	= GPRS_USART;
	uint8_t *p		= data;
	
	// get data...
	for (int i=0;i<size;i++)
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
