/*
 * ili9341.c
 *
 *  Created on: 27 мар. 2018 г.
 *      Author: gstsvetkov
 */

#include "board.h"
#include "ili9341.h"

void ili9341_rst_set_state(uint8_t state){
        if (state == SET)
           ILI9341_GPIO->BSRR |= ILI9341_PIN_RESET;
       else
           ILI9341_GPIO->BSRR |= (ILI9341_PIN_RESET << 16);
}

void ili9341_cs_set_state(uint8_t state){
        if (state == SET)
            ILI9341_GPIO->BSRR |= ILI9341_PIN_CS;
        else
            ILI9341_GPIO->BSRR |= (ILI9341_PIN_CS << 16);
}

void ili9341_dc_set_state(uint8_t state){
        if (state == SET)
            ILI9341_GPIO->BSRR |= ILI9341_PIN_DC;
        else
            ILI9341_GPIO->BSRR |= (ILI9341_PIN_DC << 16);
}

void ili9341_send_command(uint8_t command){
        ili9341_dc_set_state(RESET);
        ili9341_cs_set_state(RESET);

        spi_send_byte(ILI9341_SPI, command);

        ili9341_cs_set_state(SET);
}


void ili9341_send_data(uint8_t data){
        ili9341_dc_set_state(SET);
        ili9341_cs_set_state(RESET);

        spi_send_byte(ILI9341_SPI, data);

        ili9341_cs_set_state(SET);
}

void LCD_Init(){
  LCD_InitGPIO();

  ili9341_cs_set_state(SET);

  // сброс дисплея
  ili9341_rst_set_state(SET);
  ili9341_send_command(ILI9341_CMD_SOFT_RESET);
  Delay(100);

  /// commands here
  ili9341_send_command(ILI9341_POWERA);
  ili9341_send_data(0x39);
  ili9341_send_data(0x2C);
  ili9341_send_data(0x00);
  ili9341_send_data(0x34);
  ili9341_send_data(0x02);
  ili9341_send_command(ILI9341_POWERB);
  ili9341_send_data(0x00);
  ili9341_send_data(0xC1);
  ili9341_send_data(0x30);
  ili9341_send_command(ILI9341_DTCA);
  ili9341_send_data(0x85);
  ili9341_send_data(0x00);
  ili9341_send_data(0x78);
  ili9341_send_command(ILI9341_DTCB);
  ili9341_send_data(0x00);
  ili9341_send_data(0x00);
  ili9341_send_command(ILI9341_POWER_SEQ);
  ili9341_send_data(0x64);
  ili9341_send_data(0x03);
  ili9341_send_data(0x12);
  ili9341_send_data(0x81);
  ili9341_send_command(ILI9341_PRC);
  ili9341_send_data(0x20);
  ili9341_send_command(ILI9341_POWER1);
  ili9341_send_data(0x23);
  ili9341_send_command(ILI9341_POWER2);
  ili9341_send_data(0x10);
  ili9341_send_command(ILI9341_VCOM1);
  ili9341_send_data(0x3E);
  ili9341_send_data(0x28);
  ili9341_send_command(ILI9341_VCOM2);
  ili9341_send_data(0x86);
  ili9341_send_command(ILI9341_MAC);
  ili9341_send_data(0x48); //brg | mx
  ili9341_send_command(ILI9341_PIXEL_FORMAT);
  ili9341_send_data(0x55);
  ili9341_send_command(ILI9341_FRC);
  ili9341_send_data(0x00);
  ili9341_send_data(0x18);
  ili9341_send_command(ILI9341_DFC);
  ili9341_send_data(0x08);
  ili9341_send_data(0x82);
  ili9341_send_data(0x27);
  ili9341_send_command(ILI9341_3GAMMA_EN);
  ili9341_send_data(0x00);
  ili9341_send_command(ILI9341_COLUMN_ADDR);
  ili9341_send_data(0x00);
  ili9341_send_data(0x00);
  ili9341_send_data(0x00);
  ili9341_send_data(0xEF);
  ili9341_send_command(ILI9341_PAGE_ADDR);
  ili9341_send_data(0x00);
  ili9341_send_data(0x00);
  ili9341_send_data(0x01);
  ili9341_send_data(0x3F);
  ili9341_send_command(ILI9341_GAMMA);
  ili9341_send_data(0x01);
  ili9341_send_command(ILI9341_PGAMMA);
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
  ili9341_send_command(ILI9341_NGAMMA);
  ili9341_send_data(0x00);
  ili9341_send_data(0x0E);
  ili9341_send_data(0x14);
  ili9341_send_data(0x03);
  ili9341_send_data(0x11);
  ili9341_send_data(0x07);
  ili9341_send_data(0x31);
  ili9341_send_data(0xC1);
  ili9341_send_data(0x48);
  ili9341_send_data(0x08);
  ili9341_send_data(0x0F);
  ili9341_send_data(0x0C);
  ili9341_send_data(0x31);
  ili9341_send_data(0x36);
  ili9341_send_data(0x0F);
  ili9341_send_command(ILI9341_SLEEP_OUT);

  Delay(100);
  ili9341_send_command(ILI9341_DISPLAY_ON);
  ili9341_send_command(ILI9341_GRAM);
}
