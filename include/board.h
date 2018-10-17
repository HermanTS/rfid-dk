/*
 * board.h
 *
 *  Created on: 27 мар. 2018 г.
 *      Author: gstsvetkov
 */

#ifndef INCLUDE_BOARD_H_
#define INCLUDE_BOARD_H_

#include "stm32f103xb.h"

#if !defined(SET) && !defined(RESET)
#define SET     (uint8_t)0x01
#define RESET   (uint8_t)~SET
#endif


#endif /* INCLUDE_BOARD_H_ */
