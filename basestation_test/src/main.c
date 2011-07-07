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
#include <util/delay.h>

#include "at45dbx.h"
#include "conf_board.h"

#include "rf.h"
#include "gprs.h"
#include "test.h"
#include "debug.h"

volatile uint8_t iflag;

// ANT USART options.
static usart_rs232_options_t ANT_USART_OPTIONS = {
	.baudrate = 57600,
	.charlength = USART_CHSIZE_8BIT_gc,
	.paritytype = USART_PMODE_DISABLED_gc,
	.stopbits = false
};

// GPRS USART options.
static usart_rs232_options_t GPRS_USART_OPTIONS = {
	.baudrate = 57600,
	.charlength = USART_CHSIZE_8BIT_gc,
	.paritytype = USART_PMODE_DISABLED_gc,
	.stopbits = false
};

// DEBUG USART options.
static usart_rs232_options_t DEBUG_USART_OPTIONS = {
	.baudrate = 9600,
	.charlength = USART_CHSIZE_8BIT_gc,
	.paritytype = USART_PMODE_DISABLED_gc,
	.stopbits = 1
};


static void init(void) 
{
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
	osc_enable(OSC_ID_RC32KHZ);
	rtc_init();
	rtc_set_time(TIME_ZERO);
	RTC.PER = 0x01;
	RTC.INTFLAGS=0x03;
	
	
	// initialize rf circuits...
	rf_init();
	
	// init gprs
	gprs_init();
	
	// Initialize ant usart driver in RS232 mode
	usart_init_rs232(ANT_USART, &ANT_USART_OPTIONS);
	//usart_set_rx_interrupt_level(ANT_USART, USART_RXCINTLVL_HI_gc);
	//usart_set_tx_interrupt_level(ANT_USART, USART_TXCINTLVL_HI_gc);
	//usart_set_dre_interrupt_level(ANT_USART, USART_DREINTLVL_HI_gc);
	
	// Initialize gprs usart driver in RS232 mode
	usart_init_rs232(GPRS_USART, &GPRS_USART_OPTIONS);
	//usart_set_rx_interrupt_level(GPRS_USART, USART_RXCINTLVL_HI_gc);
	//usart_set_tx_interrupt_level(GPRS_USART, USART_TXCINTLVL_HI_gc);
	//usart_set_dre_interrupt_level(GPRS_USART, USART_DREINTLVL_HI_gc);
	
	// Initialize debug usart driver in RS232 mode
	usart_init_rs232(DEBUG_USART, &DEBUG_USART_OPTIONS);
	
}

int main (void)
{
	// initialize all modules
	init();
	
	_delay_ms(1000);
	
	// enable interrupts...
	cpu_irq_enable();	// needed for rtc to work...
	
	while (1)
	{
		// do something useful...
		iflag=0xFF;
		test_main();
	}

	/* There was an error starting the OS if we reach here */
	return (0);
	
}



ISR (BADISR_vect)
{
    iflag=0;
}


/* OSC interrupt vectors */
ISR (OSC_XOSCF_vect) {iflag=1;} /* External Oscillator Failure Interrupt (NMI) */

/* PORTC interrupt vectors */
ISR (PORTC_INT0_vect) {iflag=2;}  /* External Interrupt 0 */
ISR (PORTC_INT1_vect) {iflag=3;}  /* External Interrupt 1 */

/* PORTR interrupt vectors */
ISR (PORTR_INT0_vect) {iflag=4;}  /* External Interrupt 0 */
ISR (PORTR_INT1_vect) {iflag=5;}  /* External Interrupt 1 */

/* DMA interrupt vectors */
//ISR (DMA_CH0_vect) {iflag=6;}  /* Channel 0 Interrupt */
//ISR (DMA_CH1_vect) {iflag=7;}  /* Channel 1 Interrupt */
//ISR (DMA_CH2_vect) {iflag=8;}  /* Channel 2 Interrupt */
//ISR (DMA_CH3_vect) {iflag=9;}  /* Channel 3 Interrupt */

