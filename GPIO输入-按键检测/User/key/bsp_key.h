#ifndef BSP_KEY
#define BSP_KEY_H

/* 包含所有寄存器映射的头文件 */
#include "stm32f4xx.h"

#define KEY_ON        1
#define KEY_OFF       0



void GPIO_KEY_Config(void);
uint8_t KEY_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);






#endif  /* BSP_KEY_H */

