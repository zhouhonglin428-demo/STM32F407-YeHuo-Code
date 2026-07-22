//bsp: board support package 板件支持包

//板件支持包就是针对这个开发板上led的

#include "bsp_led.h"


void GPIO_LED_Config(void)
{
    /*
    外设首先要进行初始化
    所有外设初始化都可以简单的分为4个步骤
    这里以GPIO为例
    */

    /* 第一步：开GPIO的时钟 */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
    /* 第二步：定义GPIO初始化结构体 */
		GPIO_InitTypeDef GPIO_InitStruct;
	
    /* 第三步：配置GPIO初始化结构体成员 */
	  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_6;//配置GPIO引脚																					//配置引脚
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//配置成输出模式
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//配置成输出推挽模式
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;//配置端口输出速度
		GPIO_InitStruct.GPIO_PuPd	 = GPIO_PuPd_UP;//配置默认上拉电阻

    /* 第四步：调用GPIO初始化函数，把配置好的结构体里面的成员的参数写入寄存器 */
		GPIO_Init(GPIOF, &GPIO_InitStruct);
		
		GPIO_ResetBits(GPIOF, GPIO_Pin_6);
		
		
		
/*		点亮第二个灯
	
    
	  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_7;//配置GPIO引脚																					//配置引脚
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//配置成输出模式
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//配置成输出推挽模式
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;//配置端口输出速度
		GPIO_InitStruct.GPIO_PuPd	 = GPIO_PuPd_UP;//配置默认上拉电阻


		GPIO_Init(GPIOF, &GPIO_InitStruct);
		
		GPIO_ResetBits(GPIOF, GPIO_Pin_7);
		
		
		
		//点亮第三个灯
	
    // 第三步：配置GPIO初始化结构体成员 
	  GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8;//配置GPIO引脚																					//配置引脚
		GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;//配置成输出模式
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//配置成输出推挽模式
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;//配置端口输出速度
		GPIO_InitStruct.GPIO_PuPd	 = GPIO_PuPd_UP;//配置默认上拉电阻

     第四步：调用GPIO初始化函数，把配置好的结构体里面的成员的参数写入寄存器 
		GPIO_Init(GPIOF, &GPIO_InitStruct);
		
		GPIO_ResetBits(GPIOF, GPIO_Pin_8);*/
}



void digitalTOggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    // 读取当前引脚的输出状态，如果是1就置0，如果是0就置1（实现翻转）
    if(GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin) == Bit_SET)
    {
        GPIO_ResetBits(GPIOx, GPIO_Pin);
    }
    else
    {
        GPIO_SetBits(GPIOx, GPIO_Pin);
    }
}



