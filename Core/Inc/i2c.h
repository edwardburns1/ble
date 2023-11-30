/*
 * ic2.h
 *
 *  Created on: Nov 5, 2023
 *      Author: 17605
 */

#ifndef I2C_H_
#define I2C_H_

#define WRITE_I2C 0
#define READ_I2C  1
void i2c_init();

uint8_t i2c_transaction(uint8_t address, uint8_t dir, uint8_t* data, uint8_t len);

uint8_t write_transaction(uint8_t address, uint8_t* data, uint8_t len);

void address_phase(uint8_t slave_address, uint32_t NBYTES, uint32_t RD_WRN, uint32_t AUTOEND);

uint8_t read_transaction(uint8_t address, uint8_t* data, uint8_t len);

#endif /* I2C_H_ */
