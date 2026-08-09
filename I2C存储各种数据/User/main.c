#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./i2c/bsP_i2c_ee.h"
#include "./led/bsp_led.h"
#include <string.h>

//按字节来显示数据的内容
void show_bytes(uint8_t *data, uint8_t size)
{
	uint8_t i;
	printf("\r\nsize = %d,data in bytes:\r\n",size);

	for(i=0;i<size;i++)
	{
		printf("0x%02x ",*data);
		data++;//指向下一个数据
	}
}

uint8_t test_8bit = 0x02;

uint32_t test_32bit = 0x12345678;//小端存储格式

float test_float = 1.2;
double test_double  = 1.2;
char cn_test[] = "中文";
char eng_test[] = "abcd";

uint8_t read_buffer[30];

int main(void)
{
	LED_GPIO_Config();
		
	LED_BLUE;
	 /*初始化USART1*/
	Debug_USART_Config(); 

	printf("\r\n 这是一个EEPROM存储各种数据的实验 \r\n");

	/* I2C 外设初(AT24C02)始化 */
	I2C_EE_Init();
	
	//字节数据
	printf("\r\n字节数据:");
	show_bytes( &test_8bit,sizeof(test_8bit) );
	
	
	//4字节数据
	printf("\r\n4字节数据:");
	printf("\r\n数据:0x%02x",test_32bit);
	show_bytes( (uint8_t *)&test_32bit,sizeof(test_32bit) );
	
	//浮点型数据	
	printf("\r\n浮点型数据:");
	show_bytes( (uint8_t *)&test_float,sizeof(test_float) );
	
	//double浮点型数据
	printf("\r\ndouble浮点型数据:");
	printf("\r\n数据:%f",test_double);
	show_bytes( (uint8_t *)&test_double,sizeof(test_double) );
	
	//中文数据,strlen计算字符串的长度,strlen不包括'\0'的长度
	printf("\r\n中文数据:");
	show_bytes( (uint8_t *)cn_test,strlen(cn_test)+1 );
	
	//英文数据
	printf("\r\n英文数据:");
	show_bytes( (uint8_t *)eng_test,strlen(eng_test)+1 );		
	
	//存储数据到EEPROM
	printf("\r\n 存储double数据到EEPROM \r\n");
	I2C_EE_BufferWrite((uint8_t *)&test_double,0,sizeof(test_double));
	//写入数据到EEPROM之后要等待I2C_EE_BufferWrite()这个函数里面带有等待函数了
	
	I2C_EE_BufferRead((uint8_t *)read_buffer,0, sizeof(test_double));
	
	printf("\r\n 从EEPROM读取出来的test_double数据: \r\n");
	show_bytes(read_buffer,sizeof(test_double));
	
	printf("\r\n数据:%f",*(double *)read_buffer);
	
	//存储double数据到EEPROM
	printf("\r\n 存储整型数据到EEPROM \r\n");
	I2C_EE_BufferWrite((uint8_t *)&test_32bit,0,sizeof(test_32bit));
	
	I2C_EE_BufferRead((uint8_t *)read_buffer,0, sizeof(test_32bit));
	
	printf("\r\n 从EEPROM读取出来的test_32bit数据: \r\n");
	show_bytes(read_buffer,sizeof(test_32bit));
	
	printf("\r\n数据:0x%02x",*(uint32_t *)read_buffer);
	
	
	//存储英文数据到EEPROM
	printf("\r\n 存储英文数据到EEPROM \r\n");
	I2C_EE_BufferWrite((uint8_t *)&eng_test,0,strlen(eng_test)+1);
	
	I2C_EE_BufferRead((uint8_t *)read_buffer,0, strlen(eng_test)+1);
	
	printf("\r\n 从EEPROM读取出来的eng_test数据: \r\n");
	show_bytes(read_buffer,strlen(eng_test)+1);
	
	printf("\r\n数据:%s",read_buffer);
	
	while (1)
	{      
	}  

}






