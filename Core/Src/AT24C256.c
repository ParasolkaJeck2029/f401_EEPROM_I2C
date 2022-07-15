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
	if (page >= AT24_PAGE_COUNT || addr >= AT24_PAGE_SIZE){
		Error();
		char error_text[] = "This page or address not real\r\nChek AT24_PAGE_COUNT and AT24_PAGE_SIZE\r\n";
		HAL_UART_Transmit(&huart1, error_text, sizeof(error_text), 1000);
		return;
	}
	int page_add_position = log(AT24_PAGE_SIZE)/log(2);
	uint16_t mem_address = page<<page_add_position | addr;
	HAL_I2C_Mem_Write(&hi2c1, AT24_DEV_ADDR, mem_address, I2C_MEMADD_SIZE_16BIT, &data, 1, 1000);
#if(USE_FREERTOS == 0)
	HAL_Delay(5);
#else
	osDelay(5);
#endif

}

void AT24_ReadByte(uint16_t page, uint16_t addr, uint8_t * result){
	if (page >= AT24_PAGE_COUNT || addr >= AT24_PAGE_SIZE){
		Error();
		char error_text[] = "This page or address not real\r\nChek AT24_PAGE_COUNT and AT24_PAGE_SIZE\r\n";
		HAL_UART_Transmit(&huart1, error_text, sizeof(error_text), 1000);
		return;
	}
	int page_add_position = log(AT24_PAGE_SIZE)/log(2);
	uint16_t mem_address = page<<page_add_position | addr;
	HAL_I2C_Mem_Read(&hi2c1, AT24_DEV_ADDR, mem_address, I2C_MEMADD_SIZE_16BIT, result, 1, 1000);
}
void AT24_WriteByte_GlobalAddr(uint16_t addr, uint8_t data){
	if (addr >= AT24_PAGE_COUNT * AT24_PAGE_SIZE){
		Error();
		char error_text[] = "This address not real\r\nChek AT24_PAGE_COUNT and AT24_PAGE_SIZE\r\n";
		HAL_UART_Transmit(&huart1, error_text, sizeof(error_text), 1000);
		return;
	}
	HAL_I2C_Mem_Write(&AT24_I2C_HANDLER, AT24_DEV_ADDR, addr, I2C_MEMADD_SIZE_16BIT, &data, 1, 1000);
#if(USE_FREERTOS == 0)
	HAL_Delay(5);
#else
	osDelay(5);
#endif
}
void AT24_ReadByte_GlobalAddr(uint16_t addr, uint8_t * result){
	if (addr >= AT24_PAGE_COUNT * AT24_PAGE_SIZE){
		Error();
		char error_text[] = "This address not real\r\nChek AT24_PAGE_COUNT and AT24_PAGE_SIZE\r\n";
		HAL_UART_Transmit(&huart1, error_text, sizeof(error_text), 1000);
		return;
	}
	HAL_I2C_Mem_Read(&AT24_I2C_HANDLER, AT24_DEV_ADDR, addr, I2C_MEMADD_SIZE_16BIT, result, 1, 1000);

}

void AT24_PutData(uint16_t page, uint16_t addr, uint8_t * data, uint16_t size){
	if (page >= AT24_PAGE_COUNT || addr >= AT24_PAGE_SIZE){
			Error();
			char error_text[] = "This page or address not real\r\nChek AT24_PAGE_COUNT and AT24_PAGE_SIZE\r\n";
			HAL_UART_Transmit(&huart1, error_text, sizeof(error_text), 1000);
			return;
	}
	int page_add_position = log(AT24_PAGE_SIZE)/log(2);

	uint16_t start_page = page;
	uint16_t last_page = page + (size + addr)/AT24_PAGE_SIZE;
	uint16_t nom_pages = last_page - start_page + 1;
	uint16_t pos_byte = 0;
	for (uint16_t i = 0; i < nom_pages; i++){

		uint16_t mem_address = start_page<<page_add_position | addr;
		uint16_t nom_bytes_to_write_on_page = ((size + addr) < AT24_PAGE_SIZE) ? size : (AT24_PAGE_SIZE - addr);
		HAL_I2C_Mem_Write(&AT24_I2C_HANDLER, AT24_DEV_ADDR, mem_address, I2C_MEMADD_SIZE_16BIT, &data[pos_byte], nom_bytes_to_write_on_page, 1000);
		start_page++;
		addr = 0;
		size = size - nom_bytes_to_write_on_page;
		pos_byte = nom_bytes_to_write_on_page;
#if(USE_FREERTOS == 0)
		HAL_Delay(5);
#else
		osDelay(5);
#endif
	}

}
void AT24_GetData(uint16_t page, uint16_t addr, uint8_t * result, uint16_t size){
	if (page >= AT24_PAGE_COUNT || addr >= AT24_PAGE_SIZE){
			Error();
			char error_text[] = "This page or address not real\r\nChek AT24_PAGE_COUNT and AT24_PAGE_SIZE\r\n";
			HAL_UART_Transmit(&huart1, error_text, sizeof(error_text), 1000);
			return;
	}
	int page_add_position = log(AT24_PAGE_SIZE)/log(2);

	uint16_t start_page = page;
	uint16_t last_page = page + (size + addr)/AT24_PAGE_SIZE;
	uint16_t nom_pages = last_page - start_page + 1;
	uint16_t pos_byte = 0;
	for (uint16_t i = 0; i < nom_pages; i++){

		uint16_t mem_address = start_page<<page_add_position | addr;
		uint16_t nom_bytes_to_read_on_page = ((size + addr) < AT24_PAGE_SIZE) ? size : (AT24_PAGE_SIZE - addr);
		HAL_I2C_Mem_Read(&AT24_I2C_HANDLER, AT24_DEV_ADDR, mem_address, I2C_MEMADD_SIZE_16BIT, &result[pos_byte], nom_bytes_to_read_on_page, 1000);
		start_page++;
		addr = 0;
		size = size - nom_bytes_to_read_on_page;
		pos_byte = nom_bytes_to_read_on_page;
	}

}

void AT24_Erase_Page(uint16_t page){
	int page_add_position = log(AT24_PAGE_SIZE)/log(2);
	uint16_t mem_address = page<<page_add_position;
	uint8_t erase_array[AT24_PAGE_SIZE];
	memset(erase_array, 0xff, AT24_PAGE_SIZE);
	printf("\r\n");
	for (uint8_t i = 0; i < 8; i++){
		  printf("#%d ", i);
		  for(uint8_t j = 0; j < 8; j++){
			  printf("0x%x ", erase_array[(i+1)*j]);
		  }
		  printf("\r\n");
	  }
	  printf("\r\n");
	HAL_I2C_Mem_Write(&AT24_I2C_HANDLER, AT24_DEV_ADDR, mem_address, I2C_MEMADD_SIZE_16BIT, erase_array, AT24_PAGE_SIZE, 1000);

#if(USE_FREERTOS == 0)
		HAL_Delay(5);
#else
		osDelay(5);
#endif

}

void AT24_PrintfPage(uint16_t page){
	uint8_t page_content[AT24_PAGE_SIZE];
	AT24_GetData(1, 0, page_content, sizeof(AT24_PAGE_SIZE));
	printf("\r\n");
	for (uint8_t i = 0; i < 8; i++){
	  printf("#%d ", i);
	  for(uint8_t j = 0; j < 8; j++){
		  printf("0x%x ", page_content[(i+1)*j]);
	  }
	  printf("\r\n");
	}
}
void Error(){
	printf("Error\r\n");
}
