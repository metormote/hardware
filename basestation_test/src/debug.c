/*
 * debug.c
 *
 * Created: 2011-05-13 10:54:10
 *  Author: niklo
 */ 

#include <asf.h>
#include "gpio.h"
#include "debug.h"

// debug print routine...
void debug_print(int8_t* c)
{
	uint8_t* p=c;
	
	while (*p!=0)
	{
		usart_putchar(&USARTE0, *p);
		p++;
	}	
	
}

// wait for push button...
void debug_wait_for_step()
{
	debug_print("Press push button to continue");
	while (gpio_pin_is_high(GPIO_PUSH_BUTTON_0))
	{}
}

// LED
void debug_red_led(uint8_t state)
{
	if (state)
	{
		gpio_set_pin_low(LED1_GPIO);
	} 
	else
	{
		gpio_set_pin_high(LED1_GPIO);
	}
	
}

void debug_green_led(uint8_t state)
{
	if (state)
	{
		gpio_set_pin_low(LED0_GPIO);
	} 
	else
	{
		gpio_set_pin_high(LED0_GPIO);
	}
	
}