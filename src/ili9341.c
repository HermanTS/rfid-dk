/*
 * ili9341.c
 *
 *  Created on: 27 мар. 2018 г.
 *      Author: gstsvetkov
 */
#include "stm32f103xb.h"
#include "ili9341.h"
#include "spi.h"
#include "board.h"
#include "systick.h"
#include "symbol-table.h"

uint32_t i;

void ili9341_led_set_state(uint8_t state){
#ifdef ILI9341_PIN_LED
        if (state == SET)
            ILI9341_GPIO->BSRR |= ILI9341_PIN_LED;
        else
            ILI9341_GPIO->BRR |= ILI9341_PIN_LED;
#else
        return;
#endif
}

static void ili9341_reset()
{
           ILI9341_GPIO->BRR |= ILI9341_PIN_RESET;
           systick_delay_ms(50);
           ILI9341_GPIO->BSRR |= ILI9341_PIN_RESET;
           systick_delay_ms(200);
}

static void ili9341_cs_set_state(uint8_t state)
{
        static uint8_t prev_state = SET;

        if (prev_state != state)
        {
            while ((ILI9341_SPI->SR & SPI_SR_BSY) == SPI_SR_BSY)
                ;
            if (state == SET)
                ILI9341_GPIO->BSRR |= ILI9341_PIN_CS;
            else
                ILI9341_GPIO->BRR |= ILI9341_PIN_CS;
            prev_state = state;
        }
}

static void ili9341_dc_set_state(uint8_t state)
{
        static uint8_t prev_state = RESET;

        if (prev_state != state)
        {
            while ((ILI9341_SPI->SR & SPI_SR_BSY) == SPI_SR_BSY)
                ;
            if (state == SET)
                ILI9341_GPIO->BSRR |= ILI9341_PIN_DC;
            else
                ILI9341_GPIO->BRR |= ILI9341_PIN_DC;
            prev_state = state;
        }
}

static void ili9341_send_command(uint8_t command)
{
        //ili9341_cs_set_state(RESET);
        ili9341_dc_set_state(RESET);
        spi_send_byte(ILI9341_SPI, command);

        //ili9341_cs_set_state(SET);
}


static void ili9341_send_data(uint8_t data)
{
        //ili9341_cs_set_state(RESET);
        ili9341_dc_set_state(SET);
        spi_send_byte(ILI9341_SPI, data);

        //ili9341_cs_set_state(SET);
}

void ili9341_gpio_init()
{
/*
 * function of initialisation of CS, DC, RESET pins
 * CS - SPI chip select
 * DC - select data or command in packet, 1 - data, 0 - command
 * RESET - active low, reset device
 */


        // chip select CS: output, 50MHz
#if (ILI9341_PIN_CS_NO < 8)
        ILI9341_GPIO->CRL   |=  (3 << (ILI9341_PIN_CS_NO*4));
        ILI9341_GPIO->CRL   &= ~(0xC << (ILI9341_PIN_CS_NO*4));
#else
        ILI9341_GPIO->CRH   |=  (3 << ((ILI9341_PIN_CS_NO - 8)*4));
        ILI9341_GPIO->CRH   &= ~(0xC << ((ILI9341_PIN_CS_NO - 8)*4));
#endif
        // data select DC: output, 50MHz
#if (ILI9341_PIN_DC_NO < 8)
        ILI9341_GPIO->CRL   |=  (3 << (ILI9341_PIN_DC_NO*4));
        ILI9341_GPIO->CRL   &= ~(0xC << (ILI9341_PIN_DC_NO*4));
#else
        ILI9341_GPIO->CRH   |=  (3 << ((ILI9341_PIN_DC_NO - 8)*4));
        ILI9341_GPIO->CRH   &= ~(0xC << ((ILI9341_PIN_DC_NO - 8)*4));
#endif
        // RESET: output, 50MHz
#if (ILI9341_PIN_RESET_NO < 8)
        ILI9341_GPIO->CRL   |=  (3 << (ILI9341_PIN_RESET_NO*4));
        ILI9341_GPIO->CRL   &= ~(0xC << (ILI9341_PIN_RESET_NO*4));
#else
        ILI9341_GPIO->CRH   |=  (3 << ((ILI9341_PIN_RESET_NO - 8)*4));
        ILI9341_GPIO->CRH   &= ~(0xC << ((ILI9341_PIN_RESET_NO - 8)*4));
#endif
#ifdef ILI9341_PIN_LED
#if (ILI9341_PIN_LED_NO < 8)
        ILI9341_GPIO->CRL   |=  (3 << (ILI9341_PIN_LED_NO*4));
        ILI9341_GPIO->CRL   &= ~(0xC << (ILI9341_PIN_LED_NO*4));
#else
        ILI9341_GPIO->CRH   |=  (3 << ((ILI9341_PIN_LED_NO - 8)*4));
        ILI9341_GPIO->CRH   &= ~(0xC << ((ILI9341_PIN_LED_NO - 8)*4));
#endif
        ILI9341_GPIO->BRR |= ILI9341_PIN_LED;
#endif
        // set defaul values
        ILI9341_GPIO->BSRR |= ILI9341_PIN_CS;
        ILI9341_GPIO->BRR |= ILI9341_PIN_DC;
        ILI9341_GPIO->BRR |= ILI9341_PIN_RESET;
}

