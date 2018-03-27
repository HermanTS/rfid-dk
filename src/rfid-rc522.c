/**
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * |
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * |
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "rfid-rc522.h"

void rfid_rc522_activate(){

MFRC522_CS_PORT->BSRR = MFRC522_CS_PIN;

}

void rfid_rc522_deactivate(){

MFRC522_CS_PORT->BRR = MFRC522_CS_PIN;

}

void rfid_rc522_init(void) {

        rfid_rc522_reset();

        rfid_rc522_write_register(MFRC522_REG_T_MODE, 0x8D);
        rfid_rc522_write_register(MFRC522_REG_T_PRESCALER, 0x3E);
        rfid_rc522_write_register(MFRC522_REG_T_RELOAD_L, 30);
        rfid_rc522_write_register(MFRC522_REG_T_RELOAD_H, 0);

        /* 48dB gain */
        rfid_rc522_write_register(MFRC522_REG_RF_CFG, 0x70);

        rfid_rc522_write_register(MFRC522_REG_TX_AUTO, 0x40);
        rfid_rc522_write_register(MFRC522_REG_MODE, 0x3D);

        rfid_rc522_antenna_on();         //Open the antenna
}

uint8_t rfid_rc522_check(uint8_t* id) {
        uint8_t status;
        //Find cards, return card type
        status = rfid_rc522_request(PICC_REQIDL, id);
        if (status == MI_OK) {
                //Card detected
                //Anti-collision, return card serial number 4 bytes
                status = rfid_rc522_anticoll(id);
        }
        rfid_rc522_halt();                      //Command card into hibernation

        return status;
}

uint8_t rfid_rc522_compare(uint8_t* CardID, uint8_t* CompareID) {
        uint8_t i;
        for (i = 0; i < 5; i++) {
                if (CardID[i] != CompareID[i]) {
                        return MI_ERR;
                }
        }
        return MI_OK;
}


void rfid_rc522_write_register(uint8_t addr, uint8_t val) {
        //CS low
        rfid_rc522_activate();
        //Send address
        spi_send_byte(MFRC522_SPI, (addr << 1) & 0x7E);
        //Send data
        spi_send_byte(MFRC522_SPI, val);
        //CS high
        rfid_rc522_deactivate();
}

uint8_t rfid_rc522_read_register(uint8_t addr) {
        uint8_t val;
        //CS low
        rfid_rc522_activate();

        spi_send_byte(MFRC522_SPI, ((addr << 1) & 0x7E) | 0x80);
        val = spi_read_byte(MFRC522_SPI);
        //CS high
        rfid_rc522_deactivate();

        return val;
}

void rfid_rc522_set_bit_mask(uint8_t reg, uint8_t mask) {
        rfid_rc522_write_register(reg, rfid_rc522_read_register(reg) | mask);
}

void rfid_rc522_clear_bit_mask(uint8_t reg, uint8_t mask){
        rfid_rc522_write_register(reg, rfid_rc522_read_register(reg) & (~mask));
}

void rfid_rc522_antenna_on(void) {
        uint8_t temp;

        temp = rfid_rc522_read_register(MFRC522_REG_TX_CONTROL);
        if (!(temp & 0x03)) {
                rfid_rc522_set_bit_mask(MFRC522_REG_TX_CONTROL, 0x03);
        }
}

void rfid_rc522_antenna_off(void) {
        rfid_rc522_clear_bit_mask(MFRC522_REG_TX_CONTROL, 0x03);
}

void rfid_rc522_reset(void) {
        rfid_rc522_write_register(MFRC522_REG_COMMAND, PCD_RESETPHASE);
}

uint8_t rfid_rc522_request(uint8_t reqMode, uint8_t* TagType) {
        uint8_t status;
        uint16_t backBits;                      //The received data bits

        rfid_rc522_write_register(MFRC522_REG_BIT_FRAMING, 0x07);                //TxLastBists = BitFramingReg[2..0]     ???

        TagType[0] = reqMode;
        status = rfid_rc522_to_card(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);

        if ((status != MI_OK) || (backBits != 0x10)) {
                status = MI_ERR;
        }

        return status;
}

