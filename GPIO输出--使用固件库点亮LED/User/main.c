#include "stm32f4xx.h"

void Delay(uint32_t count)
{
	for(; count != 0; count--);
}


int main(void)
{
  /* 在这里添加你自己的程序 */
	
	GPIO_LED_Config();
  while(1)
	{
		GPIO_ResetBits(GPIOF, GPIO_Pin_6);
		Delay(0xffffff);
		GPIO_SetBits(GPIOF, GPIO_Pin_6);
		Delay(0xffffff);
		
		GPIO_ResetBits(GPIOF, GPIO_Pin_7);
		Delay(0xffffff);
		GPIO_SetBits(GPIOF, GPIO_Pin_7);
		Delay(0xffffff);
		
		GPIO_ResetBits(GPIOF, GPIO_Pin_8);
		Delay(0xffffff);
		GPIO_SetBits(GPIOF, GPIO_Pin_8);
		Delay(0xffffff);
	}
}