void ili9341_init()
{

        // reset lcd
        ili9341_reset();

        ili9341_cs_set_state(RESET);
        /// commands here
//        ili9341_send_command(0xEF);
//        ili9341_send_data(0x03);
//        ili9341_send_data(0x80);
//        ili9341_send_data(0x02);

        ili9341_send_command(ILI9341_CMD_POWER_CTRLA);
        ili9341_send_data(0x39);
        ili9341_send_data(0x2C);
        ili9341_send_data(0x00);
        ili9341_send_data(0x34);
        ili9341_send_data(0x02);
        ili9341_send_command(ILI9341_CMD_POWER_CTRLB);
        ili9341_send_data(0x00);
        ili9341_send_data(0xC1);
        ili9341_send_data(0x30);

        ili9341_send_command(ILI9341_CMD_TIMER_CTRLA_1);
        ili9341_send_data(0x85);
        ili9341_send_data(0x00);
        ili9341_send_data(0x78);
        ili9341_send_command(ILI9341_CMD_TIMER_CTRLB);
        ili9341_send_data(0x00);
        ili9341_send_data(0x00);

        ili9341_send_command(ILI9341_CMD_POWER_SEQ_CTRL);
        ili9341_send_data(0x64);
        ili9341_send_data(0x03);
        ili9341_send_data(0x12);
        ili9341_send_data(0x81);

        ili9341_send_command(ILI9341_CMD_PUMP_RATIO_CTRL);
        ili9341_send_data(0x20);

        ili9341_send_command(ILI9341_CMD_POWER_CTRL1);
        ili9341_send_data(0x23);
        ili9341_send_command(ILI9341_CMD_POWER_CTRL2);
        ili9341_send_data(0x10);

        ili9341_send_command(ILI9341_CMD_VCOM_CTRL1);
        ili9341_send_data(0x3E);
        ili9341_send_data(0x28);
        ili9341_send_command(ILI9341_CMD_VCOM_CTRL2);
        ili9341_send_data(0x86);

        ili9341_send_command(ILI9341_CMD_MEM_ACCESS_CTRL);
        ili9341_send_data(0x48);  //my,mx,mv,ml,BGR,mh,0.0
        ili9341_send_command(ILI9341_CMD_SET_PIXEL_FORMAT);
        ili9341_send_data(0x55);  //16 pit per pixel

        ili9341_send_command(ILI9341_CMD_FRAME_CTRL_NM);
        ili9341_send_data(0x00);
        ili9341_send_data(0x18);

        ili9341_send_command(ILI9341_CMD_DISPAY_FUNC_CTRL);
        ili9341_send_data(0x08);
        ili9341_send_data(0x82);
        ili9341_send_data(0x27);

        ili9341_send_command(ILI9341_CMD_GAMMA3_CTRL);
        ili9341_send_data(0x00);  // 3Gamma Function Disable

        ili9341_send_command(ILI9341_CMD_GAMMA_SET);
        ili9341_send_data(0x01);  // gamma set 4 gamma curve 01/02/04/08

        ili9341_send_command(ILI9341_CMD_POSITIV_GAMMA_CORR);
        ili9341_send_data(0x0F);
        ili9341_send_data(0x31);
        ili9341_send_data(0x2B);
        ili9341_send_data(0x0C);
        ili9341_send_data(0x0E);
        ili9341_send_data(0x08);
        ili9341_send_data(0x4E);
        ili9341_send_data(0xF1);
        ili9341_send_data(0x37);
        ili9341_send_data(0x07);
        ili9341_send_data(0x10);
        ili9341_send_data(0x03);
        ili9341_send_data(0x0E);
        ili9341_send_data(0x09);
        ili9341_send_data(0x00);

        ili9341_send_command(ILI9341_CMD_NEGATIV_GAMMA_CORR);
        ili9341_send_data(0x00);
        ili9341_send_data(0x25);
        ili9341_send_data(0x27);
        ili9341_send_data(0x05);
        ili9341_send_data(0x10);
        ili9341_send_data(0x09);
        ili9341_send_data(0x3a);
        ili9341_send_data(0x78);
        ili9341_send_data(0x4d);
        ili9341_send_data(0x05);
        ili9341_send_data(0x18);
        ili9341_send_data(0x0d);
        ili9341_send_data(0x38);
        ili9341_send_data(0x3a);
        ili9341_send_data(0x1F);

        ili9341_send_command(ILI9341_CMD_SLEEP_MODE_OFF);
        systick_delay_ms(150);
        ili9341_send_command(ILI9341_CMD_DISPLAY_ON);
        ili9341_cs_set_state(SET);
}


