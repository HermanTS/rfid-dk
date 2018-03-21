/*
 * rfid-rfid_rc522.c
 *
 *  Created on: 20 ���. 2018 �.
 *      Author: gstsvetkov
 */
#include "rfid-rc522.h"
#include "spi.h"

void rfid_rc522_activate(){

}

void rfid_rc522_deactivate(){

}

uint32_t rfid_rc522_read_request(uint8_t addr){

        // Address format 1rXXXXXXb
        addr |= (1 << 7);
        spi1_send_byte(addr);

        return 1;
}

void rfid_rc522_write(uint8_t addr,uint8_t data){

        // Address format 0rXXXXXXb
        addr &= ~(1 << 7);
        rfid_rc522_activate();
        spi1_send_byte(addr);

        spi1_send_byte(data);
        rfid_rc522_deactivate();

}

void rfid_rc522_reset(){
        rfid_rc522_write(RC522_REG_CMD, RC522_CMD_RESET);
}

void rfid_rc522_init(){
        rfid_rc522_reset();
        /*
        * Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
        * TModeReg, TPrescalerReg - определяют параметры для внутреннего таймера
        * TModeReg - автоматический старт таймера, и расчет его частоты старший биты предделителя
        * TPrescalerReg - младшие биты предделителя чатоты
        */


}


