#ifndef _BSP_IIC_EE_H
#define _BSP_IIC_EE_H


#include "stm32f4xx.h"
#include <stdio.h>
void EEPROM_IIC_Config(void);
uint8_t EEPROM_Byte_Write(uint8_t addr,uint8_t data);
uint8_t EEPROM_Random_Read(uint8_t addr,uint8_t *data);
uint8_t Wait_For_Standby(void);
uint8_t EEPROM_Page_Write(uint8_t addr,uint8_t *data, uint8_t size);
uint8_t EEPROM_Buffer_Read(uint8_t addr,uint8_t *data,uint16_t size);
uint8_t EEPROM_Buffer_Write(uint8_t addr,uint8_t *data, uint16_t size);
//引脚定义
#define EEPROM_I2C1_ADDR						0xA0   //写地址
#define EEPROM_I2C1_ADDR_R 						0xA1  // 读地址


#define EEPROM_PAGE_SIZE						8

#define EEPROM_I2C1_ClockSpeed					400000





#endif /* _BSP_IIC_EE_H */
