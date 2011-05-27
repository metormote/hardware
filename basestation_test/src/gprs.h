/*
 * gprs.h
 *
 * Created: 2011-05-13 16:31:09
 *  Author: niklo
 */ 


#ifndef GPRS_H_
#define GPRS_H_

enum status_code gprs_send(uint8_t* data, uint8_t size);
enum status_code gprs_receive(uint8_t* data, uint8_t size);

#endif /* GPRS_H_ */