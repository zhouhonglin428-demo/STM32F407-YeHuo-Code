#include "bsp_adc.h"

//定义变量，存储ADC转换出来的数字值，存放了3个通道测的数据
__IO uint16_t ADC_ConvertedValue[RHEOSTAT_NOFCHANEL]={0};


//因为使用了3个GPIO，所以都需要进行初始化(PB0 PB1 PA6)
static void ADC_GPIO_Config(void)
{
	//1.开GPIO时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB,ENABLE);
	
	//2.配置GPIO结构体参数并Init
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//选择模拟输入模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}


//初始化ADC和DMA初始化结构体
static void ADC_Mode_Config(void)
{
	/* 参考手册规定ADC1使用DMA2 通道0 数据流0 */
	//配置DMA，现在这个配置的是外部的DMA数据搬运工
	//1.开DMA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 	
	
	//2.初始化DMA
	DMA_InitTypeDef DMA_InitStructure;
	
	//3.配置DMA结构体参数并Init
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_DR_ADDR;//外设基地址也就是ADC的数据寄存器
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_ConvertedValue;//存储器的地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//数据传输方向为外设到存储器
	DMA_InitStructure.DMA_BufferSize = RHEOSTAT_NOFCHANEL;//缓冲区的大小，也就是一次传输的数据量
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不自增，因为此时只有一个外设(ADC)
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器地址自增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据大小为半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//与外设相同
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环传输模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//传输通道优先级设置为高
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;//禁用DMA FIFO，使用直连模式
	//以下配置的是FIFO大小，禁用了FIFO，配置也是没有用的
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;//选择DMA通道，通道存在于流中
	//初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	DMA_Init(DMA2_Stream0,&DMA_InitStructure);
	//使能DMA流
	DMA_Cmd(DMA2_Stream0,ENABLE);
	
	
	/* 初始化ADC Commom模式，只需配置一次 */
	//1.开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	
	//2.定义结构体
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	//3.配置结构体参数
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//设置为独立模式
	//这个DMA是ADC 内部底层的“公共 DMA 通道开关，这个是专门给多个ADC实现DMA数据共用的
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//采样时间间隔
	
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	
	/* 初始化ADC */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//ADC分辨率选择12位
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//多通道采集，需要使能扫描模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐方式选择右对齐
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//使能连续转换,配合DMA使用可以连续转换，然后储存数据
	//由于使用了软件触发，所以必须要禁止外部边缘触发
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//现在选择软件触发，随意此值随便配不影响
	ADC_InitStructure.ADC_NbrOfConversion = RHEOSTAT_NOFCHANEL;//设置转换通道数目
	
	ADC_Init(ADC1,&ADC_InitStructure);
	
	
	/* 初始化之后还要设置每个通道的转换顺序 采样周期 */
	ADC_RegularChannelConfig(ADC1,ADC_CHANNEL1,1,ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_CHANNEL2,2,ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_CHANNEL3,3,ADC_SampleTime_3Cycles);
	
	//使能ADC DMA请求,让DMA一直般数据不要停
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
	//使能ADC中的DMA
	ADC_DMACmd(ADC1,ENABLE);
	//使能ADC
	ADC_Cmd(ADC1,ENABLE);
	//开始ADC转换，软件触发
	ADC_SoftwareStartConv(ADC1);
}

void Rheostat_Init(void)
{
	ADC_GPIO_Config();
	ADC_Mode_Config();
}















