#ifndef _STM32F4XX_GPIO_H
#define _STM32F4XX_GPIO_H

//因为所有外设寄存器的映射都在STM32f4xx.h文件里面，所以需要包含
#include "STM32f4xx.h"

#define GPIO_Pin_6 (1 << 6)
void GPIO_SetBit(GPIO_TypeDef * GPIOx,uint16_t GPIO_Pin);

void GPIO_ResetBit(GPIO_TypeDef * GPIOx,uint16_t GPIO_Pin);



//enum枚举的作用：收窄边界更高的报错可以更容易发现问题，还能节省内存比如写了200这样子，
//比如写了200这样子，如果没有enum不会报错有了enum超过边界了编译器就会报错
typedef enum
{
    GPIO_Mode_IN = 0x00,
    GPIO_Mode_OUT = 0x01,
    GPIO_Mode_AF = 0x02,
    GPIO_Mode_AN = 0x03
}GPIOMode_TypeDef;

typedef enum
{
    GPIO_PuPd_NOPULL = 0x00,
    GPIO_PuPd_UP     = 0x01,
    GPIO_PuPd_DOWN   = 0x02
}GPIOPuPd_TypeDef;

typedef enum
{
    GPIO_Otype_PP = 0X00,
    GPIO_Otype_OD = 0X01
}GPIOOtype_TypeDef;

typedef enum
{
    GPIO_Speed_2MHz = 0x00,
    GPIO_Speed_25MHz = 0x01,
    GPIO_Speed_50MHz = 0x02,
    GPIO_Speed_100MHz = 0x03
}GPIOSpeed_TypeDef;



/* 定义GPIO初始化结构体成员 */
//TypeDef struct这个是标准定义结构体的写法，{}后面跟着才是结构体的名字
typedef struct
{
    //配置引脚
    uint16_t        GPIO_Pin;
    //配置是输出还是输入
    GPIOMode_TypeDef MODER;
    //配置上拉下拉电阻
    GPIOPuPd_TypeDef PUPDR;
    //如果是输出还要选择是推挽输出还是开漏输出
    GPIOOtype_TypeDef OTYPER;
    //GPIO的输出速度
    GPIOSpeed_TypeDef OSPEEDR;
}GPIO_InitTypeDef;









#endif /* _STM32F4XX_GPIO_H */


