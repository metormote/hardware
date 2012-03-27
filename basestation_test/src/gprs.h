/*
 * gprs.h
 *
 * Created: 2011-05-13 16:31:09
 *  Author: niklo
 */ 


#ifndef GPRS_H_
#define GPRS_H_

int8_t gprs_init(void);

int8_t gprs_send(uint8_t* data, uint8_t size);
int8_t gprs_receive(uint8_t* data, uint8_t size);

int8_t gprs_reset(void);
int8_t gprs_on(void);

#endif /* GPRS_H_ */