uint8_t ili9341_read_byte(uint8_t cmd)
{
        uint8_t byte;
        ili9341_cs_set_state(RESET);
        ili9341_dc_set_state(RESET);

        spi_send_read(ILI9341_SPI, cmd);
        byte = spi_read_byte(ILI9341_SPI);
        return byte;
}

uint32_t ili9341_read_ID(void)
{
        uint8_t id_buf[4];
        uint32_t ID = 0;

        // reset lcd
        ili9341_reset();

        ili9341_cs_set_state(RESET);
        ili9341_dc_set_state(RESET);
        id_buf[0] = spi_send_read(ILI9341_SPI, ILI9341_CMD_RD_ID);
        for(i=1; i<=3; i++)
        {
            ili9341_dc_set_state(SET);
            id_buf[i] = spi_send_read(ILI9341_SPI, ILI9341_CMD_NOP);
        }
        ili9341_cs_set_state(SET);

        ID = (id_buf[1]|
              id_buf[2] << 8 |
              id_buf[3] << 8);
        return ID;
}

static void ili9341_set_cursor_position(uint16_t x_max, uint16_t x_min,
                                 uint16_t y_max, uint16_t y_min)
{

        ili9341_send_command(ILI9341_CMD_SET_COLLUM_ADDR);
        ili9341_send_data((uint8_t)(x_min >> 8));
        ili9341_send_data((uint8_t)(x_min & 0x00FF));
        ili9341_send_data((uint8_t)(x_max >> 8));
        ili9341_send_data((uint8_t)(x_max & 0x00FF));

        ili9341_send_command(ILI9341_CMD_SET_PAGE_ADDR);
        ili9341_send_data((uint8_t)(y_min >> 8));
        ili9341_send_data((uint8_t)(y_min & 0x00FF));
        ili9341_send_data((uint8_t)(y_max >> 8));
        ili9341_send_data((uint8_t)(y_max & 0x00FF));
}

