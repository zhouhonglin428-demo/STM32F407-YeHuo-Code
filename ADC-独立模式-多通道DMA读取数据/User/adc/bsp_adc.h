#ifndef _BSP_ADC_H
#define _BSP_ADC_H

#include "stm32f4xx.h"
void Rheostat_Init(void);

//通道1：PB0通过跳帽连接电位器，ADC_Channel_8
#define ADC_CHANNEL1      ADC_Channel_8


//通道2：PB1 通过调帽接光敏电阻，ADC_Channel_9
#define ADC_CHANNEL2      ADC_Channel_9


//通道3：PA6 悬空，可用杜邦线接3V3或者GND来实验，ADC_Channel_6
#define ADC_CHANNEL3     ADC_Channel_6



// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define ADC_DR_ADDR    			((u32)ADC1+0x4c)//这个是规则数据寄存器的数据地址

//宏定义电位器的通道数量
#define	RHEOSTAT_NOFCHANEL		3




//关于ADC使用哪个DMA，可查看参考手册DMA章节的通道选择
//ADC DMA通道宏定义
#endif /* _BSP_ADC_H */

