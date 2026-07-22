#ifndef _STM32F4XX_H
#define _STM32F4XX_H

#include <stdint.h>

/* 用来存放寄存器映射相关的代码    */

/*(unsigned int)的解释
0x40023800表示无人工规定范围的数字
((unsigned int)0x40023800)代表规定了范围就是无符号的32位的数字
((unsigned int)0x40023800)在代码表示有范围的无符号32位数字，在硬件中表示内存地址
*/

/*
解释 *(unsigned int * )那为什么要加两个

第一个*号
加上*号就是让C语言一开始把((unsigned int)0x40023800)变成指针，然后C语言就知道
后面的无符号32位数字代码内存地址，但是还不能用(就是数字到地址的识别)
第二个*号
就代表C语言可以操作那个内存地址，开始从那个内存地址里面写东西了
*/

/*0x40023800此时只是一个普通的*/


#define RCC_BASE  ((unsigned int)0x40023800)
#define GPIOF_BASE ((unsigned int)0x40021400)

#define RCC_AHB1ENR  *(unsigned int *)(RCC_BASE + 0x30)
#define GPIOF_MODER  *(unsigned int *)(GPIOF_BASE + 0x00)
#define GPIOF_OTYPER *(unsigned int *)(GPIOF_BASE + 0x04)
#define GPIOF_ODR    *(unsigned int *)(GPIOF_BASE + 0x14)


 


/*  外设寄存器定义  */
//typedef unsigned int          uint32_t;
//typedef unsigned int  short   uint16_t;


typedef struct
{
    uint32_t MODER;
    uint32_t OTYPER;
    uint32_t OSPEEDR;
    uint32_t PUPDR;
    uint32_t IDR;
    uint32_t ODR;
    uint16_t BSRRL;
    uint16_t BSRRH;
    uint32_t LCKR;
    uint32_t AFRL;
    uint32_t AFRH;
}GPIO_TypeDef;

/*  
(GPIO_TypeDef *)是强制类型转换，告诉编译器后面的数字要强制转换成
指向GPIO_TypeDef的指针,然后宏定义GPIOF，以后写GPIOF就是代表后面的表达式
*/
#define GPIOF ((GPIO_TypeDef *)GPIOF_BASE)






#endif //STM32F4XX_H
