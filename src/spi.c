/*
 * spi.c
 *
 *  Created on: 28 мар. 2018 г.
 *      Author: gstsvetkov
 */
#include "spi.h"


//uint8_t rx_buff[256];
//uint8_t rx_buff_indexer = 0 ;

void spi_send_byte(SPI_TypeDef *spi , uint8_t data){

        while ((spi->SR & SPI_SR_TXE) != SPI_SR_TXE)
               ; //TODO: add time management
        spi->DR = data;

}

uint8_t spi_read_byte(SPI_TypeDef *spi){
        while ((spi->SR & SPI_SR_RXNE) != SPI_SR_RXNE)
            ;
        return spi->DR;
}

uint8_t spi_send_read(SPI_TypeDef *spi, uint8_t data){
        spi_send_byte(spi, data);
        return spi_read_byte(spi);
}

void spi1_gpio_init(){
        // Enable clock on GPIOA and alternative functions
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
        /*
         * MOSI - PA7
         * MISO - PA6
         * SCK  - PA5
         * CS  - PA4
         * RST  - PA3
         * CSK and MOSI in alternative function, type PUSH - PULL
         * MISO - input, pull up
         * NSS, RST output - P/P
         */

        // SCK: output, 50MHz
        GPIOA->CRL   |=  GPIO_CRL_MODE5;    //
        GPIOA->CRL   &= ~GPIO_CRL_CNF5;     //
        GPIOA->CRL   |=  GPIO_CRL_CNF5_1;   //

        // MISO: digital input, pull up
        GPIOA->CRL   &= ~GPIO_CRL_MODE6;    //
        GPIOA->CRL   &= ~GPIO_CRL_CNF6;     //
        GPIOA->CRL   |=  GPIO_CRL_CNF6_1;   //
        GPIOA->BSRR   =  GPIO_BSRR_BS6;     //

        // MOSI: output, alternative function, 50MHz
        GPIOA->CRL   |=  GPIO_CRL_MODE7;    //
        GPIOA->CRL   &= ~GPIO_CRL_CNF7;     //
        GPIOA->CRL   |=  GPIO_CRL_CNF7_1;   //
}

void spi1_periph_init(void){
        //enable clok to SPI1
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

        //clear previos settings
        SPI1->CR1 &= (uint16_t)~0xff;
        SPI1->CR2 &= (uint16_t)~0xff;

        //set master mode
        SPI1->CR1 |= SPI_CR1_MSTR;
        SPI1->CR1 |= SPI_CR1_SSI;
        SPI1->CR1 |= SPI_CR1_SSM;
        // set baud rate
        SPI1->CR1 |= SPI_CR1_BR_0;
        // Polarity cls signal CPOL = 0;
        SPI1->CR1 &= (uint16_t)~SPI_CR1_CPOL;
        //Phase cls signal    CPHA = 0;
        SPI1->CR1 &= (uint16_t)~SPI_CR1_CPHA;
        // set frame format, major bit first
        SPI1->CR1 &= (uint16_t)~SPI_CR1_LSBFIRST;

        //enable SPI1
        SPI1->CR1 |= SPI_CR1_SPE;

        //enable SPI1 interrupt
        //SPI1->CR2 |= SPI_CR2_RXNEIE;
}


//void SPI1_IRQHandler(){
//        if (SPI1->SR & SPI_SR_RXNE)
//        {
//            rx_buff[rx_buff_indexer] = spi_read_byte(SPI1);
//            rx_buff_indexer++;
//
//            SPI1->SR &= ~SPI_SR_RXNE;
//        }
//}
