/* 
这种#ifndef的标准头文件写法是为了当头文件被多个文件包含的时候
里面的内容不会被重复包含也就是避免重复定义
*/

#ifndef BSP_LED_H
#define BSP_LED_H

#include "stm32f4xx.h"

void GPIO_LED_Config(void);


#endif /* BSP_LED_H */


