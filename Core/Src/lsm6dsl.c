/*
 * lsm6dsl.c
 *
 *  Created on: Nov 12, 2023
 *      Author: 17605
 */

#include <stm32l475xx.h>
#include "lsm6dsl.h"
#include "i2c.h"
void lsm6dsl_init(){

	// 1. Write CTRL1_XL = 60h // Acc = 416 Hz (High-Performance mode)
	uint8_t data1[2] = {CTRL1_XL, 0x60};
	i2c_transaction(WRITE_ADR, WRITE_I2C, data1, 2);

	// 2. Write INT1_CTRL = 01h // Acc data-ready interrupt on INT1
	uint8_t data2[2] = {INT1_CTRL, 0x01};
	i2c_transaction(WRITE_ADR, WRITE_I2C, data2, 2);
}

void lsm6dsl_read_xyz(int16_t* x, int16_t* y, int16_t* z){

	// Check the status register for available data
	uint8_t status = STATUS_REG;
	uint8_t result = i2c_transaction(READ_ADR, READ_I2C, status, 1);

	// Zero out x y and z
	*x = 0;
	*y = 0;
	*z = 0;

	// read low and high registers for x, y, and z.
	uint8_t register_addr = OUTX_L_XL;
	uint8_t x_low = i2c_transaction(READ_ADR, READ_I2C, &register_addr, 1);
	*x |= x_low;

	register_addr = OUTX_H_XL;
	uint8_t x_high = i2c_transaction(READ_ADR, READ_I2C, &register_addr, 1);
	*x |= x_high << 8;

	register_addr = OUTY_L_XL;
	uint8_t y_low = i2c_transaction(READ_ADR, READ_I2C, &register_addr, 1);
	*y |= y_low;

	register_addr = OUTY_H_XL;
	uint8_t y_high = i2c_transaction(READ_ADR, READ_I2C, &register_addr, 1);
	*y |= y_high << 8;

	register_addr = OUTZ_L_XL;
	uint8_t z_low = i2c_transaction(READ_ADR, READ_I2C, &register_addr, 1);
	*z |= z_low;

	register_addr = OUTZ_H_XL;
	uint8_t z_high = i2c_transaction(READ_ADR, READ_I2C, &register_addr, 1);
	*z |= z_high << 8;
}
