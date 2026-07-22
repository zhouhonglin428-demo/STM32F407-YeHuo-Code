#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_exti.h"



void Delay(uint32_t count)
{
	for(; count != 0; count--);
}


int main(void)
{
  /* 在这里添加你自己的程序 */
	//LED端口初始化
	LED_GPIO_Config();
	
	//初始化EXTI
	EXTI_Key_Config();
	while(1)
	{
			
		
		
	}
}


