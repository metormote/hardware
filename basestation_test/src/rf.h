/*
 * rf.h
 *
 * Created: 2011-05-13 16:30:34
 *  Author: niklo
 */ 

#include <asf.h>

#ifndef RF_H_
#define RF_H_

enum status_code rf_init(void);

enum status_code rf_send(uint8_t* data, uint8_t size);
enum status_code rf_receive(uint8_t* data, uint8_t size);

enum status_code rf_sleep(uint8_t state);
enum status_code rf_suspend(uint8_t state);

#endif /* RF_H_ */