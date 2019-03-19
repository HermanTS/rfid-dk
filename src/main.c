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

    //test functions
    ili9341_fill_screen(yellow);
    ili9341_draw_point(50, 100, red, 25);
    ili9341_draw_pixel(ILI9341_WIDTH/2, ILI9341_HEIGHT/2, red);
    ili9341_draw_line(50, 150, 200, 300, black, 1);
    ili9341_draw_line(150, 50, 200, 300, black, 3);
    ili9341_draw_line(200, 200, 150, 200, red, 4);
    ili9341_draw_line(0, ILI9341_WIDTH, ILI9341_HEIGHT, ILI9341_HEIGHT, white, 4);


    while (1)
    {

    }
}
