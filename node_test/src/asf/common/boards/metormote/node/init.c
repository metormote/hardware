/**
 * \file
 *
 * \brief Metormote Node board init.
 *
 * This file contains board initialization function.
 *
 * Copyright (C) 2011 Metormote AB. All rights reserved.
 */
#include "board.h"
#include "compiler.h"
#include "metormote_node.h"
#include "conf_board.h"
#include "ioport.h"

void board_init(void)
{

	ioport_configure_pin(GPIO_POWER_VBAT_SENSE, IOPORT_DIR_INPUT);
	ioport_configure_pin(GPIO_POWER_VUSB_SENSE, IOPORT_DIR_INPUT);
	ioport_configure_pin(GPIO_POWER_GOOD, IOPORT_DIR_INPUT);
	ioport_configure_pin(GPIO_POWER_DCDC_ENABLE, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	
	ioport_configure_pin(GPIO_LED_RED, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(GPIO_LED_GREEN, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(GPIO_LED_YELLOW, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(GPIO_LED_ORANGE, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	
	//ioport_configure_pin(GPIO_SWITCH, IOPORT_DIR_INPUT | IOPORT_PULL_UP);
	ioport_configure_pin(GPIO_SWITCH, IOPORT_DIR_INPUT);
  
	ioport_configure_pin(GPIO_SENSOR_TEMP, IOPORT_DIR_INPUT);
  
// RF
#ifdef CONF_BOARD_ENABLE_ANT
	ioport_configure_pin(nRF24AP2_TX, IOPORT_DIR_INPUT);
	ioport_configure_pin(nRF24AP2_RX, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	// configure nRESET for rf...
	ioport_configure_pin(nRF24AP2_nRESET, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	// configure nSUSPEND for rf...
	ioport_configure_pin(nRF24AP2_nSUSPEND, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	// configure SLEEP for rf...
	ioport_configure_pin(nRF24AP2_SLEEP, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	// configure RTS for rf...
	ioport_configure_pin(nRF24AP2_RTS, IOPORT_DIR_INPUT);
#endif


#ifdef CONF_BOARD_ENABLE_BLE
	ioport_configure_pin(BLE_RESET, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(BLE_ACTIVE, IOPORT_DIR_INPUT);
	ioport_configure_pin(BLE_RDYN, IOPORT_DIR_INPUT);
	ioport_configure_pin(BLE_REQN, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(BLE_MASTER_SCK, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(BLE_MASTER_MOSI, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(BLE_MASTER_MISO, IOPORT_DIR_INPUT);
	ioport_configure_pin(BLE_TXD, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(BLE_RXD, IOPORT_DIR_INPUT);
#endif

}
