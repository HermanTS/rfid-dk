// ----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdint.h>
#include "board.h"
#include "spi.h"

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

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
            break;
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
