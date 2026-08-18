#include "bsp_adc.h"


//因为存放2个ADC的值，所以数组大小为2
//作为 DMA 搬运数据的即内存缓冲区，定义变量，将ADC转换数据安全地存下来，
volatile uint16_t ADC_ConvertedValue[2];

//初始化PB1和PB2
static void ADC_GPIO_Config(void)
{
	//1.开时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//2.定义结构体
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//采用模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	//3.配置结构体参数并Init
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

}

static void ADC_Mode_Config(void)
{
	/* 初始化DMA */
	//1.开DMA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 	
	
	//2.初始化DMA
	DMA_InitTypeDef DMA_InitStructure;
	
	//3.配置DMA结构体参数并Init
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC_CDR_ADDR;//外设基地址也就是ADC的数据寄存器
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC_ConvertedValue;//存储器的地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//数据传输方向为外设到存储器
	//虽然采集了 2 个 ADC 通道，但它们在硬件上共用了同一个 CDR 寄存器，这个寄存器地址又是固定的
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
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
	//使能DMA
	DMA_Cmd(DMA2_Stream0,ENABLE);	


	/* 初始化ADC Commom模式，只需配置一次 */
	//1.开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
	
	//2.定义结构体
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	//3.配置结构体参数
	ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;//设置规则
	//使用DMA模式1
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//采样时间间隔
	
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	
	/* 初始化ADC1和ADC2 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2 , ENABLE);
	
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//ADC分辨率选择12位
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//此时ADC1只有1个通道，ADC2也是只有一个通道，所以不需要扫描
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐方式选择右对齐
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//使能连续转换,配合DMA使用可以连续转换，然后储存数据
	//由于使用了软件触发，所以必须要禁止外部边缘触发
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//现在选择软件触发，随意此值随便配不影响
	ADC_InitStructure.ADC_NbrOfConversion = 1;//设置转换通道数目
	
	
	/* 两个ADC都是共用的 */
	/* 初始化ADC1 */
	ADC_Init(ADC1,&ADC_InitStructure);
	//配置ADC1规则通道转换顺序
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_3Cycles);
	
	/* 初始化ADC2 */
	ADC_Init(ADC2,&ADC_InitStructure);
	ADC_RegularChannelConfig(ADC2,ADC_Channel_9,1,ADC_SampleTime_3Cycles);
	
	
	//使能多重ADCADC DMA请求,让DMA一直般数据不要停
	ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
	
	//使能ADC
	ADC_Cmd(ADC1,ENABLE);
	ADC_Cmd(ADC2,ENABLE);
	//开始ADC转换，软件触发，触发只需要触发主ADC1就行了
	ADC_SoftwareStartConv(ADC1);
}

void Rheostat_Init(void)
{
	ADC_GPIO_Config();
	ADC_Mode_Config();
}





