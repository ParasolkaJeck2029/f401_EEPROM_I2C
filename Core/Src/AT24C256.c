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


uint8_t I2Cx_WriteData(uint16_t addr, uint16_t reg, uint8_t * value){
	HAL_StatusTypeDef res = HAL_OK;
	res = HAL_I2C_Mem_Write(&AT24_I2C_HANDLER, addr, reg, I2C_MEMADD_SIZE_8BIT, value, 1, 1000);
	return res;
}
uint8_t I2Cx_ReadData(uint16_t addr, uint16_t reg, uint8_t * result){
	HAL_StatusTypeDef res = HAL_OK;
	res = HAL_I2C_Mem_Read(&AT24_I2C_HANDLER, addr, reg, I2C_MEMADD_SIZE_8BIT, result, 1, 1000);
	return res;
}


void AT24_WriteByte(uint16_t page, uint16_t addr, uint8_t data){
	int page_add_position = log(AT24_PAGE_SIZE)/log(2);
	uint16_t mem_address = page<<page_add_position | addr;
	HAL_I2C_Mem_Write(&hi2c1, AT24_DEV_ADDR, mem_address, I2C_MEMADD_SIZE_16BIT, &data, 1, 1000);
	HAL_Delay(5);

}

void AT24_ReadByte(uint16_t page, uint16_t addr, uint8_t * result){
	int page_add_position = log(AT24_PAGE_SIZE)/log(2);
	uint16_t mem_address = page<<page_add_position | addr;
	HAL_I2C_Mem_Read(&hi2c1, AT24_DEV_ADDR, mem_address, I2C_MEMADD_SIZE_16BIT, result, 1, 1000);
}
void AT24_WriteByte_GlobalAddr(uint16_t addr, uint8_t data){

	HAL_I2C_Mem_Write(&AT24_I2C_HANDLER, AT24_DEV_ADDR, addr, I2C_MEMADD_SIZE_16BIT, &data, 1, 1000);
	HAL_Delay(5);
}
void AT24_ReadByte_GlobalAddr(uint16_t addr, uint8_t * result){
	HAL_I2C_Mem_Read(&AT24_I2C_HANDLER, AT24_DEV_ADDR, addr, I2C_MEMADD_SIZE_16BIT, result, 1, 1000);

}
void Error(){
	printf("Error\r\n");
}
