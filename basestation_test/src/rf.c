/*
 * rf.c
 *
 * Created: 2011-05-13 16:30:22
 *  Author: niklo
 */ 

#include <asf.h>
#include "rf.h"

int8_t rf_init(void)
{
	// set rf circuit in reasonable mode eg. sleep mode...
	gpio_set_pin_high(nRF24AP2_SLEEP);
	gpio_set_pin_high(nRF24AP2_nSUSPEND);
	
	return STATUS_OK;
}

int8_t rf_send_byte(uint8_t data)
{
	USART_t *usart	= ANT_USART;
	
	// wait for empty data register...
	while (usart_data_register_is_empty(usart) == false)
	{
	}
	
	// wait for rts to be deasserted...
	while (gpio_pin_is_high(nRF24AP2_RTS))
	{
	}
		
	(usart)->DATA = data;
	
	return 0;
}

int8_t rf_send(ANTHDR* hdr, uint8_t* data)
{
	uint8_t *p		= data;
	uint8_t chksum	= 0;
	
	// put data...
	rf_send_byte(hdr->sync);
	rf_send_byte(hdr->id);
	rf_send_byte(hdr->len);
	
	chksum = (hdr->sync)^(hdr->id)^(hdr->len);
		
	for (int i=0;i<hdr->len;i++)
	{
		rf_send_byte(*p);
		chksum ^= *p;
		p++;
	}

	rf_send_byte(chksum);
	
	return STATUS_OK;
}

int8_t rf_receive_byte(uint8_t* data)
{
	USART_t *usart	= ANT_USART;
	
	// wait for empty data register...
	while (usart_rx_is_complete(usart) == false)
	{
	}
	
	*data = (usart)->DATA;

	return STATUS_OK;	
}

int8_t rf_receive(ANTHDR* hdr, uint8_t* data)
{
	uint8_t *p		= data;
	uint8_t buflen	= hdr->len;
	uint8_t chksum1	= 0;
	uint8_t chksum2	= 0;
	
	// get data...
	rf_receive_byte(&(hdr->sync));
	rf_receive_byte(&(hdr->id));
	rf_receive_byte(&(hdr->len));
	
	chksum1 = (hdr->sync)^(hdr->id)^(hdr->len);
	
	for (int i=0;i<hdr->len;i++)
	{
		rf_receive_byte(p);
		chksum1 ^= *p;
		p++;
	}
	
	rf_receive_byte(chksum2);
	
	if (chksum1!=chksum2)
	{
		return -1;	
	}		
	
	return STATUS_OK;
}

int8_t rf_reset(uint8_t state)
{
	if (state)
	{
		gpio_set_pin_low(nRF24AP2_nRESET);			
	}
	else
	{
		gpio_set_pin_high(nRF24AP2_nRESET);	
	}
	
	return STATUS_OK;
}

int8_t rf_sleep(uint8_t state)
{
	if (state)
	{
		gpio_set_pin_high(nRF24AP2_SLEEP);			
	}
	else
	{
		gpio_set_pin_low(nRF24AP2_SLEEP);	
	}
	
	return STATUS_OK;
}

int8_t rf_suspend(uint8_t state)
{
	if (state)
	{
		gpio_set_pin_high(nRF24AP2_SLEEP);
		gpio_set_pin_low(nRF24AP2_nSUSPEND);
		
		gpio_set_pin_high(nRF24AP2_nSUSPEND);
	}
	else
	{
		gpio_set_pin_low(nRF24AP2_SLEEP);
		gpio_set_pin_high(nRF24AP2_nSUSPEND);
	}
	
	return STATUS_OK;
}