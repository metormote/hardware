/**
 * \file
 *
 * \brief Basestation Test
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * AVR Software Framework (ASF).
 */
#include <asf.h>
#include "at45dbx.h"
#include "conf_board.h"


/* Constants */

#define ANT_USART                        &USARTD0

#define GPRS_USART                       &USARTC0


// ANT USART options.
static usart_rs232_options_t ANT_USART_OPTIONS = {
	.baudrate = 57600,
	.charlength = USART_CHSIZE_8BIT_gc,
	.paritytype = USART_PMODE_DISABLED_gc,
	.stopbits = false
};

// GPRS USART options.
static usart_rs232_options_t GPRS_USART_OPTIONS = {
	.baudrate = 38400,
	.charlength = USART_CHSIZE_8BIT_gc,
	.paritytype = USART_PMODE_DISABLED_gc,
	.stopbits = false
};

static void init(void) {
	//initialize interrupt controller
	pmic_init();
	//initialize board
	board_init();
	//initialize system clock
	sysclk_init();
	//initialize sleep manager
	sleepmgr_init();
	//initialize data flash
	at45dbx_init();
	//initialize real time counter
	rtc_init();
	
	rtc_set_time(TIME_ZERO);
	
	
	// Initialize ant usart driver in RS232 mode
	usart_init_rs232(ANT_USART, &ANT_USART_OPTIONS);
	usart_set_rx_interrupt_level(ANT_USART, PMIC_LVL_HIGH);
	usart_set_tx_interrupt_level(ANT_USART, PMIC_LVL_HIGH);
	usart_set_dre_interrupt_level(ANT_USART, PMIC_LVL_HIGH);
	
	// Initialize gprs usart driver in RS232 mode
	usart_init_rs232(GPRS_USART, &GPRS_USART_OPTIONS);
	usart_set_rx_interrupt_level(GPRS_USART, PMIC_LVL_HIGH);
	usart_set_tx_interrupt_level(GPRS_USART, PMIC_LVL_HIGH);
	usart_set_dre_interrupt_level(GPRS_USART, PMIC_LVL_HIGH);
	
	
}

int main (void)
{
  
	//initialize all modules
	init();
	
	while (1)
      ;

  /* There was an error starting the OS if we reach here */
  return (0);
	
}

