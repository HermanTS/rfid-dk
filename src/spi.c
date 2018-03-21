/*
 * spi.c
 *
 *  Created on: 20 ���. 2018 �.
 *      Author: gstsvetkov
 */
#include "stm32f10x.h"
#include "spi.h"


uint8_t data;

void spi1_send_byte(uint8_t data){
        if (SPI1->SR & SPI_SR_TXE)
        {
            SPI1->DR = data;
        }
}

void spi1_gpio_init(){
        // Enable clock on GPIOA and alternative functions
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
        /*
         * MOSI - PA7
         * MISO - PA6
         * SCK  - PA5
         * NSS  - PA4
         * RST  - PA3
         * CSK and MOSI in alternative function, type PUSH - PULL
         * MISO - input, pull up
         * NSS, RST output - P/P
         */
        // set mode output
        GPIOA->CRL |= GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE7;
        // set mode input
        GPIOA->CRL &= ~GPIO_CRL_MODE6;
        // set alternative function Push-pull
        GPIOA->CRL |= GPIO_CRL_CNF4_1 | GPIO_CRL_CNF5_1 | GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_1;
}

void spi1_periph_init(){
        //enable clok to SPI1
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

        //set master mode
        SPI1->CR1 |= SPI_CR1_MSTR;

        // set baud rate
        SPI1->CR1 |= SPI_CR1_BR;
        // set data size 8 bit

        // Polarity cls signal CPOL = 0;
        SPI1->CR1 &= ~SPI_CR1_CPOL;
        //Phase cls signal    CPHA = 0;
        SPI1->CR1 &= ~SPI_CR1_CPHA;

        // set frame format, major bit first
        SPI1->CR1 &= ~SPI_CR1_LSBFIRST;

        //enable SPI1
        SPI1->CR1 |= SPI_CR1_SPE;

        //enable SPI1 interrupt
        SPI1->CR2 |= SPI_CR2_RXNEIE;
        NVIC_EnableIRQ(SPI1_IRQn);
}

void SPI1_IRQHandler(){
        if (SPI1->SR & SPI_SR_RXNE)
            data = SPI1->DR;
        SPI1->SR &= ~SPI_SR_RXNE;
}
