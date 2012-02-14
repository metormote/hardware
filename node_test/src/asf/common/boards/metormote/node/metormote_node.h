/*
 * metormote_node.h
 *
 * Created: 9/9/2011 2:45:52 PM
 *  Author: Administrator
 */ 
#ifndef METORMOTE_NODE_H_
#define METORMOTE_NODE_H_


#include "compiler.h"


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



/*! \name Power functions pin mapping
 */
//! @{
//Battery power
#define GPIO_POWER_VBAT_SENSE     IOPORT_CREATE_PIN(PORTA,0)
//External power
#define GPIO_POWER_VUSB_SENSE     IOPORT_CREATE_PIN(PORTA,1)
//Power good
#define GPIO_POWER_GOOD           IOPORT_CREATE_PIN(PORTA,5)

#define GPIO_POWER_DCDC_ENABLE    IOPORT_CREATE_PIN(PORTA,6)

//! @}



/*! \name Temp sensor ADC pin number
 */
//! @{
// Use ADC0.
#define ADC			&ADCA

#define GPIO_SENSOR_TEMP	IOPORT_CREATE_PIN(PORTA, 4)
//! @}


/*! \name Sensor interface pin mapping
 */
//! @{
#define GPIO_SENSOR_ADC0          IOPORT_CREATE_PIN(PORTA,2)
#define GPIO_SENSOR_ADC1          IOPORT_CREATE_PIN(PORTA,3)
#define GPIO_SENSOR_DAC0          IOPORT_CREATE_PIN(PORTB,1)
#define GPIO_SENSOR_DAC1          IOPORT_CREATE_PIN(PORTB,2)
#define GPIO_SENSOR_SDA           IOPORT_CREATE_PIN(PORTC,0)
#define GPIO_SENSOR_SCL           IOPORT_CREATE_PIN(PORTC,1)
#define GPIO_SENSOR_CS            IOPORT_CREATE_PIN(PORTE,4)
#define GPIO_SENSOR_MOSI          IOPORT_CREATE_PIN(PORTE,5)
#define GPIO_SENSOR_MISO          IOPORT_CREATE_PIN(PORTE,6)
#define GPIO_SENSOR_SCK           IOPORT_CREATE_PIN(PORTE,7)
#define GPIO_SENSOR_RX            IOPORT_CREATE_PIN(PORTF,2)
#define GPIO_SENSOR_TX            IOPORT_CREATE_PIN(PORTF,3)
//! @}


/*! \name GPIO Connections of LEDs
 */
//! @{
//! Number of LEDs.
#define LED_COUNT   4

#define GPIO_LED_GREEN         IOPORT_CREATE_PIN(PORTB,4)
#define GPIO_LED_RED           IOPORT_CREATE_PIN(PORTB,5)	
#define GPIO_LED_YELLOW        IOPORT_CREATE_PIN(PORTB,6)	
#define GPIO_LED_ORANGE        IOPORT_CREATE_PIN(PORTB,7)
//! @}


/*! \name GPIO Connections of Push Buttons
 */
//! @{
#define GPIO_SWITCH	            IOPORT_CREATE_PIN(PORTB,3)
#define GPIO_SWITCH_INT_VECTOR  PORTB_INT0_vect
//! @}


/*! \name GPIO Connections of Dataflash
 */
//! @{
#define GPIO_DATAFLASH_RESET	 IOPORT_CREATE_PIN(PORTC,4)
#define GPIO_DATAFLASH_CS	     IOPORT_CREATE_PIN(PORTD,4)
#define GPIO_DATAFLASH_MOSI	   IOPORT_CREATE_PIN(PORTD,5)
#define GPIO_DATAFLASH_MISO	   IOPORT_CREATE_PIN(PORTD,6)
#define GPIO_DATAFLASH_SCK	   IOPORT_CREATE_PIN(PORTD,7)
//! @}


/*! \name GPIO Connections of ANT
 */
//! @{
#define CONF_BOARD_ENABLE_ANT
#define ANT_USART			  &USARTD0

#define nRF24AP2_nSUSPEND	  IOPORT_CREATE_PIN(PORTF, 4)
#define nRF24AP2_SLEEP		  IOPORT_CREATE_PIN(PORTF, 5)
#define nRF24AP2_RTS		    IOPORT_CREATE_PIN(PORTF, 6)
#define nRF24AP2_nRESET		  IOPORT_CREATE_PIN(PORTF, 7)
#define nRF24AP2_TX		      IOPORT_CREATE_PIN(PORTD, 2)
#define nRF24AP2_RX		      IOPORT_CREATE_PIN(PORTD, 3)
//! @}


/*! \name GPIO Connections of the nRF8001 Bluetooth Low Energy chipset
 */
//! @{
#define CONF_BOARD_ENABLE_BLE
#define BLE_SPI             &SPIC
#define BLE_SPI_CLK_MASK    SYSCLK_PORT_C

#define BLE_MASTER_MOSI     IOPORT_CREATE_PIN(PORTC,5)  // MOSI as output
#define BLE_MASTER_MISO     IOPORT_CREATE_PIN(PORTC,6)  // MISO as input
#define BLE_MASTER_SCK      IOPORT_CREATE_PIN(PORTC,7)  // SCK as output
#define BLE_ACTIVE          IOPORT_CREATE_PIN(PORTD,0)  // RDYN as input
#define BLE_RESET           IOPORT_CREATE_PIN(PORTD,1)  // RDYN as input
#define BLE_RDYN            IOPORT_CREATE_PIN(PORTE,0)  // RDYN as input
#define BLE_REQN            IOPORT_CREATE_PIN(PORTE,1)  // REQN as output
#define BLE_RDYN_INT_VECTOR PORTE_INT0_vect

//uart ble test interface
#define BLE_USART			      &USARTE0
#define BLE_TXD             IOPORT_CREATE_PIN(PORTE,2)  // RDYN as input
#define BLE_RXD             IOPORT_CREATE_PIN(PORTE,3)  // RDYN as input
//! @}



#endif /* METORMOTE_NODE_H_ */