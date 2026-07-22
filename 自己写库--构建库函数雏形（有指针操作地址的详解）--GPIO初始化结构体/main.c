#include "STM32f4xx.h"
#include "STM32f4xx_gpio.h"


void Delay (unsigned int count)
{
	for(; count != 0; count--);
}



int main(void)
{

	#if 0 /* 定义外设的寄存器结构体 */
	//第一步：开GPIO时钟
	RCC_AHB1ENR |= (1 << 5);
	//第二步：配置GPIO为输出
	GPIOF->MODER &= ~(0x03 << (2*6));
	GPIOF->MODER |= (1 << (2*6));
	//第三步:让PF6先输出0或者1
	GPIOF->ODR |= (1 << 6);
	
	while(1)
	{
		GPIOF_ODR &= ~(1 << 6);
		Delay(0x0fffff);
		GPIOF_ODR &= ~(1 << 6);
		GPIOF_ODR |= (1 << 6);
		Delay(0x0fffff);
	}

	#elif 1 /*	编写端口的置位复位函数 */
	//第一步：开GPIO时钟
	RCC_AHB1ENR |= (1 << 5);
	//第二步：配置GPIO为输出
//	GPIOF->MODER &= ~(0x03 << (2*6));
//	GPIOF->MODER |= (1 << (2*6));
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.MODER = GPIO_Mode_OUT;
	GPIO_InitStruct.PUPDR = GPIO_PuPd_UP;
	GPIO_InitStruct.OTYPER = GPIO_Otype_PP;
	GPIO_InitStruct.OSPEEDR = GPIO_Speed_2MHz;
	//第三步:让PF6先输出0或者1
	GPIOF->ODR |= (1 << 6);
}	
	while(1)
	{
		GPIOF->BSRRL = GPIO_Pin_6;
		Delay(0x0fffff);
		GPIOF->BSRRH = GPIO_Pin_6;
		Delay(0x0fffff);
	}

	#elif 1 /*	定义外设的初始化结构体和编写外设的初始化函数 */
	//第一步：开GPIO时钟
	RCC_AHB1ENR |= (1 << 5);
	//第二步：配置GPIO为输出
	GPIOF->MODER &= ~(0x03 << (2*6));
	GPIOF->MODER |= (1 << (2*6));
	//第三步:让PF6先输出0或者1
	GPIOF->ODR |= (1 << 6);
	
	while(1)
	{
		GPIO_ResetBit(GPIOF,GPIO_Pin_6);
		Delay(0x0fffff);
		GPIO_SetBit(GPIOF,GPIO_Pin_6);
		Delay(0x0fffff);
	}

	#endif

}

void SystemInit(void)
{
	//函数体为空目的是骗过编译器不报错
	
}






