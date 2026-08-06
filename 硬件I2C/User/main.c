#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "bsp_iic_ee.h"


#define		TEST_SIZE		256
int main(void)
{	
	uint8_t data[TEST_SIZE];
	uint8_t buff[TEST_SIZE];
	uint16_t i;
	/*初始化USART 配置模式为 115200 8-N-1，中断接收*/
	Debug_USART_Config();

	/* 发送一个字符串 */
	printf("这是一个EEPROM读写实验\n");

	/* 给buff赋值 */
	for(i=0;i<TEST_SIZE;i++)
	{
		buff[i] = i;
	}
	
	EEPROM_IIC_Config();
	printf("初始化完成\n");
	
	//写入测试
	EEPROM_Byte_Write(0x01,0x12);
	
	EEPROM_Random_Read(0x01,data);
	
	printf("单字节读写测试结束,data=0x%x\n",data[0]);
	
	
	//数组名就是一个指针
	
	
	EEPROM_Buffer_Write(0x00,buff,TEST_SIZE);
	
	
	EEPROM_Buffer_Read(0x00,data,TEST_SIZE);
	
	printf("页读写测试结束,读取到的数据为:\n");
	
	for(i=0;i<TEST_SIZE;i++)
	{
		printf("0x%02x ",data[i]);
	}
	

	while(1)
		{	
		
		}	
}



/*********************************************END OF FILE**********************/

