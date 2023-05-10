/*
 * I2C2.h
 *
 *  Created on: 15 ���. 2018 �.
 *      Author: vlinc
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f4xx.h"



uint8_t hal_write_i2c(uint8_t slave_addr, uint8_t addr, uint8_t data);
uint8_t hal_read_i2c(uint8_t slave_addr, uint8_t addr, uint8_t size, uint8_t *puch_data);

void write_i2c2(uint8_t slave_addr, uint8_t addr, uint8_t size, uint8_t *data);
void read_i2c2(uint8_t slave_addr, uint8_t addr, uint8_t size, uint8_t *data);


#endif /* I2C_H_ */




