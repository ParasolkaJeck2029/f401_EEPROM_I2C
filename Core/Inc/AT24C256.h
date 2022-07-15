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

#define AT24_PAGE_SIZE 64
#define AT24_PAGE_COUNT 256

uint8_t at24_check_connect();
void Error();
uint8_t I2Cx_WriteData(uint16_t addr, uint16_t reg, uint8_t * value);
uint8_t I2Cx_ReadData(uint16_t addr, uint16_t reg, uint8_t * result);

void AT24_WriteByte(uint16_t page, uint16_t addr, uint8_t data);

#endif /* INC_AT24C256_H_ */

