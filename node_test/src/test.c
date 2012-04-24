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
#include "sensors.h"

void test_main()
{
//	test_debug_usart();
	test_switch_and_leds();
//	test_rf();
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
//	test_prologue("Switch and LEDs...\n");

	// init
	gpio_set_pin_high(GPIO_LED_RED);
	gpio_set_pin_high(GPIO_LED_GREEN);
	gpio_set_pin_high(GPIO_LED_YELLOW);
	gpio_set_pin_high(GPIO_LED_ORANGE);
	
//	debug_print("Step 1. GPIO_LED_RED");
	debug_wait_for_step();
	gpio_set_pin_low(GPIO_LED_RED);
	
//	debug_print("Step 2. GPIO_LED_GREEN");
	debug_wait_for_step();
	gpio_set_pin_low(GPIO_LED_GREEN);
	
//	debug_print("Step 3. GPIO_LED_YELLOW");
	debug_wait_for_step();
	gpio_set_pin_low(GPIO_LED_YELLOW);
	
//	debug_print("Step 4. GPIO_LED_ORANGE");
	debug_wait_for_step();
	gpio_set_pin_low(GPIO_LED_ORANGE);
	
//	debug_print("Finished");
	debug_wait_for_step();
	
	// exit
	gpio_set_pin_high(GPIO_LED_RED);
	gpio_set_pin_high(GPIO_LED_GREEN);
	gpio_set_pin_high(GPIO_LED_YELLOW);
	gpio_set_pin_high(GPIO_LED_ORANGE);
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
	int i, j;
	uint8_t msg[16];
	for(i=0;i<16;i++) msg[i]=i;
	//rf_send(msg, 5);
	//rf_receive(msg, 4);
	
	// TODO: test usart communication e.g. read status registry
	// NOTE: in order to send data the rts signal has to be obeyed
	// TODO: test SLEEP and nSUSPEND
	
	rf_reset(true);
	rf_reset(false);
	rf_receive(msg, 5);
	//_delay_ms(5000);
	

	rf_sleep(true);
	rf_sleep(false);
	//rf_receive(msg, 5);
	
	rf_suspend(true);
	rf_suspend(false);
	rf_receive(msg, 5);
  
	//enable crystal message
	msg[0]=0xA4;	// sync byte
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
	
	//assign channel
	msg[0]=0xA4; //sync byte
	msg[1]=0x04;  //data length
	msg[2]=0x42; //assign channel
	msg[3]=0x00; //channel no
	msg[4]=0x10;  //channel type 0x00=slave 0x10=master
	msg[5]=0x0;  //network number
	msg[6]=0x0;  //extended assignment
	msg[7]=msg[0] ^ msg[1] ^ msg[2] ^ msg[3] ^ msg[4] ^ msg[5] ^ msg[6]; //checksum
	rf_send(msg, 8);
  
	//read assign channel response
	rf_receive(msg, 7);
	for(i=0;i<7;i++) 
	{
		usart_putchar(&USARTE0, msg[i]);
	}
	
	//set channel id
	msg[0]=0xA4; //sync byte
	msg[1]=0x05;  //data length
	msg[2]=0x51; //set channel id
	msg[3]=0x00; //channel no
	msg[4]=0x01;  //device no lsb
	msg[5]=0x00;  //device no msb
	msg[6]=0x01;  //device type
	msg[7]=0x01;  //trans type
	msg[8]=msg[0] ^ msg[1] ^ msg[2] ^ msg[3] ^ msg[4] ^ msg[5] ^ msg[6] ^ msg[7]; //checksum
	rf_send(msg, 9);
  
	//read set channel id response
	rf_receive(msg, 7);
	for(i=0;i<7;i++) 
	{
		usart_putchar(&USARTE0, msg[i]);
	}
	
	//set channel freq
	msg[0]=0xA4; //sync byte
	msg[1]=0x02;  //data length
	msg[2]=0x45; //set channel freq
	msg[3]=0x00; //channel no
	msg[4]=0x42;  //freq 2466MHz
	msg[5]=msg[0] ^ msg[1] ^ msg[2] ^ msg[3] ^ msg[4]; //checksum
	rf_send(msg, 6);
  
	//read set channel freq response
	rf_receive(msg, 7);
	for(i=0;i<7;i++) 
	{
		usart_putchar(&USARTE0, msg[i]);
	}
	
	//set tx power
	msg[0]=0xA4; //sync byte
	msg[1]=0x02;  //data length
	msg[2]=0x60; //set tx power
	msg[3]=0x00; //channel no
	msg[4]=0x03;  //power 3=0dB
	msg[5]=msg[0] ^ msg[1] ^ msg[2] ^ msg[3] ^ msg[4]; //checksum
	rf_send(msg, 6);
  
	//read set tx power response
	rf_receive(msg, 7);
	for(i=0;i<7;i++) 
	{
		usart_putchar(&USARTE0, msg[i]);
	}
	
	//set channel period
	msg[0]=0xA4; //sync byte
	msg[1]=0x03;  //data length
	msg[2]=0x43; //set channel period
	msg[3]=0x00; //channel no
	msg[4]=0x00;  //period lsb
	msg[5]=0x20;  //period msb
	msg[6]=msg[0] ^ msg[1] ^ msg[2] ^ msg[3] ^ msg[4] ^ msg[5]; //checksum
	rf_send(msg, 7);
  
	//read set period response
	rf_receive(msg, 7);
	for(i=0;i<7;i++) 
	{
		usart_putchar(&USARTE0, msg[i]);
	}
	
	//set channel search timeout
	msg[0]=0xA4; //sync byte
	msg[1]=0x02;  //data length
	msg[2]=0x44; //set channel search timeout
	msg[3]=0x00; //channel no
	msg[4]=0x0A;  //timeout 10 sec
	msg[5]=msg[0] ^ msg[1] ^ msg[2] ^ msg[3] ^ msg[4]; //checksum
	rf_send(msg, 6);
  
	//read set channel timeout response
	rf_receive(msg, 7);
	for(i=0;i<7;i++) 
	{
		usart_putchar(&USARTE0, msg[i]);
	}
	
	
	//set channel low prio search timeout
	msg[0]=0xA4; //sync byte
	msg[1]=0x02;  //data length
	msg[2]=0x63; //set channel low prio search timeout
	msg[3]=0x00; //channel no
	msg[4]=0x02;  //timeout 5 sec
	msg[5]=msg[0] ^ msg[1] ^ msg[2] ^ msg[3] ^ msg[4]; //checksum
	rf_send(msg, 6);
  
	//read set channel timeout response
	rf_receive(msg, 7);
	for(i=0;i<7;i++) 
	{
		usart_putchar(&USARTE0, msg[i]);
	}
	
	//open channel
	msg[0]=0xA4; //sync byte
	msg[1]=0x01;  //data length
	msg[2]=0x4B; //open channel
	msg[3]=0x00; //channel no
	msg[4]=msg[0] ^ msg[1] ^ msg[2] ^ msg[3]; //checksum
	rf_send(msg, 5);
  
	//read open channel response
	rf_receive(msg, 7);
	for(i=0;i<7;i++) 
	{
		usart_putchar(&USARTE0, msg[i]);
	}
	
	_delay_ms(1000);
	
	//request ant info
	msg[0]=0xA4; //sync byte
	msg[1]=0x02;  //data length
	msg[2]=0x4D; //request info request
	msg[3]=0x00; //channel no
	msg[4]=0x52;  //ant status command
	msg[5]=msg[0] ^ msg[1] ^ msg[2] ^ msg[3] ^ msg[4]; //checksum
	rf_send(msg, 6);
  
	//read ant info response
	rf_receive(msg, 6);
	for(i=0;i<6;i++) 
	{
		usart_putchar(&USARTE0, msg[i]);
	}
	
	_delay_ms(100);
	
    j=0;
	while(true) {
		//send data
		msg[0]=0xA4; //sync byte
		msg[1]=0x09;  //data length
		msg[2]=0x4F; //send data 4E=broadcast 4F=acknowledged
		msg[3]=0x00; //channel no
		msg[4]=0x1;  //data...
		msg[5]=0x2;
		msg[6]=0x3;
		msg[7]=0x4;
		msg[8]=0x5;
		msg[9]=0x6;
		msg[10]=0x7;
		msg[11]=(uint8_t)j++;
		msg[12]=msg[0] ^ msg[1] ^ msg[2] ^ msg[3] ^ msg[4] ^ msg[5] ^ msg[6] ^ msg[7] ^ msg[8] ^ msg[9] ^ msg[10] ^ msg[11]; //checksum
	
		rf_send(msg, 13);
		
		//read response
		rf_receive(msg, 10);
		for(i=0;i<10;i++) 
		{
			usart_putchar(&USARTE0, msg[i]);
		}
		
		_delay_ms(1000);
  	}
	
}
	
