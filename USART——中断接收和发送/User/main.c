#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "bsp_usart.h"
#include <stdio.h>

void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	Debug_USART_Config();

	//Usart_SendByte( DEBUG_USART, 'a');

	//USsart_SendString(DEBUG_USART, "串口调试测试\n");
	
	printf("Hello World!\n");
	
	while (1)
	{

	}
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

