/*
 * test.c
 *
 * Created: 2011-05-13 10:57:32
 *  Author: niklo
 */ 

#include <asf.h>
#include <util/delay.h>
#include "test.h"
#include "debug.h"
#include "rf.h"
#include "gprs.h"

void test_main()
{
//	test_debug_usart();
//	test_switch_and_leds();
//	test_rf();
	test_gprs();
//	test_flash();
//	test_rtc();
}

// include at top of every test function...
void test_prologue(char* testcase)
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
	int i;
	uint8_t msg[16];
	
	// TODO: test usart communication e.g. read status registry
	// NOTE: in order to send data the rts signal has to be obeyed
	// TODO: test SLEEP and nSUSPEND
  
	rf_reset(true);
	rf_reset(false);
	rf_sleep(true);
	rf_sleep(false);
	rf_suspend(true);
	rf_suspend(false);
  
	// reset ant
//	msg[0] = 0x4A;	// sync byte
	msg[0] = 0xA4;	// sync byte
	msg[1] = 0x01;	// data length
	msg[2] = 0x4A;	// reset command
	msg[3] = 0x00;	// filler byte
	msg[4] = msg[0] ^ msg[1] ^ msg[2] ^ msg[3]; // checksum
	rf_send(msg, 5);
	_delay_ms(2000);
  
	// read startup message
	// 4A:01:6F:10:<checksum>
	rf_receive(msg, 5);
	for(i=0; i<5; i++) 
	{
		usart_putchar(&USARTE0, msg[i]);
	}
  
	//enable crystal message
	msg[0]=0x4A;	// sync byte
	msg[1]=0x01;	// data length
	msg[2]=0x6D;	// enable crystal command
	msg[3]=0x00;	// filler byte
	msg[4]=msg[0] ^ msg[1] ^ msg[2] ^ msg[3]; //checksum
	rf_send(msg, 5);
  
	//read channel response
	rf_receive(msg, 7);
	for(i=0;i<7;i++) 
	{
		usart_putchar(&USARTE0, msg[i]);
	}
  
	//request ant version
	msg[0]=0x4A; //sync byte
	msg[1]=0x02;  //data length
	msg[2]=0x4D; //request info request
	msg[3]=0x01; //channel no
	msg[4]=0x3E;  //ant version command
	msg[5]=msg[0] ^ msg[1] ^ msg[2] ^ msg[3] ^ msg[4]; //checksum
	rf_send(msg, 6);
  
	//read ant version response
	rf_receive(msg, 15);
	for(i=0;i<15;i++) 
	{
		usart_putchar(&USARTE0, msg[i]);
	}
}
	
// - gprs (usart)
void test_gprs()
{
	uint8_t ret;
	uint8_t msg[16];
	// TODO: read status registry or equivalent...
	
	// power on the device...
	gprs_on();
	
	// sanity check...
	// any connectivity?
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '\r';
	msg[3] = '\n'; // BUG: only \r ie CR shall be sent...
	ret = gprs_send(msg, 4);
//	ret = gprs_receive(msg, 16);
	
	// AT+CGMR: Returns the Software version information
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '+';
	msg[3] = 'C';
	msg[4] = 'G';
	msg[5] = 'M';
	msg[6] = 'R';
	msg[7] = '\r';
	msg[8] = '\n';
	ret = gprs_send(msg, 9);
//	ret = gprs_receive(msg, 16);
	
	// AT+CGMM: Returns the Telit Module identification
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '+';
	msg[3] = 'C';
	msg[4] = 'G';
	msg[5] = 'M';
	msg[6] = 'M';
	msg[7] = '\r';
	msg[8] = '\n';
	ret = gprs_send(msg, 9);
//	ret = gprs_receive(msg, 16);
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

