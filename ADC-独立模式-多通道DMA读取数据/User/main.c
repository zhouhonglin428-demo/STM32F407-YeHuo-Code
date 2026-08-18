#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "bsp_usart.h"
#include <stdio.h>
#include "bsp_adc.h"

//延时函数
static void Delay(__IO uint32_t ncount)
{
	for(;ncount!=0;ncount--);
}

//ADC转换的电压值通过DMA传到SRAM
extern __IO uint16_t ADC_ConvertedValue[RHEOSTAT_NOFCHANEL];

//再定义变量，用来保存转换计算后得到的电压值
float ADC_ConvertedValueLocal[RHEOSTAT_NOFCHANEL] ={0}; 

int main(void)
{
	//初始化串口
	Debug_USART_Config();

	//初始化ADC
	Rheostat_Init();
	
	
	while (1)
	{
		ADC_ConvertedValueLocal[0] =(float) ADC_ConvertedValue[0]/4096*(float)3.3;
		ADC_ConvertedValueLocal[1] =(float) ADC_ConvertedValue[1]/4096*(float)3.3;
		ADC_ConvertedValueLocal[2] =(float) ADC_ConvertedValue[2]/4096*(float)3.3;
		
		printf("\r\n CH1_PB0 value = %f V \r\n",ADC_ConvertedValueLocal[0]);
		printf("\r\n CH2_PB1 value = %f V \r\n",ADC_ConvertedValueLocal[1]);
		printf("\r\n CH3_PA6 value = %f V \r\n",ADC_ConvertedValueLocal[2]);
		
		printf("\r\n\r\n");
		Delay(0xffffff);  	
	}
}