/*
// - flash
void test_flash()
{
	int i;
	// Dataflash target sector used in this example
    uint32_t TARGET_SECTOR=0x00002;
	// RAM buffer used in this example
	uint8_t ram_buf[AT45DBX_SECTOR_SIZE];
	
	gpio_set_pin_high(LED0_GPIO);
	gpio_set_pin_high(LED1_GPIO);
	
	if(at45dbx_mem_check())	{
		gpio_set_pin_low(LED0_GPIO);		
	} else
	{
		gpio_set_pin_low(LED1_GPIO);
	}
	
	// Prepare half a data flash sector to 0xAA
	for(i=0;i<AT45DBX_SECTOR_SIZE/2;i++) {
		ram_buf[i]=0xAA;
	}
	// And the remaining half to 0x55
	for(;i<AT45DBX_SECTOR_SIZE;i++) {
		ram_buf[i]=0x55;
	}

	at45dbx_write_sector_open(TARGET_SECTOR);
	at45dbx_write_sector_from_ram(ram_buf);
	at45dbx_write_close();

	// Read back this sector and compare to expected values
	at45dbx_read_sector_open(TARGET_SECTOR);
	at45dbx_read_sector_to_ram(ram_buf);
	at45dbx_read_close();
	for(i=0;i<AT45DBX_SECTOR_SIZE/2;i++) {
		if (ram_buf[i]!=0xAA) {
			gpio_set_pin_low(LED1_GPIO);
		}
	}
	for(;i<AT45DBX_SECTOR_SIZE;i++) {
		if (ram_buf[i]!=0x55) {
			gpio_set_pin_low(LED1_GPIO);
		}
	}

	// Write one data flash sector to 0x00, 0x01 ....	
	for(i=0;i<AT45DBX_SECTOR_SIZE;i++) {
		ram_buf[i]=i;
	}
	at45dbx_write_sector_open(TARGET_SECTOR);
	at45dbx_write_sector_from_ram(ram_buf);
	at45dbx_write_close();
	
	// Read one data flash sector to ram
	at45dbx_read_sector_open(TARGET_SECTOR);
	at45dbx_read_sector_to_ram(ram_buf);
	at45dbx_read_close();
	for(i=0;i<AT45DBX_SECTOR_SIZE;i++) {
		if ( ram_buf[i]!=(i%0x100) ) {
			gpio_set_pin_low(LED1_GPIO);
		}
	}	
	
	gpio_set_pin_low(LED0_GPIO);
	
}
*/

