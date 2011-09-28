/*
 * debug.h
 *
 * Created: 2011-05-13 10:56:38
 *  Author: niklo
 */ 

#include <asf.h>


#ifndef DEBUG_H_
#define DEBUG_H_

void debug_print(char* c);
void debug_wait_for_step(void);
void debug_red_led(uint8_t state);
void debug_green_led(uint8_t state);

#endif /* DEBUG_H_ */