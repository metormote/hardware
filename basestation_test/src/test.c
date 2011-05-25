/*
 * test.c
 *
 * Created: 2011-05-13 10:57:32
 *  Author: niklo
 */ 

#include <asf.h>
#include "test.h"
#include "debug.h"
#include "rf.h"

void test_main()
{
	test_debug_usart();
	test_switch_and_leds();
//	test_rf();
//	test_gprs();
//	test_flash();
//	test_rtc();
}

// include at top of every test function...
void test_prologue(int8_t* testcase)
{
	debug_print("Test case: ");
	debug_print(testcase);
	debug_print("Press push button to start...");
	debug_wait_for_step();
}


// simple test routine(s) to see functionality of:
// - switch
// - leds
void test_switch_and_leds()
{
	test_prologue("Switch and LEDs...\n");

	// init
	gpio_set_pin_high(LED0_GPIO);
	gpio_set_pin_high(LED1_GPIO);
	
	debug_print("Step 1. LED0");
	debug_wait_for_step();
	gpio_set_pin_low(LED0_GPIO);
	
	debug_print("Step 2. LED1");
	debug_wait_for_step();
	gpio_set_pin_low(LED1_GPIO);
	
	debug_print("Finished");
	debug_wait_for_step();
	
	// exit
	gpio_set_pin_high(LED0_GPIO);
	gpio_set_pin_high(LED1_GPIO);
}

void test_debug_usart()
{
	debug_print("test_debug():\n");
	debug_print("this is a test...\n");
	
	usart_putchar(&USARTE0, 'h');
	usart_putchar(&USARTE0, 'e');
	usart_putchar(&USARTE0, 'l');
	usart_putchar(&USARTE0, 'l');
	usart_putchar(&USARTE0, 'o');
}


// - rf (usart)
void test_rf()
{
	// TODO: test usart communication e.g. read status registry
	// NOTE: in order to send data the rts signal has to be obeyed
	// TODO: test SLEEP and nSUSPEND
	
}
	
// - gprs (usart)
void test_gprs()
{
	// TODO: read status registry or equivalent...
}

// - flash
void test_flash()
{
	// TODO: read status registry or equivalent...
}

// - rtc
void test_rtc()
{
	// TODO: read status registry or equivalent...
}