/**
 * \brief Alarm callback
 *
 * Reschedules the alarm in 1 second.
 */
static void alarm(uint32_t time)
{
	uint8_t bcd;

	/* Since the current time will give alarm when rolling over to
	 * next time unit, we just call with that one.
	 * This is safe to here since it's called from a time unit roll
	 * over.
	 */
	rtc_set_alarm(time);

	// Extract last two digits from time, and put them in bcd
	bcd = time % 10;
	time -= bcd;
	time /= 10;
	bcd = bcd | ((time % 10) << 4);
	
	if(bcd % 2) 
	{
		gpio_set_pin_low(GPIO_LED_RED);
	}
	else 
	{
		gpio_set_pin_high(GPIO_LED_RED);
	}
}

// - rtc
void test_rtc()
{
	// For now skip the external RTC instead use the internal...
	rtc_set_callback(alarm);

	cpu_irq_enable();

	/* We just initialized the counter so an alarm should trigger on next
	 * time unit roll over.
	 */
	rtc_set_alarm_relative(0);

	while (true) 
	{
		/* Alarm action is handled in alarm callback so we just go to
		 * sleep here.
		 */
		sleepmgr_enter_sleep();
	}
	

}

// internal adc...
void test_adc(void)
{
	// TODO: implement...	
//	adc_enable(ADCA);
//	adc_start_conversion(ADCA, 0xff);
}

void test_sensors(void)
{
		
}