/*
 * rfid-rc522.h
 *
 *  Created on: 21 мар. 2018 г.
 *      Author: gstsvetkov
 */

#ifndef INCLUDE_RFID_RC522_H_
#define INCLUDE_RFID_RC522_H_

// MFRC522 redisters
#define RC522_REG_CMD           0x01

// MFRC522 commands
#define RC522_CMD_IDLE          0
#define RC522_CMD_MEM           1
#define RC522_CMD_GEN_RAND_ID   2
#define RC522_CMD_CALC_CRC      3
#define RC522_CMD_TRANSMIT      4
#define RC522_CMD_NO_CHANGE     7
#define RC522_CMD_RECEIVE       8
#define RC522_CMD_TRANSCEIVE    12
#define RC522_CMD_MF_AUTH       14
#define RC522_CMD_RESET         15

#endif /* INCLUDE_RFID_RC522_H_ */
