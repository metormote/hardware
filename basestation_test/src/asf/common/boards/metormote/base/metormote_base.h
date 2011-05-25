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
#define LED_COUNT   2

/*! \name GPIO Connections of LEDs
 */
//! @{
#  define LED0_GPIO   IOPORT_CREATE_PIN(PORTA,4)
#  define LED1_GPIO   IOPORT_CREATE_PIN(PORTA,5)
//! @}


/*! \name GPIO Connections of Push Buttons
 */
//! @{
#  define GPIO_PUSH_BUTTON_0	IOPORT_CREATE_PIN(PORTA,3)
//! @}

/*! \name SPI Connections of the AT45DBX Data Flash Memory
 */
//! @{
#define AT45DBX_SPI           &SPIC
#define AT45DBX_CS            IOPORT_CREATE_PIN(PORTC,4)  // CS as output
#define AT45DBX_MASTER_SS     IOPORT_CREATE_PIN(PORTC,4)  // SS as output
#define AT45DBX_MASTER_SCK    IOPORT_CREATE_PIN(PORTC,7)  // SCK as output
#define AT45DBX_MASTER_MOSI   IOPORT_CREATE_PIN(PORTC,5)  // MOSI as output
#define AT45DBX_MASTER_MISO   IOPORT_CREATE_PIN(PORTC,6)  // MISO as input
#define AT45DBX_SPI_CLK_MASK  SYSCLK_PORT_C

//! @}


/*! \name External oscillator
 */
//@{
#define BOARD_XOSC_HZ          32768
#define BOARD_XOSC_TYPE        XOSC_TYPE_32KHZ
#define BOARD_XOSC_STARTUP_US  500000
//@}

#define nRF24AP2_nSUSPEND	IOPORT_CREATE_PIN(PORTB, 0)
#define nRF24AP2_SLEEP		IOPORT_CREATE_PIN(PORTB, 1)
#define nRF24AP2_RTS		IOPORT_CREATE_PIN(PORTB, 2)

#define GPRS_ON_OFF			IOPORT_CREATE_PIN(PORTA, 6)
#define GPRS_RESET			IOPORT_CREATE_PIN(PORTA, 7)

#define DATAFLASH_WP		IOPORT_CREATE_PIN(PORTB, 3)

/* Constants */
#define ANT_USART			&USARTD0
#define GPRS_USART			&USARTC0
#define DEBUG_USART			&USARTE0

#endif  // _METORMOTE_BASE_H_
