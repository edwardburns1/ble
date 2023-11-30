/*
 * timer.c
 *
 *  Created on: Oct 5, 2023
 *      Author: schulman
 */

#include "timer.h"


void timer_init(TIM_TypeDef* timer)
{
  // TODO implement this

	// 1 Stop timer, clear out timer state, reset all counters
//TIM2_BASE

	switch( (uint32_t) timer){
	case TIM2_BASE:
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
		// External Interrupt Enable
		NVIC_EnableIRQ(TIM2_IRQn);
		NVIC_SetPriority(TIM2_IRQn, 1); // Maybe priority is different?
		break;
	case TIM3_BASE:
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN;
		// External Interrupt Enable
		NVIC_EnableIRQ(TIM3_IRQn);
		NVIC_SetPriority(TIM3_IRQn, 0); // Maybe priority is different?
		break;
	case TIM5_BASE:
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
		NVIC_EnableIRQ(TIM5_IRQn);
		NVIC_SetPriority(TIM5_IRQn, 5);
		break; // Maybe priority is different?
	default:
		return;
	}


	// Stop Counter + Reset state
	timer->CR1 = 0;
	// Reset counter
	timer->CNT = 0;

	// Auto Reload at uint32 max
	timer->ARR = 0xFFFFFFFF;

	// Divide by PSC +1
	timer->PSC = 7;

	timer->EGR |= TIM_EGR_UG;
	timer->SR = 0;






	// Internal Update Interrupt Enable
	timer->DIER |= TIM_DIER_UIE_Msk;
	// Enable timer
	timer->CR1 |= TIM_CR1_CEN_Msk;

}

void timer_reset(TIM_TypeDef* timer)
{
  // TODO implement this
	timer->CNT = 0;
}

void timer_set_ms(TIM_TypeDef* timer, uint16_t period_ms)
{
  // TODO implement this

	// Current clock rate = 1MHz
	// Period = 1 us
	// Convert to ms by waiting 1000 clock cycles

	// Times 2 because of System clock config for BLE
	timer->ARR = 1000 * period_ms;
}

