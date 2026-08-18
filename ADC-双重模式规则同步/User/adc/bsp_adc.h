#ifndef _BSP_ADC_H
#define _BSP_ADC_H

#include "stm32f4xx.h"

void Rheostat_Init(void);

/* ADC1使用PB0,ADC2使用PB1,每个ADC分别采集一个通道 */

/* 通道一 */

//ADC各自采集数据的时候，采集到的数据还是会放在各自的数据寄存器里
//ADC CDR寄存器宏定义ADC转换后的数字值则存放在这里  DMA传输的时候是使用CDR通用数据寄存器
#define ADC_CDR_ADDR		(uint32_t)0x40012308

/* 通道一 */

/* 通道二 */

/* ADC DMA通道宏定义，这里选择DMA传输,此时双重模式应该选择主ADC(ADC1)的DMA */
#define ADC_DMA_Channel		DMA_Channel_0
#define ADC_DMA_Stream

#endif /* _BSP_ADC_H */

