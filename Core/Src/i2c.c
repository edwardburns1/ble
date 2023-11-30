/*
 * i2c.c
 *
 *  Created on: Nov 5, 2023
 *      Author: 17605
 */


#include <stm32l475xx.h>
#include "i2c.h"
void i2c_init(){


	// Turn on HSI 16 MHz clock
	RCC->CR |= RCC_CR_HSION;
	while (!(RCC->CR & RCC_CR_HSIRDY));

	// Use HSI16 for my I2C
	RCC->CCIPR |= RCC_CCIPR_I2C2SEL_1;

	// Enable I2C2 and GPIOB
	RCC->APB1ENR1 |= RCC_APB1ENR1_I2C2EN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	// Configure GPIO PB10 and PB11 to be in Alternate Function Mode
	GPIOB->MODER &= ~GPIO_MODER_MODE10_Msk;
	GPIOB->MODER |= GPIO_MODER_MODE10_1;

	GPIOB->MODER &= ~GPIO_MODER_MODE11_Msk;
	GPIOB->MODER |= GPIO_MODER_MODE11_1;

	// Push Pull
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT10;
	GPIOB->OTYPER &= ~GPIO_OTYPER_OT11;

	GPIOB->OTYPER |= GPIO_OTYPER_OT10 | GPIO_OTYPER_OT11; // Set to open-drain


	// Set PB10 and PB11 to Pull up
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD10_0;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD11_0;

	/* Configure the GPIO to use low speed mode */
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED10_1;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED11_1;

	// Configure PB10 and PB11 to SCL and SDA for I2C2
	GPIOB->AFR[1] |= 4 << GPIO_AFRH_AFSEL10_Pos;
	GPIOB->AFR[1] |= 4 << GPIO_AFRH_AFSEL11_Pos;

	// I2C Configuration Stage, per the stm32 manual

	// Step 1
	// Clear PE bit in I2C_CR1
	I2C2->CR1 &= ~I2C_CR1_PE;

	// Step 2
	// Configure ANFOFF and DNF[3:0] in I2C_CR1
	// will leave the  defaults
	I2C2->CR1;

	// Step 3
	// Configure PRESC[3:0],
	// SDADEL[3:0], SCLDEL[3:0], SCLH[7:0],
	// SCLL[7:0] in I2C_TIMINGR

	uint32_t PRESC = 3;
	uint32_t SCLL = 0x13;
	uint32_t SCLH = 0xF;
	uint32_t SDADEL = 0x2;
	uint32_t SCLDEL = 0x4;

	I2C2->TIMINGR &= ~(I2C_TIMINGR_PRESC_Msk | I2C_TIMINGR_SCLDEL_Msk | I2C_TIMINGR_SDADEL_Msk | I2C_TIMINGR_SCLH_Msk | I2C_TIMINGR_SCLL_Msk);
	I2C2->TIMINGR |= (PRESC << I2C_TIMINGR_PRESC_Pos) | (SDADEL << I2C_TIMINGR_SDADEL_Pos) | (SCLDEL << I2C_TIMINGR_SCLDEL_Pos) | (SCLH << I2C_TIMINGR_SCLH_Pos) | (SCLL << I2C_TIMINGR_SCLL_Pos);



	// Step 4
	// Configure NOSTRETCH in I2C_CR1
	I2C2->CR1 &= ~I2C_CR1_NOSTRETCH;



	// Step 5
	// Set PE bit in I2C_CR1, enabling I2C

	I2C2->CR1 |= I2C_CR1_PE;

}

uint8_t i2c_transaction(uint8_t address, uint8_t dir, uint8_t* data, uint8_t len){


	// READ vs WRITE gate
	if(dir){
		return read_transaction(address, data, len);
	}
	else{
		return write_transaction(address, data, len);
	}




}

uint8_t read_transaction(uint8_t address, uint8_t* data, uint8_t len){

	// All reads require a register write, len = 1
	write_transaction(address, data, 1);

	uint32_t NBYTES = len << I2C_CR2_NBYTES_Pos;
	uint32_t RD_WRN = 1 << I2C_CR2_RD_WRN_Pos;
	uint32_t AUTOEND = I2C_CR2_AUTOEND;

	I2C2->ICR |= I2C_ICR_STOPCF;

	// Send another START signaling we want to read now
	address_phase(address, NBYTES, RD_WRN, AUTOEND);

	// Wait until there's something to read
	while(!(I2C2->ISR & I2C_ISR_RXNE));
	uint8_t value = I2C2->RXDR;

	return value;
}

uint8_t write_transaction(uint8_t address, uint8_t* data, uint8_t len){

	uint32_t NBYTES = len << I2C_CR2_NBYTES_Pos;
	uint32_t RD_WRN = 0 << I2C_CR2_RD_WRN_Pos;
	uint32_t AUTOEND = I2C_CR2_AUTOEND;


	// Send the address over I2C, and configure CR2 bits
	address_phase(address, NBYTES, RD_WRN, AUTOEND);


	// Write len bytes over I2C
	// Note: The first iteration writes the register address
	for(uint8_t i = 0; i < len; i++){
		// Wait until TXIS is 1
		while(!(I2C2->ISR & I2C_ISR_TXIS));

		// Write to Transmitter Register
		I2C2->TXDR = *(data)++;
	}
	return 0x00;

}

// Configures CR2 and sends the slave address over i2c
void address_phase(uint8_t slave_address, uint32_t NBYTES, uint32_t RD_WRN, uint32_t AUTOEND){
	I2C2->CR2 = I2C_CR2_START | slave_address | NBYTES | RD_WRN | AUTOEND;
}


