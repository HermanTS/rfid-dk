/*
 * systick.h
 *
 *  Created on: 11 апр. 2018 г.
 *      Author: gstsvetkov
 */

#ifndef INCLUDE_SYSTICK_H_
#define INCLUDE_SYSTICK_H_
#include "stdint.h"

void init_tick(void);
uint32_t get_tick(void);
void systick_delay_ms(uint32_t ms);
void SysTick_Handler(void);


#endif /* INCLUDE_SYSTICK_H_ */
