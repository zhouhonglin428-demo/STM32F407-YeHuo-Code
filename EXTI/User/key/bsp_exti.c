#include "bsp_exti.h"

static void NVIC_Configuration(void)
{
	//定义NVIC初始化结构体
	NVIC_InitTypeDef NVIC_InitStruct;
	//配置NVIC为优先级组1
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* 填入按键一NVIC的结构体参数 */
	//配置中断源：按键一
	NVIC_InitStruct.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
	//配置抢占优先级：1
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	//配置子优先级：1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	//使能中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/* 填入按键一NVIC的结构体参数 */
	//配置中断源：按键一
	NVIC_InitStruct.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
	//使能中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}






void EXTI_Key_Config()
{
	//定义外设初始化结构体
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	/*第一步：初始化需要连接到EXTI线的GPIO */
	
	/* 开启按键GPIO端口的时钟 */
	RCC_AHB1PeriphClockCmd(KEY1_INT_GPIO_CLK | KEY2_INT_GPIO_CLK,ENABLE);

	/* 初始化按键一的GPIO */
	
	/* 选择按键一的引脚 */
	GPIO_InitStruct.GPIO_Pin = KEY1_INT_GPIO_PIN;
	/* 设置引脚为输入模式 */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	/* 设置既不上拉也不下拉模式 */
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	/* 将配置好的结构体参数初始化按键 */
	GPIO_Init(KEY1_INT_GPIO_PORT,&GPIO_InitStruct);
	
	/* 初始化按键二的GPIO */
	//选择按键二的引脚
	GPIO_InitStruct.GPIO_Pin = KEY2_INT_GPIO_PIN;
	//其他与上面相同
	GPIO_Init(KEY2_INT_GPIO_PORT,&GPIO_InitStruct);
	
	/*第二步：初始化EXTI */
	//使能SYSCFG时钟，使用GPIO外部中断必须开启SYSCFG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	//连接EXTI中断源到key1引脚
	SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,KEY1_INT_EXTI_PINSOURCE);
	//连接EXTI中断源到key1引脚
	SYSCFG_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,KEY2_INT_EXTI_PINSOURCE);
	
	
	/* 配置按键一的EXTI */
	//选择EXTI中断源
	EXTI_InitStruct.EXTI_Line = KEY1_INT_EXTI_LINE;	
	//配置成中断模式
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;	
	//上升沿触发
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	//使能中断/事件线
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	/* 配置按键二的EXTI */
	//选择EXTI中断源
	EXTI_InitStruct.EXTI_Line = KEY2_INT_EXTI_LINE;
	//配置成中断模式
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	//选择下降沿触发
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	//使能中断/事件线
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	
	
	/*第三步：配置NVIC */
	NVIC_Configuration();
	
	/*第四步：编写中断服务函数 */
}











