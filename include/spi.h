/*
 * spi.h
 *
 *  Created on: 20 ���. 2018 �.
 *      Author: gstsvetkov
 */

#ifndef INCLUDE_SPI_H_
#define INCLUDE_SPI_H_

void spi1_send_byte(uint8_t data);
void spi1_gpio_init();
void spi1_periph_init();
void SPI1_IRQHandler();

#endif /* INCLUDE_SPI_H_ */
