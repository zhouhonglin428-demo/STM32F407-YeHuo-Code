#include "stm32f4xx.h"
#include "bsp_key.h"
#include "bsp_led.h"

void Delay(uint32_t count)
{
	for(; count != 0; count--);
}


int main(void)
{
  /* 在这里添加你自己的程序 */
	/* LED端口初始化 */
	GPIO_LED_Config();
	/* key(按键)初始化 */
	GPIO_KEY_Config();
  

	/* 按键扫描 */
	while(1)
	{
		if(KEY_Scan(GPIOA,GPIO_Pin_0) == KEY_ON)
		{
			LED1_TOGGLE;
		}
	}

}


