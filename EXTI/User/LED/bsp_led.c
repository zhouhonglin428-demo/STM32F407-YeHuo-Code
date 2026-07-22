//bsp: board support package 板件支持包

//板件支持包就是针对这个开发板上led的

#include "bsp_led.h"


void LED_GPIO_Config(void)
{
    /*
    外设首先要进行初始化
    所有外设初始化都可以简单的分为4个步骤
    这里以GPIO为例
    */

    /* 第一步：开GPIO的时钟 */
		RCC_AHB1PeriphClockCmd(LED1_GPIO_CLK| LED2_GPIO_CLK,ENABLE);
    /* 第二步：定义GPIO初始化结构体 */
		GPIO_InitTypeDef GPIO_InitStruct;
	
    /* 第三步：配置GPIO初始化结构体成员 */
	    GPIO_InitStruct.GPIO_Pin   = LED1_PIN;//配置GPIO引脚																					
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//配置成输出模式
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//配置成输出推挽模式
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;//配置端口输出速度
		GPIO_InitStruct.GPIO_PuPd	 = GPIO_PuPd_UP;//配置默认上拉电阻

    /* 第四步：调用GPIO初始化函数，把配置好的结构体里面的成员的参数写入寄存器 */
		GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);
		
		GPIO_ResetBits(LED1_GPIO_PORT, LED1_PIN);
		
		
		
		//点亮第二个灯
	
    
	  GPIO_InitStruct.GPIO_Pin   = LED2_PIN;//配置GPIO引脚																					
		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);		
		GPIO_ResetBits(LED2_GPIO_PORT, GPIO_Pin_7);
		
	/*关闭RGB灯*/
	LED_RGBOFF;	
}






