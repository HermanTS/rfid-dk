// ----------------------------------------------------------------------------

#include <stdlib.h>
#include "stdint.h"
#include "board.h"
#include "spi.h"
#include "rfid-rc522.h"
// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void main()
{
uint8_t temp = 0;
        spi1_gpio_init();
        spi1_periph_init();


        while(1){

                spi_send_byte(SPI1, 0xfe);

                temp = spi_read_byte(SPI1);

                if (temp == 0xfe)
                    temp = !temp;

        }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
