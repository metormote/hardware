/*
 * test.c
 *
 * Created: 2011-05-13 10:57:32
 *  Author: niklo
 */ 

#include <asf.h>
#include <util/delay.h>
#include <asf/common/components/memory/data_flash/at45dbx/at45dbx.h>
#include "test.h"
#include "debug.h"
#include "rf.h"
#include "gprs.h"

void test_main()
{
//	test_debug_usart();
//	test_switch_and_leds();
//	test_rf();
//	test_gprs();
//	test_flash();
	test_rtc();
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
	
// - gprs (usart)
void test_gprs()
{
	uint8_t i,ret;
	uint8_t msg[32];
	// TODO: read status registry or equivalent...
	
	// power on the device...
	gprs_on();
	
	_delay_ms(4000);
	
	// sanity check...
	// any connectivity?
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '\r';
	msg[3] = '\n'; // BUG: only \r ie CR shall be sent...
	ret = gprs_send(msg, 4);
	ret = gprs_receive(msg, 10);
	
	_delay_ms(500);
	
	//echo off
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = 'E';
	msg[3] = '0';
	msg[4] = '\r';
	msg[5] = '\n'; 
	ret = gprs_send(msg, 6);
	ret = gprs_receive(msg, 12);
	
	_delay_ms(500);
	
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '#';
	msg[3] = 'C';
	msg[4] = 'B';
	msg[5] = 'C';
	msg[6] = '\r';
	msg[7] = '\n'; 
	//ret = gprs_send(msg, 8);
	//ret = gprs_receive(msg, 16);
	
	_delay_ms(500);
	
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '+';
	msg[3] = 'I';
	msg[4] = 'P';
	msg[5] = 'R';
	msg[6] = '=';
	msg[7] = '5';
	msg[8] = '7';
	msg[9] = '6';
	msg[10] = '0';
	msg[11] = '0';
	msg[12] = '\r';
	msg[13] = '\n'; 
	ret = gprs_send(msg, 14);
	ret = gprs_receive(msg, 6);
	
	_delay_ms(500);
	
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '+';
	msg[3] = 'C';
	msg[4] = 'M';
	msg[5] = 'E';
	msg[6] = 'E';
	msg[7] = '=';
	msg[8] = '1';
	msg[9] = '\r';
	msg[10] = '\n'; 
	ret = gprs_send(msg, 11);
	ret = gprs_receive(msg, 6);
	
	_delay_ms(500);
	
	// AT#REGMODE=1: Set regmode
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '#';
	msg[3] = 'R';
	msg[4] = 'E';
	msg[5] = 'G';
	msg[6] = 'M';
	msg[7] = 'O';
	msg[8] = 'D';
	msg[9] = 'E';
	msg[10] = '=';
	msg[11] = '1';
	msg[12] = '\r';
	msg[13] = '\n';
	ret = gprs_send(msg, 14);
	ret = gprs_receive(msg, 6);
	
	_delay_ms(500);
	
	// AT+CMGF=1: Set SMS text mode
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '+';
	msg[3] = 'C';
	msg[4] = 'M';
	msg[5] = 'G';
	msg[6] = 'F';
	msg[7] = '=';
	msg[8] = '1';
	msg[9] = '\r';
	msg[10] = '\n';
	ret = gprs_send(msg, 11);
	ret = gprs_receive(msg, 6);
	
	_delay_ms(500);
	
	// AT#SMSMODE=0: Disable improved commands
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '#';
	msg[3] = 'S';
	msg[4] = 'M';
	msg[5] = 'S';
	msg[6] = 'M';
	msg[7] = 'O';
	msg[8] = 'D';
	msg[9] = 'E';
	msg[10] = '=';
	msg[11] = '0';
	msg[12] = '\r';
	msg[13] = '\n';
	ret = gprs_send(msg, 14);
	ret = gprs_receive(msg, 6);
	
	_delay_ms(2000);
	
	// AT+CPMS="ME": Select sms memory
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '+';
	msg[3] = 'C';
	msg[4] = 'P';
	msg[5] = 'M';
	msg[6] = 'S';
	msg[7] = '=';
	msg[8] = '"';
	msg[9] = 'M';
	msg[10] = 'E';
	msg[11] = '"';
	msg[12] = '\r';
	msg[13] = '\n';
	ret = gprs_send(msg, 14);
	ret = gprs_receive(msg, 30);
	
	_delay_ms(500);
	
	// AT+CNMI=1,1,0,0,0: Select sms memory storage
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '+';
	msg[3] = 'C';
	msg[4] = 'N';
	msg[5] = 'M';
	msg[6] = 'I';
	msg[7] = '=';
	msg[8] = '1';
	msg[9] = ',';
	msg[10] = '1';
	msg[11] = ',';
	msg[12] = '0';
	msg[13] = ',';
	msg[14] = '0';
	msg[15] = ',';
	msg[16] = '0';
	msg[17] = '\r';
	msg[18] = '\n';
	ret = gprs_send(msg, 19);
	ret = gprs_receive(msg, 6);
	
	_delay_ms(500);
	
	// AT+CSMP= 17,167,0,0: Set text mode params
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '+';
	msg[3] = 'C';
	msg[4] = 'S';
	msg[5] = 'M';
	msg[6] = 'P';
	msg[7] = '=';
	msg[8] = '1';
	msg[9] = '7';
	msg[10] = ',';
	msg[11] = '1';
	msg[12] = '6';
	msg[13] = '7';
	msg[14] = ',';
	msg[15] = '0';
	msg[16] = ',';
	msg[17] = '0';
	msg[18] = '\r';
	msg[19] = '\n';
	ret = gprs_send(msg, 20);
	ret = gprs_receive(msg, 6);
	
	_delay_ms(2000);
	
	for (i=0;i<2;i++)
	{
		// AT+CREG?: Query network status
		msg[0] = 'A';
		msg[1] = 'T';
		msg[2] = '+';
		msg[3] = 'C';
		msg[4] = 'R';
		msg[5] = 'E';
		msg[6] = 'G';
		msg[7] = '?';
		msg[8] = '\r';
		msg[9] = '\n';
		ret = gprs_send(msg, 10);
		ret = gprs_receive(msg, 20);
	
		_delay_ms(200);
	}
		
	// ATD +390404X92XYX;: Voice call
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = 'D';
	msg[3] = '+';
	msg[4] = '4';
	msg[5] = '6';
	msg[6] = '7';
	msg[7] = '0';
	msg[8] = '8';
	msg[9] = '5';
	msg[10] = '2';
	msg[11] = '2';
	msg[12] = '1';
	msg[13] = '6';
	msg[14] = '4';
	msg[15] = ';';
	msg[16] = '\r';
	//msg[17] = '\n';
	ret = gprs_send(msg, 17);
	ret = gprs_receive(msg,14);
	
	// AT+CMGS=\"%s\",145: Send sms
	msg[0] = 'A';
	msg[1] = 'T';
	msg[2] = '+';
	msg[3] = 'C';
	msg[4] = 'M';
	msg[5] = 'G';
	msg[6] = 'S';
	msg[7] = '=';
	msg[8] = '"';
	msg[9] = '+';
	msg[10] = '4';
	msg[11] = '6';
	msg[12] = '7';
	msg[13] = '0';
	msg[14] = '8';
	msg[15] = '5';
	msg[16] = '2';
	msg[17] = '2';
	msg[18] = '1';
	msg[19] = '6';
	msg[20] = '4';
	msg[21] = '"';
	msg[22] = ',';
	msg[23] = '1';
	msg[24] = '4';
	msg[25] = '5';
	msg[26] = '\r';
	//msg[27] = '\n';
	ret = gprs_send(msg, 27);
	ret = gprs_receive(msg, 4);
	_delay_ms(200);
	msg[0] = 'T';
	msg[1] = 'e';
	msg[2] = 's';
	msg[3] = 't';
	msg[4] = '1';
	msg[5] = '2';
	msg[6] = '4';
	msg[7] = 0x1A;
	ret = gprs_send(msg, 8);
	ret = gprs_receive(msg, 18);

	msg[0] = 'T';
}

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
	
	if(bcd % 2) {
		gpio_set_pin_low(LED1_GPIO);
	}
	else {
		gpio_set_pin_high(LED1_GPIO);
	}
}

// - rtc
void test_rtc()
{
	// For now skip the external RTC instead use the internal...
	// TODO: implement simple test
	rtc_set_callback(alarm);

	cpu_irq_enable();

	/* We just initialized the counter so an alarm should trigger on next
	 * time unit roll over.
	 */
	rtc_set_alarm_relative(0);

	while (true) {
		/* Alarm action is handled in alarm callback so we just go to
		 * sleep here.
		 */
		sleepmgr_enter_sleep();
	}
	
}



// internal adc...
void test_adc()
{
	// TODO: implement...	
//	adc_enable(ADCA);
//	adc_start_conversion(ADCA, 0xff);
}