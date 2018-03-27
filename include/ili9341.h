/*
 * ili9341.h
 *
 *  Created on: 27 мар. 2018 г.
 *      Author: gstsvetkov
 */

#ifndef INCLUDE_ILI9341_H_
#define INCLUDE_ILI9341_H_

#include "stm32f10x.h"


#if  !defined(SET) || !defined(RESET)
#define SET             (uint8_t)0x01
#define RESET           !SET
#endif

#ifndef ILI9341_GPIO
#define ILI9341_GPIO                    GPIOA
#define ILI9341_PIN_RESET               GPIO_Pin_2
#define ILI9341_PIN_DC                  GPIO_Pin_3
#define ILI9341_PIN_CS                  GPIO_Pin_4
#endif

#ifndef ILI9341_SPI
#define ILI9341_SPI                     SPI1
#endif

#ifndef ILI9341_SCREEN_SIZE
#define ILI9341_WIDTH       240
#define ILI9341_HEIGHT      320
#define ILI9341_PIXEL_COUNT ILI9341_WIDTH * LCD_HEIGHT
#endif

//Commands
#define ILI9341_RESET                           0x01
#define ILI9341_SLEEP_OUT                       0x11
#define ILI9341_GAMMA                           0x26
#define ILI9341_DISPLAY_OFF                     0x28
#define ILI9341_DISPLAY_ON                      0x29
#define ILI9341_COLUMN_ADDR                     0x2A
#define ILI9341_PAGE_ADDR                       0x2B
#define ILI9341_GRAM                            0x2C
#define ILI9341_MAC                             0x36
#define ILI9341_PIXEL_FORMAT                    0x3A
#define ILI9341_WDB                             0x51
#define ILI9341_WCD                             0x53
#define ILI9341_RGB_INTERFACE                   0xB0
#define ILI9341_FRC                             0xB1
#define ILI9341_BPC                             0xB5
#define ILI9341_DFC                             0xB6
#define ILI9341_POWER1                          0xC0
#define ILI9341_POWER2                          0xC1
#define ILI9341_VCOM1                           0xC5
#define ILI9341_VCOM2                           0xC7
#define ILI9341_POWERA                          0xCB
#define ILI9341_POWERB                          0xCF
#define ILI9341_PGAMMA                          0xE0
#define ILI9341_NGAMMA                          0xE1
#define ILI9341_DTCA                            0xE8
#define ILI9341_DTCB                            0xEA
#define ILI9341_POWER_SEQ                       0xED
#define ILI9341_3GAMMA_EN                       0xF2
#define ILI9341_INTERFACE                       0xF6
#define ILI9341_PRC                                 0xF7

//commands

#define ILI9341_CMD_NOP                         0x00
#define ILI9341_CMD_SOFT_RESET                  0x01
#define ILI9341_CMD_RD_ID                       0x04
#define ILI9341_CMD_RD_STATUS                   0x09
#define ILI9341_CMD_RD_POWER_MODE               0x0A
#define ILI9341_CMD_RD_MADCTL                   0x0B
#define ILI9341_CMD_RD_PIXEL_FORMAT             0x0C
#define ILI9341_CMD_RD_IMG_FORMAT               0x0D
#define ILI9341_CMD_RD_SIGNAL_MODE              0x0E
#define ILI9341_CMD_RD_SELF_DIAG_RES            0x0F
#define ILI9341_CMD_SLEEP_MODE_ON               0x10
#define ILI9341_CMD_SLEEP_MODE_OFF              0x11
#define ILI9341_CMD_PARTIAL_MODE_ON             0x12
#define ILI9341_CMD_NORMAL_DISPLAY_MODE         0x13
#define ILI9341_CMD_DISPLAY_INVERSION_OFF       0x20
#define ILI9341_CMD_DISPLAY_INVERSION_ON        0x21
#define ILI9341_CMD_GAMMA_SET                   0x26
#define ILI9341_CMD_DISPLAY_OFF                 0x28
#define ILI9341_CMD_DISPLAY_ON                  0x29
#define ILI9341_CMD_SET_COLL_ADDR               0x2A
#define ILI9341_CMD_SET_PAGE_ADDR               0x2B
#define ILI9341_CMD_SET_COLOR                   0x2D
#define ILI9341_CMD_WRITE_MEM                   0x2C
#define ILI9341_CMD_READ_MEM                    0x2E
#define ILI9341_CMD_PARTIAL_AREA                0x30
#define ILI9341_CMD_DEF_VERTICAL_SCROLL         0x33
#define ILI9341_CMD_TEAR_EFF_OFF                0x34
#define ILI9341_CMD_TEAR_EFF_ON                 0x35
#define ILI9341_CMD_MEM_ACCESS_CTRL             0x36
#define ILI9341_CMD_VERT_SCROLL_START_ADDR      0x37
#define ILI9341_CMD_IDDLE_MODE_OFF              0x38
#define ILI9341_CMD_IDDLE_MODE_ON               0x39
#define ILI9341_CMD_SET_PIXEL_FORMAT            0x3A
#define ILI9341_CMD_WRITE_MEM_CONT              0x3C
#define ILI9341_CMD_READ_MEM_CONT               0x3E
#define ILI9341_CMD_SET_TEAR_SCANLINE           0x44
#define ILI9341_CMD_GET_TEAR_SCANLINE           0x45
#define ILI9341_CMD_SET_BRIGHTNESS              0x51
#define ILI9341_CMD_GET_BRIGHTNESS              0x52
#define ILI9341_CMD_WRITE_CTRL_DISPLAY          0x53
#define ILI9341_CMD_READ_CTRL_DISPLAY           0x54
#define ILI9341_CMD_SET_AUTO_BRIGHTNESS         0x55
#define ILI9341_CMD_GET_AUTO_BRIGHTNESS         0x56
#define ILI9341_CMD_SET_CABC_MIN_BRIGHT         0x5E
#define ILI9341_CMD_GET_CABC_MIN_BRIGHT         0x5F
#define ILI9341_CMD_READ_ID1                    0xDA
#define ILI9341_CMD_READ_ID2                    0xDB
#define ILI9341_CMD_READ_ID3                    0xDC

// extends command list

#define ILI9341_CMD_RGB_INT_SIGNAL_CTRL         0xB0
#define ILI9341_CMD_FRAME_CTRL_NM               0xB1
#define ILI9341_CMD_FRAME_CTRL_IM               0xB2
#define ILI9341_CMD_FRAME_CTRL_PM               0xB3
#define ILI9341_CMD_DISPLAY_INVERSION_CTRL      0XB4
#define ILI9341_CMD_BLANK_PORCH_CTRL            0XB5


#endif /* INCLUDE_ILI9341_H_ */
