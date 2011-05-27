/*
 * rf.h
 *
 * Created: 2011-05-13 16:30:34
 *  Author: niklo
 */ 

#include <asf.h>

#ifndef RF_H_
#define RF_H_

typedef struct _anthdr
{
	uint8_t sync;
	uint8_t len;
	uint8_t id;
}ANTHDR;

int8_t rf_init(void);

int8_t rf_send(ANTHDR* hdr, uint8_t* data);
int8_t rf_receive(ANTHDR* hdr, uint8_t* data);

int8_t rf_reset(uint8_t state);
int8_t rf_sleep(uint8_t state);
int8_t rf_suspend(uint8_t state);

#endif /* RF_H_ */