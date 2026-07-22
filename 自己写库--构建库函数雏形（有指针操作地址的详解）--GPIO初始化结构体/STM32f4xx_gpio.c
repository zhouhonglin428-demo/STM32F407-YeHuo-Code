
#include "STM32f4xx_gpio.h"

/*
因为此时编写的置位复位函数是针对所有端口的，所以第一个参数跟GPIOF对应
GPIOx是通用型端口，所以自然函数主体内部就不能写GPIOF_BSRRL
如果这么写就代表把函数写死了，此时就不是通用的函数

结论：GPIOx->BSRRL就表示GPIOx是一个指针指向了GPIO_TypeDef的指针
然后对里面结构体中的BSRRL进行操作

*/

void GPIO_SetBit(GPIO_TypeDef * GPIOx,uint16_t GPIO_Pin)
{
    GPIOx->BSRRL = GPIO_Pin;
}

void GPIO_ResetBit(GPIO_TypeDef * GPIOx,uint16_t GPIO_Pin)
{
    GPIOx->BSRRH = GPIO_Pin;
}