/* RTC interrupt vectors */
//ISR (RTC_OVF_vect) {iflag=10;}  /* Overflow Interrupt */
//ISR (RTC_COMP_vect) {iflag=11;}  /* Compare Interrupt */

/* TWIC interrupt vectors */
ISR (TWIC_TWIS_vect) {iflag=12;}  /* TWI Slave Interrupt */
ISR (TWIC_TWIM_vect) {iflag=13;}  /* TWI Master Interrupt */

/* TCC0 interrupt vectors */
//ISR (TCC0_OVF_vect) {iflag=14;}  /* Overflow Interrupt */
//ISR (TCC0_ERR_vect) {iflag=15;}  /* Error Interrupt */
//ISR (TCC0_CCA_vect) {iflag=16;}  /* Compare or Capture A Interrupt */
//ISR (TCC0_CCB_vect) {iflag=17;}  /* Compare or Capture B Interrupt */
//ISR (TCC0_CCC_vect) {iflag=18;}  /* Compare or Capture C Interrupt */
//ISR (TCC0_CCD_vect) {iflag=19;}  /* Compare or Capture D Interrupt */

/* TCC1 interrupt vectors */
//ISR (TCC1_OVF_vect) {iflag=20;}  /* Overflow Interrupt */
//ISR (TCC1_ERR_vect) {iflag=21;}  /* Error Interrupt */
//ISR (TCC1_CCA_vect) {iflag=22;}  /* Compare or Capture A Interrupt */
//ISR (TCC1_CCB_vect) {iflag=23;}  /* Compare or Capture B Interrupt */

/* SPIC interrupt vectors */
ISR (SPIC_INT_vect) {iflag=24;}  /* SPI Interrupt */

/* USARTC0 interrupt vectors */
ISR (USARTC0_RXC_vect) {iflag=25;}  /* Reception Complete Interrupt */
ISR (USARTC0_DRE_vect) {iflag=26;}  /* Data Register Empty Interrupt */
ISR (USARTC0_TXC_vect) {iflag=27;}  /* Transmission Complete Interrupt */

/* USARTC1 interrupt vectors */
ISR (USARTC1_RXC_vect) {iflag=28;}  /* Reception Complete Interrupt */
ISR (USARTC1_DRE_vect) {iflag=29;}  /* Data Register Empty Interrupt */
ISR (USARTC1_TXC_vect) {iflag=30;}  /* Transmission Complete Interrupt */

/* AES interrupt vectors */
//ISR (AES_INT_vect) {iflag=31;}  /* AES Interrupt */

/* NVM interrupt vectors */
ISR (NVM_EE_vect) {iflag=32;}  /* EE Interrupt */
ISR (NVM_SPM_vect) {iflag=33;}  /* SPM Interrupt */

/* PORTB interrupt vectors */
ISR (PORTB_INT0_vect) {iflag=34;}  /* External Interrupt 0 */
ISR (PORTB_INT1_vect) {iflag=35;}  /* External Interrupt 1 */

/* PORTE interrupt vectors */
ISR (PORTE_INT0_vect) {iflag=36;}  /* External Interrupt 0 */
ISR (PORTE_INT1_vect) {iflag=37;}  /* External Interrupt 1 */

/* TWIE interrupt vectors */
ISR (TWIE_TWIS_vect) {iflag=38;}  /* TWI Slave Interrupt */
ISR (TWIE_TWIM_vect) {iflag=39;}  /* TWI Master Interrupt */

/* TCE0 interrupt vectors */
//ISR (TCE0_OVF_vect) {iflag=40;}  /* Overflow Interrupt */
//ISR (TCE0_ERR_vect) {iflag=41;}  /* Error Interrupt */
//ISR (TCE0_CCA_vect) {iflag=42;}  /* Compare or Capture A Interrupt */
//ISR (TCE0_CCB_vect) {iflag=43;}  /* Compare or Capture B Interrupt */
//ISR (TCE0_CCC_vect) {iflag=44;}  /* Compare or Capture C Interrupt */
//ISR (TCE0_CCD_vect) {iflag=45;}  /* Compare or Capture D Interrupt */

