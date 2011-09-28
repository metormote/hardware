/*
 * sensors.h
 *
 * Created: 2011-05-27 09:56:17
 *  Author: niklo
 */ 

#include <asf.h>

#ifndef SENSORS_H_
#define SENSORS_H_


int8_t sensors_init(void);

int8_t sensors_read_vbat(float* value);
int8_t sensors_read_vusb(float* value);
int8_t sensors_read_temp(float* value);

#endif /* SENSORS_H_ */