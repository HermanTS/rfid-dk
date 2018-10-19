/*
 * ili9341.h
 *
 *  Created on: 27 мар. 2018 г.
 *      Author: gstsvetkov
 */

#ifndef INCLUDE_ILI9341_H_
#define INCLUDE_ILI9341_H_

//color map
typedef enum {
    white = 0xFFFF,
    black = 0x0000,
    red = 0xF800,
    yellow = 0xFFE0,
}color_t;

// public functions
void ili9341_gpio_init();
void ili9341_init();
uint32_t ili9341_read_ID(void);
void ili9341_led_set_state(uint8_t state);
uint8_t ili9341_read_byte(uint8_t cmd);

// drawn functions
void ili9341_fill_screen(color_t color);
void ili9341_draw_pixel(uint16_t x, uint16_t y, color_t color);
void ili9341_draw_point(uint16_t x, uint16_t y, color_t color, uint8_t size);
void ili9341_draw_line(uint16_t x, uint16_t y, color_t color, uint32_t line_size);
void ili9341_draw_sqare(uint16_t x_top, uint16_t x_bottom,
                        uint16_t y_right, uint16_t y_left,
                        uint16_t border_px, color_t color);

#ifndef ILI9341_GPIO
#define ILI9341_GPIO                    GPIOA
#define ILI9341_PIN_RESET_NO            3
#define ILI9341_PIN_DC_NO               2
#define ILI9341_PIN_CS_NO               4
#define ILI9341_PIN_LED_NO              1
#endif


#ifndef ILI9341_SPI
#define ILI9341_SPI                     SPI1
#endif

#define ILI9341_PIN_RESET               (1 << ILI9341_PIN_RESET_NO)
#define ILI9341_PIN_DC                  (1 << ILI9341_PIN_DC_NO)
#define ILI9341_PIN_CS                  (1 << ILI9341_PIN_CS_NO)
#ifdef ILI9341_PIN_LED_NO
#define ILI9341_PIN_LED                 (1 << ILI9341_PIN_LED_NO)
#endif

#ifndef ILI9341_SCREEN_SIZE
#define ILI9341_WIDTH       240
#define ILI9341_HEIGHT      320
#define ILI9341_PIXEL_COUNT (ILI9341_WIDTH * ILI9341_HEIGHT)
#endif

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
#define ILI9341_CMD_SET_COLLUM_ADDR             0x2A
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
#define ILI9341_CMD_DISPLAY_INVERSION_CTRL      0xB4
#define ILI9341_CMD_BLANK_PORCH_CTRL            0xB5
#define ILI9341_CMD_DISPAY_FUNC_CTRL            0xB6
#define ILI9341_CMD_SET_ENTRY_MODE              0xB7
#define ILI9341_CMD_BACKLIGHT_CTRL1             0xB8
#define ILI9341_CMD_BACKLIGHT_CTRL2             0xB9
#define ILI9341_CMD_BACKLIGHT_CTRL3             0xBA
#define ILI9341_CMD_BACKLIGHT_CTRL4             0xBB
#define ILI9341_CMD_BACKLIGHT_CTRL5             0xBC
#define ILI9341_CMD_BACKLIGHT_CTRL7             0xBE
#define ILI9341_CMD_BACKLIGHT_CTRL8             0xBF
#define ILI9341_CMD_POWER_CTRL1                 0xC0
#define ILI9341_CMD_POWER_CTRL2                 0xC1
#define ILI9341_CMD_VCOM_CTRL1                  0xC5
#define ILI9341_CMD_VCOM_CTRL2                  0xC7
#define ILI9341_CMD_NV_MEM_WRITE                0xD0
#define ILI9341_CMD_NV_MEM_PRETECKT_KEY         0xD1
#define ILI9341_CMD_NV_MEM_STATUS_READ          0xD2
#define ILI9341_CMD_READ_ID4                    0xD3
#define ILI9341_CMD_POSITIV_GAMMA_CORR          0xE0
#define ILI9341_CMD_NEGATIV_GAMMA_CORR          0xE1
#define ILI9341_CMD_DIGIT_GAMMA_CTRL1           0xE2
#define ILI9341_CMD_DIGIT_GAMMA_CTRL2           0xE3
#define ILI9341_CMD_INTERFACE_CTRL              0xF6

// extend register command

#define ILI9341_CMD_POWER_CTRLA                 0xCB
#define ILI9341_CMD_POWER_CTRLB                 0xCF
#define ILI9341_CMD_TIMER_CTRLA_1               0xE8
#define ILI9341_CMD_TIMER_CTRLA_2               0xE9
#define ILI9341_CMD_TIMER_CTRLB                 0xEA
#define ILI9341_CMD_POWER_SEQ_CTRL              0xED
#define ILI9341_CMD_GAMMA3_CTRL                 0xF2
#define ILI9341_CMD_PUMP_RATIO_CTRL             0xF7

#endif /* INCLUDE_ILI9341_H_ */
