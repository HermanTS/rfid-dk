// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdint.h>
#include "stm32f103xb.h"
#include "board.h"
#include "spi.h"

// ----- main() ---------------------------------------------------------------
void main()
{
    spi1_gpio_init();
    spi1_periph_init();

    while (1)
    {
        uint8_t temp;
        spi_send_byte(SPI1, 0xda);

        temp = spi_read_byte(SPI1);
        if (temp == 0xda)
        {
            temp++;
            break;
        }
    }
}
