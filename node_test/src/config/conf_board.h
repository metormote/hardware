/**
 * \file
 *
 * \brief User board configuration template
 *
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

/* The difference, measured in seconds, between the indicated 
 * time and midnight, January 1, 1970 UTC.
 * For easy compatibility with Java-time standard.
 * 5 May 2011 1800 EST
 */
#define TIME_ZERO 1304632800

// Enable USARTD0 - rf
#define CONF_BOARD_ENABLE_USARTD0

// Enable USARTE0 - debug
#define CONF_BOARD_ENABLE_USARTE0

#endif // CONF_BOARD_H
