/*
 * spi.h
 *
 *  Created on: 28 мар. 2018 г.
 *      Author: gstsvetkov
 */

#ifndef INCLUDE_SPI_H_
#define INCLUDE_SPI_H_

#include "board.h"
#include "stm32f103xb.h"


void spi_send_byte(SPI_TypeDef *spi , uint8_t data);
uint8_t spi_read_byte(SPI_TypeDef *spi);
uint8_t spi_send_read(SPI_TypeDef *spi, uint8_t data);
void spi1_gpio_init();
void spi1_periph_init();
void SPI1_IRQHandler();

#endif /* INCLUDE_SPI_H_ */