/* TCE1 interrupt vectors */
ISR (TCE1_OVF_vect) {iflag=46;}  /* Overflow Interrupt */
ISR (TCE1_ERR_vect) {iflag=47;}  /* Error Interrupt */
ISR (TCE1_CCA_vect) {iflag=48;}  /* Compare or Capture A Interrupt */
ISR (TCE1_CCB_vect) {iflag=49;}  /* Compare or Capture B Interrupt */

/* USARTE0 interrupt vectors */
ISR (USARTE0_RXC_vect) {iflag=50;}  /* Reception Complete Interrupt */
ISR (USARTE0_DRE_vect) {iflag=51;}  /* Data Register Empty Interrupt */
ISR (USARTE0_TXC_vect) {iflag=52;}  /* Transmission Complete Interrupt */

/* PORTD interrupt vectors */
ISR (PORTD_INT0_vect) {iflag=53;}  /* External Interrupt 0 */
ISR (PORTD_INT1_vect) {iflag=54;}  /* External Interrupt 1 */

/* PORTA interrupt vectors */
ISR (PORTA_INT0_vect) {iflag=55;}  /* External Interrupt 0 */
ISR (PORTA_INT1_vect) {iflag=56;}  /* External Interrupt 1 */

/* ACA interrupt vectors */
//ISR (ACA_AC0_vect) {iflag=57;}  /* AC0 Interrupt */
//ISR (ACA_AC1_vect) {iflag=58;}  /* AC1 Interrupt */
//ISR (ACA_ACW_vect) {iflag=59;}  /* ACW Window Mode Interrupt */

/* ADCA interrupt vectors */
ISR (ADCA_CH0_vect) {iflag=60;}  /* Interrupt 0 */
ISR (ADCA_CH1_vect) {iflag=61;}  /* Interrupt 1 */
ISR (ADCA_CH2_vect) {iflag=62;}  /* Interrupt 2 */
ISR (ADCA_CH3_vect) {iflag=63;}  /* Interrupt 3 */

/* TCD0 interrupt vectors */
//ISR (TCD0_OVF_vect) {iflag=64;}  /* Overflow Interrupt */
//ISR (TCD0_ERR_vect) {iflag=65;}  /* Error Interrupt */
//ISR (TCD0_CCA_vect) {iflag=66;}  /* Compare or Capture A Interrupt */
//ISR (TCD0_CCB_vect) {iflag=67;}  /* Compare or Capture B Interrupt */
//ISR (TCD0_CCC_vect) {iflag=68;}  /* Compare or Capture C Interrupt */
//ISR (TCD0_CCD_vect) {iflag=69;}  /* Compare or Capture D Interrupt */

/* TCD1 interrupt vectors */
//ISR (TCD1_OVF_vect) {iflag=70;}  /* Overflow Interrupt */
//ISR (TCD1_ERR_vect) {iflag=71;}  /* Error Interrupt */
//ISR (TCD1_CCA_vect) {iflag=72;}  /* Compare or Capture A Interrupt */
//ISR (TCD1_CCB_vect) {iflag=73;}  /* Compare or Capture B Interrupt */

/* SPID interrupt vectors */
ISR (SPID_INT_vect) {iflag=74;}  /* SPI Interrupt */

/* USARTD0 interrupt vectors */
ISR (USARTD0_RXC_vect) {iflag=75;}  /* Reception Complete Interrupt */
ISR (USARTD0_DRE_vect) {iflag=76;}  /* Data Register Empty Interrupt */
ISR (USARTD0_TXC_vect) {iflag=77;}  /* Transmission Complete Interrupt */

/* USARTD1 interrupt vectors */
ISR (USARTD1_RXC_vect) {iflag=78;}  /* Reception Complete Interrupt */
ISR (USARTD1_DRE_vect) {iflag=79;}  /* Data Register Empty Interrupt */
ISR (USARTD1_TXC_vect) {iflag=80;}  /* Transmission Complete Interrupt */