uint8_t rfid_rc522_to_card(uint8_t command, uint8_t* sendData, uint8_t sendLen, uint8_t* backData, uint16_t* backLen) {
        uint8_t status = MI_ERR;
        uint8_t irqEn = 0x00;
        uint8_t waitIRq = 0x00;
        uint8_t lastBits;
        uint8_t n;
        uint16_t i;

        switch (command) {
                case PCD_AUTHENT: {
                        irqEn = 0x12;
                        waitIRq = 0x10;
                        break;
                }
                case PCD_TRANSCEIVE: {
                        irqEn = 0x77;
                        waitIRq = 0x30;
                        break;
                }
                default:
                        break;
        }

        rfid_rc522_write_register(MFRC522_REG_COMM_IE_N, irqEn | 0x80);
        rfid_rc522_clear_bit_mask(MFRC522_REG_COMM_IRQ, 0x80);
        rfid_rc522_set_bit_mask(MFRC522_REG_FIFO_LEVEL, 0x80);

        rfid_rc522_write_register(MFRC522_REG_COMMAND, PCD_IDLE);

        //Writing data to the FIFO
        for (i = 0; i < sendLen; i++) {
                rfid_rc522_write_register(MFRC522_REG_FIFO_DATA, sendData[i]);
        }

        //Execute the command
        rfid_rc522_write_register(MFRC522_REG_COMMAND, command);
        if (command == PCD_TRANSCEIVE) {
                rfid_rc522_set_bit_mask(MFRC522_REG_BIT_FRAMING, 0x80);           //StartSend=1,transmission of data starts
        }

        //Waiting to receive data to complete
        i = 2000;       //i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms???
        do {
                //CommIrqReg[7..0]
                //Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
                n = rfid_rc522_read_register(MFRC522_REG_COMM_IRQ);
                i--;
        } while ((i!=0) && !(n&0x01) && !(n&waitIRq));

        rfid_rc522_clear_bit_mask(MFRC522_REG_BIT_FRAMING, 0x80);                 //StartSend=0

        if (i != 0)  {
                if (!(rfid_rc522_read_register(MFRC522_REG_ERROR) & 0x1B)) {
                        status = MI_OK;
                        if (n & irqEn & 0x01) {
                                status = MI_NOTAGERR;
                        }

                        if (command == PCD_TRANSCEIVE) {
                                n = rfid_rc522_read_register(MFRC522_REG_FIFO_LEVEL);
                                lastBits = rfid_rc522_read_register(MFRC522_REG_CONTROL) & 0x07;
                                if (lastBits) {
                                        *backLen = (n - 1) * 8 + lastBits;
                                } else {
                                        *backLen = n * 8;
                                }

                                if (n == 0) {
                                        n = 1;
                                }
                                if (n > MFRC522_MAX_LEN) {
                                        n = MFRC522_MAX_LEN;
                                }

                                //Reading the received data in FIFO
                                for (i = 0; i < n; i++) {
                                        backData[i] = rfid_rc522_read_register(MFRC522_REG_FIFO_DATA);
                                }
                        }
                } else {
                        status = MI_ERR;
                }
        }

        return status;
}

uint8_t rfid_rc522_anticoll(uint8_t* serNum) {
        uint8_t status;
        uint8_t i;
        uint8_t serNumCheck = 0;
        uint16_t unLen;

        rfid_rc522_write_register(MFRC522_REG_BIT_FRAMING, 0x00);                //TxLastBists = BitFramingReg[2..0]

        serNum[0] = PICC_ANTICOLL;
        serNum[1] = 0x20;
        status = rfid_rc522_to_card(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);

        if (status == MI_OK) {
                //Check card serial number
                for (i = 0; i < 4; i++) {
                        serNumCheck ^= serNum[i];
                }
                if (serNumCheck != serNum[i]) {
                        status = MI_ERR;
                }
        }
        return status;
}

