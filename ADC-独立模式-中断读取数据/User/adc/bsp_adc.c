#include "bsp_adc.h"


//初始化ADC的GPIO
static void ADC_GPIO_Config(void)
{
	//开GPIOB时钟
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOB,ENABLE);
	
	//配置GPIO结构体成员并Init
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = ADC_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = ADC_GPIO_Mode;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

//初始化ADC模式
static void ADC_Mode_Config(void)
{
	//开ADC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	
	//初始化并配置ADC_Common结构体，只需要配一次，因为同时管ADC1，2，3
	ADC_CommonInitTypeDef	ADC_CommonInitStructure;
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//选择ADC为独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//设置ADC为四分频
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//禁止DMA直接访问
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;//采样时间间隔，现在随便设的
	
	ADC_CommonInit(&ADC_CommonInitStructure);
	

	//初始化并配置ADC结构体成员并Init
	ADC_InitTypeDef		ADC_InitStructure;
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//采用12位ADC最大分频率
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//单通道采集不需要扫描扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//采用连续转换，就是不断的采集
	ADC_InitStructure.ADC_ExternalTrigConvEdge = DISABLE;//禁止外部边沿触发
	//现在使用的是软件触发，所以此值可随便配
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//外部触发通道
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//使用右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;//设置转换通道为1个
	
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//配置ADC通道转换顺序 具体使用的通道 通道顺序 采样周期
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_56Cycles);//使用了ADC规则通道的转换顺序
	
	//ADC结束之后产生中断，在中断服务程序中读取转换值
	//使能ADC中断，这个就是转换之后产生中断，然后进入中断服务函数，这个中断与注入通道一点关系没有
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);//使用转换完成中断
	//使能ADC
	ADC_Cmd(ADC1,ENABLE);
	//开始ADC中断，软件触发
	ADC_SoftwareStartConv(ADC1);
}


/*
配置NVIC，就是指定中断源.将中断源传到内核里面，最后在内核访问中断服务函数
*/
//设置中断优先级
static void ADC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruture;

	NVIC_InitStruture.NVIC_IRQChannel = ADC_IRQn;//设置中断源
	NVIC_InitStruture.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruture.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruture.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruture);
}

void Rheosta_Init(void)
{
	ADC_GPIO_Config();
	ADC_Mode_Config();
	ADC_NVIC_Config();
}

//编写ADC转换完成中断服务函数







