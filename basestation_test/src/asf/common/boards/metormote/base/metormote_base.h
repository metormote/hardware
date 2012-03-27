/**
 * \file
 *
 * \brief Metormote Base board header file.
 *
 * This file contains definitions and services related to the features of the
 * Metormote Base board.
 *
 * To use this board, define BOARD=METORMOTE_BASE.

 * Copyright (C) 2011 Metormote AB. All rights reserved.
 */
#ifndef _METORMOTE_BASE_H_
#define _METORMOTE_BASE_H_

#include "compiler.h"


/*! \name Miscellaneous data
 */
//! @{
//! @}

//! Number of LEDs.
#define LED_COUNT 3

/*! \name GPIO Connections of LEDs
 */
//! @{
#define LED0_GPIO IOPORT_CREATE_PIN(PORTB, 4) // GREEN
#define LED1_GPIO IOPORT_CREATE_PIN(PORTB, 5) // RED
#define LED2_GPIO IOPORT_CREATE_PIN(PORTB, 6) // ETH, NOTE: not connected in prototype...
//! @}

/*! \name GPIO Connections of Push Buttons
 */
//! @{
#define GPIO_PUSH_BUTTON_0 IOPORT_CREATE_PIN(PORTB, 3)
//! @}

/*! \name SPI Connections of the AT45DBX Data Flash Memory
 */
//! @{
#define AT45DBX_SPI           &SPID
#define AT45DBX_CS            IOPORT_CREATE_PIN(PORTD, 4)  // CS as output
//#define AT45DBX_MASTER_SS     IOPORT_CREATE_PIN(PORTC,4)  // SS as output
#define AT45DBX_MASTER_SCK    IOPORT_CREATE_PIN(PORTD, 7)  // SCK as output
#define AT45DBX_MASTER_MOSI   IOPORT_CREATE_PIN(PORTD, 5)  // MOSI as output
#define AT45DBX_MASTER_MISO   IOPORT_CREATE_PIN(PORTD, 6)  // MISO as input
#define AT45DBX_SPI_CLK_MASK  SYSCLK_PORT_D
//! @}

/*! \name SPI Connections of the Ethernet Controller
 */
//! @{
#define ETH_SPI           &SPIE
#define ETH_CS            IOPORT_CREATE_PIN(PORTE, 4)  // CS as output
#define ETH_MASTER_SCK    IOPORT_CREATE_PIN(PORTE, 7)  // SCK as output
#define ETH_MASTER_MOSI   IOPORT_CREATE_PIN(PORTE, 5)  // MOSI as output
#define ETH_MASTER_MISO   IOPORT_CREATE_PIN(PORTE, 6)  // MISO as input
#define ETH_SPI_CLK_MASK  SYSCLK_PORT_E
//! @}

/*! \name SPI Connections of the BLE Transceiver
 */
//! @{
#define BLE_SPI           &SPIC
//#define BLE_CS            IOPORT_CREATE_PIN(PORTE, 4)  // CS as output
#define BLE_MASTER_SCK    IOPORT_CREATE_PIN(PORTC, 7)  // SCK as output
#define BLE_MASTER_MOSI   IOPORT_CREATE_PIN(PORTC, 5)  // MOSI as output
#define BLE_MASTER_MISO   IOPORT_CREATE_PIN(PORTC, 6)  // MISO as input
#define BLE_SPI_CLK_MASK  SYSCLK_PORT_C
//! @}

/*! \name External oscillator
 */
//@{
#define BOARD_XOSC_HZ          16000000
#define BOARD_XOSC_TYPE        XOSC_TYPE_XTAL
#define BOARD_XOSC_STARTUP_US  1000
//#define BOARD_XOSC_HZ          32768
//#define BOARD_XOSC_TYPE        XOSC_TYPE_32KHZ
//#define BOARD_XOSC_STARTUP_US  500000
//@}

#define F_CPU   16000000UL

#define ANT_nSUSPEND		IOPORT_CREATE_PIN(PORTF, 4)
#define ANT_SLEEP			IOPORT_CREATE_PIN(PORTF, 5)
#define ANT_RTS				IOPORT_CREATE_PIN(PORTF, 6)
#define ANT_nRESET			IOPORT_CREATE_PIN(PORTF, 7)

#define GPRS_STATUS			IOPORT_CREATE_PIN(PORTF, 0)
#define GPRS_POWER_MON		IOPORT_CREATE_PIN(PORTF, 1)
#define GPRS_RESET			IOPORT_CREATE_PIN(PORTF, 2)
#define GPRS_ON_OFF			IOPORT_CREATE_PIN(PORTF, 3)

//#define DATAFLASH_WP		IOPORT_CREATE_PIN(PORTB, 3)

#define ETH_nINT			IOPORT_CREATE_PIN(PORTA, 5)
#define ETH_nRST			IOPORT_CREATE_PIN(PORTA, 6)
#define ETH_PWDN			IOPORT_CREATE_PIN(PORTA, 7)
#define ETH_nLINKLED		IOPORT_CREATE_PIN(PORTB, 2)

#define POWER_DCM			IOPORT_CREATE_PIN(PORTB, 0)
#define CHARGER_DOK			IOPORT_CREATE_PIN(PORTB, 1)

#define DATAFLASH_RESET		IOPORT_CREATE_PIN(PORTC, 4)

#define BLE_ACTIVE			IOPORT_CREATE_PIN(PORTD, 0)
#define BLE_RESET			IOPORT_CREATE_PIN(PORTD, 1)
#define BLE_RDYN			IOPORT_CREATE_PIN(PORTE, 0)
#define BLE_REQN			IOPORT_CREATE_PIN(PORTE, 1)

/* Constants */
#define ANT_USART	&USARTD0
#define GPRS_USART	&USARTC0
#define BLE_USART	&USARTE0

#endif  // _METORMOTE_BASE_H_