void rfid_rc522_calculate_CRC(uint8_t*  pIndata, uint8_t len, uint8_t* pOutData) {
        uint8_t i, n;

        rfid_rc522_clear_bit_mask(MFRC522_REG_DIV_IRQ, 0x04);                     //CRCIrq = 0
        rfid_rc522_set_bit_mask(MFRC522_REG_FIFO_LEVEL, 0x80);                    //Clear the FIFO pointer
        //Write_MFRC522(CommandReg, PCD_IDLE);

        //Writing data to the FIFO
        for (i = 0; i < len; i++) {
                rfid_rc522_write_register(MFRC522_REG_FIFO_DATA, *(pIndata+i));
        }
        rfid_rc522_write_register(MFRC522_REG_COMMAND, PCD_CALCCRC);

        //Wait CRC calculation is complete
        i = 0xFF;
        do {
                n = rfid_rc522_read_register(MFRC522_REG_DIV_IRQ);
                i--;
        } while ((i!=0) && !(n&0x04));                  //CRCIrq = 1

        //Read CRC calculation result
        pOutData[0] = rfid_rc522_read_register(MFRC522_REG_CRC_RESULT_L);
        pOutData[1] = rfid_rc522_read_register(MFRC522_REG_CRC_RESULT_M);
}

uint8_t rfid_rc522_select_tag(uint8_t* serNum) {
        uint8_t i;
        uint8_t status;
        uint8_t size;
        uint16_t recvBits;
        uint8_t buffer[9];

        buffer[0] = PICC_SElECTTAG;
        buffer[1] = 0x70;
        for (i = 0; i < 5; i++) {
                buffer[i+2] = *(serNum+i);
        }
        rfid_rc522_calculate_CRC(buffer, 7, &buffer[7]);         //??
        status = rfid_rc522_to_card(PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits);

        if ((status == MI_OK) && (recvBits == 0x18)) {
                size = buffer[0];
        } else {
                size = 0;
        }

        return size;
}

uint8_t rfid_rc522_auth(uint8_t authMode, uint8_t BlockAddr, uint8_t* Sectorkey, uint8_t* serNum) {
        uint8_t status;
        uint16_t recvBits;
        uint8_t i;
        uint8_t buff[12];

        //Verify the command block address + sector + password + card serial number
        buff[0] = authMode;
        buff[1] = BlockAddr;
        for (i = 0; i < 6; i++) {
                buff[i+2] = *(Sectorkey+i);
        }
        for (i=0; i<4; i++) {
                buff[i+8] = *(serNum+i);
        }
        status = rfid_rc522_to_card(PCD_AUTHENT, buff, 12, buff, &recvBits);

        if ((status != MI_OK) || (!(rfid_rc522_read_register(MFRC522_REG_STATUS2) & 0x08))) {
                status = MI_ERR;
        }

        return status;
}

uint8_t rfid_rc522_read(uint8_t blockAddr, uint8_t* recvData) {
        uint8_t status;
        uint16_t unLen;

        recvData[0] = PICC_READ;
        recvData[1] = blockAddr;
        rfid_rc522_calculate_CRC(recvData,2, &recvData[2]);
        status = rfid_rc522_to_card(PCD_TRANSCEIVE, recvData, 4, recvData, &unLen);

        if ((status != MI_OK) || (unLen != 0x90)) {
                status = MI_ERR;
        }

        return status;
}

uint8_t rfid_rc522_write(uint8_t blockAddr, uint8_t* writeData) {
        uint8_t status;
        uint16_t recvBits;
        uint8_t i;
        uint8_t buff[18];

        buff[0] = PICC_WRITE;
        buff[1] = blockAddr;
        rfid_rc522_calculate_CRC(buff, 2, &buff[2]);
        status = rfid_rc522_to_card(PCD_TRANSCEIVE, buff, 4, buff, &recvBits);

        if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)) {
                status = MI_ERR;
        }

        if (status == MI_OK) {
                //Data to the FIFO write 16Byte
                for (i = 0; i < 16; i++) {
                        buff[i] = *(writeData+i);
                }
                rfid_rc522_calculate_CRC(buff, 16, &buff[16]);
                status = rfid_rc522_to_card(PCD_TRANSCEIVE, buff, 18, buff, &recvBits);

                if ((status != MI_OK) || (recvBits != 4) || ((buff[0] & 0x0F) != 0x0A)) {
                        status = MI_ERR;
                }
        }

        return status;
}

void rfid_rc522_halt(void) {
        uint16_t unLen;
        uint8_t buff[4];

        buff[0] = PICC_HALT;
        buff[1] = 0;
        rfid_rc522_calculate_CRC(buff, 2, &buff[2]);

        rfid_rc522_to_card(PCD_TRANSCEIVE, buff, 4, buff, &unLen);
}

