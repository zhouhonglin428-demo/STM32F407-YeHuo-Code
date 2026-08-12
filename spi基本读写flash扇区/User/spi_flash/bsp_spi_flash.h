#ifndef _SPI_FLASH_H
#define _SPI_FLASH_H


#include "stm32f4xx.h"
#include <stdio.h>
void SPI_FLASH_Init(void);
uint8_t SPI_FLASH_Byte_Write(uint8_t data);
void SPI__FLASH_Write_Enable(void);
void SPI__FLASH_Wait_For_Standby(void);
void SPI_FLASH_Erase_Sector(uint32_t addr);
void SPI_FLASH_Read_buffer(uint32_t addr,uint8_t *buffer,uint32_t size);
void SPI_FLASH_Page_Write(uint32_t addr,uint8_t *buffer,uint32_t size);

//宏定义
#define SPI_CLK 				RCC_APB2Periph_SPI1
#define SPI_FLASH				SPI1
#define DUMMY					0xFF

#define SPI_FLASH_LOW()				GPIO_ResetBits(GPIOG,GPIO_Pin_6)
#define SPI_FLASH_HIGH()			GPIO_SetBits(GPIOG,GPIO_Pin_6)

//命令定义
#define W25X_WriteEnable		      0x06 
#define W25X_WriteDisable		      0x04 
#define W25X_ReadStatusReg		      0x05 
#define W25X_WriteStatusReg		 	  0x01 
#define W25X_ReadData			      0x03 
#define W25X_FastReadData		      0x0B 
#define W25X_FastReadDual		      0x3B 
#define W25X_PageProgram		      0x02 
#define W25X_BlockErase			      0xD8 
#define W25X_SectorErase		      0x20 
#define W25X_ChipErase			      0xC7 
#define W25X_PowerDown			      0xB9 
#define W25X_ReleasePowerDown	      0xAB 
#define W25X_DeviceID			      0xAB 	
#define W25X_ManufactDeviceID   	  0x90 
#define W25X_JedecDeviceID		      0x9F 

//等待超时时间
#define SPI_FLAG_TIMEOUT          ((uint32_t)0x1000)
#define SPI_LONG_TIMEOUT         ((uint32_t)(10 * SPI_FLAG_TIMEOUT))


#endif /* _SPI_FLASH_H*/
