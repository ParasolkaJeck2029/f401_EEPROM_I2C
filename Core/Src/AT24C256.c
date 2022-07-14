/*
 * AT24C256.c
 *
 *  Created on: Jul 14, 2022
 *      Author: ParasolkaJeck
 */

#include "AT24C256.h"

extern I2C_HandleTypeDef AT24_I2C_HANDLER;
extern UART_HandleTypeDef huart1;

uint8_t at24_check_connect(){
	HAL_StatusTypeDef res = HAL_OK;
	res = HAL_I2C_IsDeviceReady(&AT24_I2C_HANDLER, AT24_DEV_ADDR, 3, 1000);
	if (res != HAL_OK)Error();
	return res;
}

void Error(){
	printf("Error\r\n");
}
