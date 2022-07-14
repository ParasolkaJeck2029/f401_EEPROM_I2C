/*
 * AT24C256.h
 *
 *  Created on: Jul 14, 2022
 *      Author: ParasolkaJeck
 */

#ifndef INC_AT24C256_H_
#define INC_AT24C256_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

#define AT24_I2C_HANDLER hi2c1

#define AT24_DEV_ADDR 0xA0

uint8_t at24_check_connect();
void Error();

#endif /* INC_AT24C256_H_ */
