/**
 * \file
 *
 * \brief Metormote Base board init.
 *
 * This file contains board initialization function.
 *
 * Copyright (C) 2011 Metormote AB. All rights reserved.
 */
#include "board.h"
#include "compiler.h"
#include "metormote_base.h"
#include "conf_board.h"
#include "ioport.h"

void board_init(void)
{
	ioport_configure_pin(LED0_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(LED1_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	
	//ioport_configure_pin(GPIO_PUSH_BUTTON_0, IOPORT_DIR_INPUT | IOPORT_PULL_UP);
	ioport_configure_pin(GPIO_PUSH_BUTTON_0, IOPORT_DIR_INPUT);

#ifdef CONF_BOARD_AT45DBX
//	ioport_configure_pin(AT45DBX_MASTER_SS, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(AT45DBX_MASTER_SCK, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(AT45DBX_MASTER_MOSI, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(AT45DBX_MASTER_MISO, IOPORT_DIR_INPUT);
	ioport_configure_pin(AT45DBX_CS, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
#endif

// GPRS...
#ifdef CONF_BOARD_ENABLE_USARTC0
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 3), IOPORT_DIR_OUTPUT
			| IOPORT_INIT_HIGH);
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 2), IOPORT_DIR_INPUT);
#endif

// RF...
#ifdef CONF_BOARD_ENABLE_USARTD0
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 3), IOPORT_DIR_OUTPUT
			| IOPORT_INIT_HIGH);
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 2), IOPORT_DIR_INPUT);
#endif

// debug...
#ifdef CONF_BOARD_ENABLE_USARTE0
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTE, 3), IOPORT_DIR_OUTPUT
			| IOPORT_INIT_HIGH);
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTE, 2), IOPORT_DIR_INPUT);
#endif

	// configure nRESET for rf...
	ioport_configure_pin(nRF24AP2_nRESET, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	
	// configure nSUSPEND for rf...
	ioport_configure_pin(nRF24AP2_nSUSPEND, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);

	// configure SLEEP for rf...
	ioport_configure_pin(nRF24AP2_SLEEP, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);

	// configure RTS for rf...
	ioport_configure_pin(nRF24AP2_RTS, IOPORT_DIR_INPUT);
	
	
	// configure ON/OFF for gprs
	ioport_configure_pin(GPRS_ON_OFF, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	
	// configure RESET for gprs
	ioport_configure_pin(GPRS_RESET, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	
}
