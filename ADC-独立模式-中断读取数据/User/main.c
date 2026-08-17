#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "bsp_usart.h"
#include <stdio.h>
#include "bsp_adc.h"

/*
ADC会产生转换，转换之后会产生一个点
转换之后会把这个数据放到规则序列寄存器里面
然后产生中断，接着就要响应中断
*/

//这个变量是存储ADC测量结果，已经在stm32f4xx_it.c定义过了
__IO uint16_t ADC_ConvertedValue;
//定义一个变量，用于保存转换计算后ADC测量结果的电压值
float ADC_Vol;

//延时函数
static void Delay(__IO uint32_t ncount)
{
	for(;ncount!=0;ncount--);
}


int main(void)
{
	//初始化串口
	Debug_USART_Config();

	//启动ADC转换
	Rheosta_Init();
	
	while (1)
	{
	ADC_Vol = (float)ADC_ConvertedValue/4096*(float)3.3;
		
	printf("\r\n The current AD value = 0x%04X \r\n", ADC_ConvertedValue); //打印数字量
    printf("\r\n The current AD value = %f V \r\n",ADC_Vol);//打印模拟量
		
	Delay(0xffffee);
		
	}
}




