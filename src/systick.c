/*
 * systick.c
 *
 *  Created on: 11 апр. 2018 г.
 *      Author: gstsvetkov
 */
#include "systick.h"
#include "stdint.h"
#include "stm32f103xb.h"

volatile uint32_t tick;

// Configure Systick to 1ms
void init_tick(void)
{
        SysTick_Config(SystemCoreClock/1000);
}

uint32_t get_tick(void)
{
        return tick;
}

void systick_delay_ms(uint32_t ms)
{
        uint32_t initial_tick = get_tick();

        while (get_tick() - initial_tick < ms)
                ;
}

//  This function handles SysTick Handler.
void SysTick_Handler(void)
{
        tick++;
}
