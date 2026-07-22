#include "STM32f4xx.h"

void Delay (unsigned int count)
{
	for(; count != 0; count--);
}



int main(void)
{
	//第一步：开GPIO时钟
	RCC_AHB1ENR |= (1 << 5);
	//第二步：配置GPIO为输出
	GPIOF->MODER &= ~(0x03 << (2*6));
	GPIOF->MODER |= (1 << (2*6));
	//第三步:让PF6先输出0或者1
	GPIOF->ODR |= (1 << 6);
	
	while(1)
	{
//		GPIOF->ODR &= ~(1 << 6);
//		Delay(0x0fffff);
//		GPIOF->ODR |= (1 << 6);
//		Delay(0x0fffff);
	}


}

void SystemInit(void)
{
	//函数体为空目的是骗过编译器不报错
	
}






