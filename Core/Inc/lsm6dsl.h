/*
 * lsm6dsl.h
 *
 *  Created on: Nov 12, 2023
 *      Author: 17605
 */

#ifndef LSM6DSL_H_
#define LSM6DSL_H_

#define CTRL1_XL 	0x10
#define INT1_CTRL 	0x0D
#define STATUS_REG 	0x1E

#define READ_ADR 	0b11010101
#define WRITE_ADR	0b11010100

#define OUTX_L_XL	0x28
#define OUTX_H_XL	0x29
#define OUTY_L_XL	0x2A
#define OUTY_H_XL	0x2B
#define OUTZ_L_XL	0x2C
#define OUTZ_H_XL	0x2D

void lsm6dsl_init();
void lsm6dsl_read_xyz(int16_t* x, int16_t* y, int16_t* z);


#endif /* LSM6DSL_H_ */
