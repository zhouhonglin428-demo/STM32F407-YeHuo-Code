#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "bsp_spi_flash.h"

uint8_t Read_Buffer[4096] = {0};
uint8_t Write_Buffer[4096] = {0};

int main(void)
{	
	uint8_t flash_id = 0;
	uint32_t i;
	
	//初始化串口USART1
	Debug_USART_Config();
	
	SPI_FLASH_Init();
	
	printf("\r\n 这是一个FLASH读写测试代码\r\n");
	
	flash_id = SPI_FLASH_Read_ID();
	
	printf("\r\n FLASH_ID = 0x%x \r\n",flash_id);
	
	printf("\r\n擦除开始");
	
	//擦除第0个扇区测试
	SPI_FLASH_Erase_Sector(4096*0);
	
	SPI_FLASH_Erase_Sector(4096*1);
	printf("\r\n擦除结束");
	
	
	//读取整个扇区数据，从第0个地址开始读
	SPI_FLASH_Read_buffer(0,Read_Buffer,4096);
	
	//校验读取到的数据是否正确
	for(i=0;i<4096;i++)
	{
		//若读取到的数据不等于0xFF,说明擦除不成功
		if(Read_Buffer[i] != 0xFF)
		printf("\r\n擦除校验失败");
	}

	
	//初始化要写入的数据
	for(i=0;i<4096;i++)
	{
		Write_Buffer[i] = 0x22;
	}
	
	printf("\r\n写入开始");
	SPI_FLASH_Page_Write(0,Write_Buffer,4096);
	printf("\r\n写入完成");	
	
	
	SPI_FLASH_Read_buffer(0,Read_Buffer,4096);	
	printf("\r\n读取到的数据");
	for(i=0;i<4096;i++)
	{
		printf("0x%02x  ",Read_Buffer[i]);
	}
	printf("\r\n写入校验成功");
	

	
	while(1)
		{	
		
		}	
}




