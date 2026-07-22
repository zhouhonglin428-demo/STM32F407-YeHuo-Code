#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "bsp_systick.h"


void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/* LED 端口初始化 */
	LED_GPIO_Config();

	SysTick_Init();
	
	/* 控制LED灯 */
	while (1)
	{
		LED1(ON);
		Delay_ms(500);
		LED1(OFF);
		Delay_ms(500);
		
	}
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

