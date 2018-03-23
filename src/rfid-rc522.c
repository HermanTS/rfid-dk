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
        rfid_rc522_activate();
        spi1_send_byte(((addr << 1) & 0x7E) | 0x80);

        rfid_rc522_deactivate();
        return 1;
}

void rfid_rc522_write_byte(uint8_t addr,uint8_t data){

        // Address format 0rXXXXXXb
        rfid_rc522_activate();
        spi1_send_byte((addr << 1) & 0x7E);

        spi1_send_byte(data);
        rfid_rc522_deactivate();

}

void rfid_rc522_write(uint8_t first_addr,uint8_t data[], uint8_t data_size){

        uint8_t i;

        rfid_rc522_activate();
        // Address format 0rXXXXXXb
        spi1_send_byte((first_addr << 1) & 0x7E);

        for (i = 0 ; i < data_size; i++)
                spi1_send_byte(data[i]);

        rfid_rc522_deactivate();

}

void rfid_rc522_antenna_on(void) {

        TM_MFRC522_SetBitMask(MFRC522_REG_TX_CONTROL, 0x03);
}

void rfid_rc522_reset(){
        rfid_rc522_write_byte(RC522_REG_CMD, RC522_CMD_RESET);
}

void rfid_rc522_init(){

        rfid_rc522_reset();
        /*
        * Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
        * MFRC522_REG_T_MODE, MFRC522_REG_T_PRESCALER - определяют параметры для внутреннего таймера
        * MFRC522_REG_T_MODE - автоматический старт таймера, и расчет его частоты старший биты предделителя
        * MFRC522_REG_T_PRESCALER - младшие биты предделителя чатоты
        */

        rfid_rc522_write_byte(MFRC522_REG_T_MODE, 0x8D);
        rfid_rc522_write_byte(MFRC522_REG_T_PRESCALER, 0x3E);
        rfid_rc522_write_byte(MFRC522_REG_T_RELOAD_L, 30);
        rfid_rc522_write_byte(MFRC522_REG_T_RELOAD_H, 0);

        /* 48dB gain */
        //rfid_rc522_write_byte(MFRC522_REG_RF_CFG, 0x70);
        rfid_rc522_write_byte(MFRC522_REG_TX_AUTO, 0x40);
        rfid_rc522_write_byte(MFRC522_REG_MODE, 0x3D);

        rfid_rc522_antenna_on();         //Open the antenna

}


