// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdint.h>
#include "stm32f103xb.h"
#include "system_stm32f1xx.h"
#include "systick.h"
#include "ili9341.h"
#include "board.h"
#include "spi.h"

// ----- main() ---------------------------------------------------------------
void main()
{
    //init system timer
    init_tick();
    //Priferial initialization
    spi1_gpio_init();
    spi1_periph_init();
    //init lcd
    ili9341_gpio_init();
    ili9341_init();
    //switch on light
    ili9341_led_set_state(SET);
    ili9341_fill_screen(yellow);


    while (1)
    {

    }
}
