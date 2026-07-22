#include "bsp_key.h"


void GPIO_KEY_Config(void)
{
    /* 所有外设初始化都有4步 */
    /* 第一步：开启GPIO时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	 
	/* 第二步：定义GPIO初始化结构体 */
    GPIO_InitTypeDef GPIO_InitStruct;
    
	/* 第三步：配置GPIO初始化结构体的参数 */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	/* 第四步：使用GPIO_Init函数，将配置好的参数写入对应的寄存器 */
    GPIO_Init(GPIOA,&GPIO_InitStruct);
	

}	

    /* 按键扫描 */
    uint8_t KEY_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin)
    {
        /* 读取IO口的电平状态，判断有没有按下*/
		if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON)
		{
			/* 松手检测：如果按键一直按下，就是KEY_ON表示while循环一直进行 */
			while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON);
			return KEY_ON;
		}
		else
		{
			return KEY_OFF;
		}
    }