void ili9341_set_color(color_t color)
{
        ili9341_send_data((uint8_t)(color >> 8));
        ili9341_send_data((uint8_t)(color & 0x00FF));
}

void ili9341_fill_screen(color_t color)
{
        ili9341_cs_set_state(RESET);
        ili9341_set_cursor_position(ILI9341_WIDTH - 1, 0, ILI9341_HEIGHT -1, 0);

        ili9341_send_command(ILI9341_CMD_WRITE_MEM);
        for (i = 0; i < ILI9341_PIXEL_COUNT; i++)
        {
            ili9341_set_color(color);
        }
        ili9341_cs_set_state(SET);
}

void ili9341_draw_pixel(uint16_t x, uint16_t y, color_t color)
{

        ili9341_set_cursor_position(x, x, y, y);

        ili9341_send_command(ILI9341_CMD_WRITE_MEM);
        ili9341_set_color(color);
}

/*
 * x, y coordinates is start of point
 */
void ili9341_draw_point(uint16_t x, uint16_t y, color_t color, uint8_t size)
{
        ili9341_cs_set_state(RESET);
        ili9341_set_cursor_position(x, x - size, y, y - size);

        ili9341_send_command(ILI9341_CMD_WRITE_MEM);
        for (uint8_t i = 0; i < size*size; i++)
        {
            ili9341_set_color(color);
        }
        ili9341_cs_set_state(SET);
}

void ili9341_draw_simbol(uint16_t x, uint16_t y, color_t color, color_t phone, const uint8_t *sim)
{
        ili9341_cs_set_state(RESET);
        ili9341_set_cursor_position(x, x - 8, y, y - 8);


        ili9341_cs_set_state(SET);
}

void ili9341_draw_line(uint16_t x, uint16_t y, color_t color, uint32_t line_size)
{

        ili9341_set_cursor_position(x, x, y, y);

        ili9341_send_command(ILI9341_CMD_WRITE_MEM);
        for (i = 0; i < line_size; i++)
        {
            ili9341_set_color(color);
        }
}

void ili9341_draw_sqare(uint16_t x_top, uint16_t x_bottom,
                        uint16_t y_right, uint16_t y_left,
                        uint16_t border_px, color_t color)
{
        uint32_t pixels_in_line;

        // firstly print top line
        pixels_in_line = (y_right - y_left) * border_px;
        ili9341_set_cursor_position(x_top, (x_top - border_px), y_right, y_left);

        ili9341_send_command(ILI9341_CMD_WRITE_MEM);
        for (i = 0; i < pixels_in_line; i++)
        {
                ili9341_set_color(color);
        }

        // now print bottom line
        ili9341_set_cursor_position((x_bottom + border_px), x_bottom, y_right, y_left);

        ili9341_send_command(ILI9341_CMD_WRITE_MEM);
        for (i = 0; i < pixels_in_line; i++)
        {
                ili9341_set_color(color);
        }

        // print left line'
        pixels_in_line = (x_top - x_bottom) * border_px;
        ili9341_set_cursor_position(x_top, x_bottom, y_left, (y_left - border_px));

        ili9341_send_command(ILI9341_CMD_WRITE_MEM);
        for (i = 0; i < pixels_in_line; i++)
        {
                ili9341_set_color(color);
        }

        // print right line
        ili9341_set_cursor_position(x_top, x_bottom, (y_right + border_px), y_right);

        ili9341_send_command(ILI9341_CMD_WRITE_MEM);
        for (i = 0; i < pixels_in_line; i++)
        {
                ili9341_set_color(color);
        }
}

/*void ili9341_put_char(uint16_t x, uint16_t y, uint8_t character, color_t color)
{

}
*/
