/* brief:EEPROM驱动程序 */


#include "bsp_iic_ee.h"

/*
实现往EEPROM写入数据。
1.初始化硬件I2C
2.编写往EEPROM传输数据的函数（包括读和写）
3.利用读写函数往EEPROM写入数据，并读取数据进行校验
*/

#define TIME_OUT			0x000FFFFFF

uint32_t count_wait =  TIME_OUT;

//static是静态函数，表示此函数或变量只能作用于当前文件
static uint8_t Error_CallBack(uint8_t code);

/* EEPROM_IIC_Config初始化GPIO和I2C模式 */
void EEPROM_IIC_Config(void)
{
	/* I2C必须使用开漏模式 */
	/* I2C外设SCL连接到PB8，SDA连接到PB9 */
	/* 第一步：开GPIOB时钟和I2C1时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	
	/* 第二步：引脚复用映射 */
	/* 连接GPIO到I2C */
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_I2C1);
	
	/* 第三步：配置GPIO并Init(每个引脚都需要配置结构体参数) */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 |  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	
	/* 第四步：配置I2C并Init */
	I2C_InitTypeDef I2C_InitStructure;
	
	//使能或关闭响应
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	//指定地址的长度
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	//设置SCL时钟频率
	I2C_InitStructure.I2C_ClockSpeed = EEPROM_I2C1_ClockSpeed;
	//指定时钟占空比
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	//指定I2C模式
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C ;
	//指定自身的I2C设备地址
	I2C_InitStructure.I2C_OwnAddress1 = 0x78;//随便一个设备地址，反正是主机的地址，不要跟其他设备地址相同就ok
	
	I2C_Init(I2C1,&I2C_InitStructure);
	
	
	/* 第五步：使能I2C */
	I2C_Cmd(I2C1,ENABLE);
}



/* 编写往EEPROM传输数据的函数（包括读和写） */
/* addr:要写入存储单元的地址 */
/* data:要写入的数据 */
//return:0表示正常，非0为失败
uint8_t EEPROM_Byte_Write(uint8_t addr,uint8_t data)
{
	/* 产生起始信号 */
	I2C_GenerateSTART(I2C1,ENABLE);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV5事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(1);
		}
	}
	
	
	/* 要发送的EEPROM设备地址，并设置为写方向 */
	I2C_Send7bitAddress(I2C1,EEPROM_I2C1_ADDR,I2C_Direction_Transmitter);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV6事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(2);
		}
	}

	
	/* 发送要写入存储单元的地址 */
	I2C_SendData(I2C1,addr);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV8_2事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(3);
		}
	}
	
	/* 发送要写入的数据 */
	I2C_SendData(I2C1,data);
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV8_2事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(4);
		}
	}
	
	/* 产生结束信号 */
	I2C_GenerateSTOP(I2C1,ENABLE);

	
	/* 因为stm32的操作速度非常快 ，但是I2C的速度相比于I2C会慢 */
	/* 例如stm32发送完起始信号之后，立马就去发送EEPROM地址了。所以要等待I2C进行响应 */

	return Wait_For_Standby();
}




/* addr:要写入存储单元的首地址 */
/* data:要写入的数据的指针 */
/* size:要写入多少个数据,(size <= 8) */
//return:0表示正常，非0为失败
uint8_t EEPROM_Page_Write(uint8_t addr,uint8_t *data, uint8_t size)
{
	/* 产生起始信号 */
	I2C_GenerateSTART(I2C1,ENABLE);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV5事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(12);
		}
	}
	
	
	/* 要发送的EEPROM设备地址，并设置为写方向 */
	I2C_Send7bitAddress(I2C1,EEPROM_I2C1_ADDR,I2C_Direction_Transmitter);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV6事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(13);
		}
	}

	
	/* 发送要写入存储单元的地址 */
	I2C_SendData(I2C1,addr);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV8_2事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(14);
		}
	}
	
	while(size--)
	{
	/* 发送要写入的数据 */
	I2C_SendData(I2C1,*data);
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV8_2事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
			{
			return Error_CallBack(15);
			}
		} 
		
		data++;
	}

	/* 产生结束信号 */
	I2C_GenerateSTOP(I2C1,ENABLE);

	
	/* 因为stm32的操作速度非常快 ，但是I2C的速度相比于I2C会慢 */
	/* 例如stm32发送完起始信号之后，立马就去发送EEPROM地址了。所以要等待I2C进行响应 */

	
	/* 等待写入完成 */
	return Wait_For_Standby();
}





/* addr:要写入存储单元的首地址 */
/* data:要写入的数据的指针 */
/* size:要写入多少个数据 */
//return:0表示正常，非0为失败
uint8_t EEPROM_Buffer_Write(uint8_t addr,uint8_t *data, uint16_t size)
{
	uint8_t single_addr = addr%EEPROM_PAGE_SIZE;
	
	uint8_t num_of_page = size/EEPROM_PAGE_SIZE;
	uint8_t single_byte = size%EEPROM_PAGE_SIZE;
	if(single_addr == 0)
	{
		while(num_of_page--)
		{
		//当size<=8,调用页写入函数
		EEPROM_Page_Write(addr,data,EEPROM_PAGE_SIZE);
		
		//等待写入完成
		Wait_For_Standby();
		addr += EEPROM_PAGE_SIZE;
		data += EEPROM_PAGE_SIZE;
		}
		
		
		//当size<=8,调用页写入函数
		EEPROM_Page_Write(addr,data,single_byte);
		
		//等待写入完成
		Wait_For_Standby();
	}
	
	return 0;
}




/*
函数内部改不了外部变量的值。
要想在函数内修改外部变量，就得把它的地址（指针）传进来，顺着地址改它内存里存的数据
*/
//读取EEPROM存储单元的数据
//addr:要读取的存储单元地址
//data:用来存储 读取到的数据的 指针
//return:0表示正常，非0为失败
uint8_t EEPROM_Random_Read(uint8_t addr,uint8_t *data)
{
	/* 产生起始信号 */
	I2C_GenerateSTART(I2C1,ENABLE);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV5事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(5);
		}
	}
	
	
	/* 要发送的EEPROM设备地址，并设置为写方向 */
	I2C_Send7bitAddress(I2C1,EEPROM_I2C1_ADDR,I2C_Direction_Transmitter);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV6事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(6);
		}
	}

	
	/* 发送要读取存储单元的地址 */
	I2C_SendData(I2C1,addr);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV8_2事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(7);
		}
	}

	
	
	/* 产生第二次起始信号 */
	I2C_GenerateSTART(I2C1,ENABLE);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV5事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(8);
		}
	}
	
/*-----------------------------------------读方向------------------------------------------------*/		
	/* 要发送的EEPROM设备地址，并设置为读方向 */
	I2C_Send7bitAddress(I2C1,EEPROM_I2C1_ADDR_R,I2C_Direction_Receiver);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV6事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(9);
		}
	}
	
	/* 
	前面的while等待实际就包含了应答,是由stm发出信号,EEPROM应答
	现在这个是EEPROM接收到数据，stm32读取，此时应答由stm32发出
	这里选择DISABLE是stm32告诉EEPROM传输够了,这是我读取的最后一个字节不需要再发送
	*/
	/* stm32作出非应答信号 */
	/* 在接收到数据之前直接非应答，把NACK信号传出去 */
	I2C_AcknowledgeConfig(I2C1,DISABLE);

	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV7事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(10);
		}
	}
	
	/* 接收数据 */
	/* 接收到的数据保存在*data */
	*data = I2C_ReceiveData(I2C1);
	
	/* 产生结束信号 */
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	/* 因为stm32的操作速度非常快 ，但是I2C的速度相比于I2C会慢 */
	/* 例如stm32发送完起始信号之后，立马就去发送EEPROM地址了。所以要等待I2C进行响应 */
	
	return 0;
}



//addr:要读取的存储单元的首地址
//data:用来存储 读取到的数据的 指针
//return:0表示正常，非0为失败
//size:要读取数据的个数
uint8_t EEPROM_Buffer_Read(uint8_t addr,uint8_t *data,uint16_t size)
{
	/* 产生起始信号 */
	I2C_GenerateSTART(I2C1,ENABLE);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV5事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(16);
		}
	}
	
	
	/* 要发送的EEPROM设备地址，并设置为写方向 */
	I2C_Send7bitAddress(I2C1,EEPROM_I2C1_ADDR,I2C_Direction_Transmitter);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV6事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(17);
		}
	}

	
	/* 发送要读取存储单元的地址 */
	I2C_SendData(I2C1,addr);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV8_2事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(18);
		}
	}

	
	
	/* 产生第二次起始信号 */
	I2C_GenerateSTART(I2C1,ENABLE);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV5事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(19);
		}
	}
	
/*-----------------------------------------读方向------------------------------------------------*/		
	/* 要发送的EEPROM设备地址，并设置为读方向 */
	I2C_Send7bitAddress(I2C1,EEPROM_I2C1_ADDR_R,I2C_Direction_Receiver);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV6事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(20);
		}
	}

	
	/* 表示读取多个数据 */
	while(size--)
	{
		if(size == 0)
		{
			/* 作出非应答信号 */
		I2C_AcknowledgeConfig(I2C1,DISABLE);
		}
		else
		{
			/* 作出应答信号 */
		I2C_AcknowledgeConfig(I2C1,ENABLE);
		}
		/* 重置 count_wait */
		count_wait = TIME_OUT;
		/* 等待EV7事件直到成功(如果不成功将会一直卡在这) */
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)
		{
			count_wait--;
			if(count_wait == 0)
			{
				return Error_CallBack(21);
			}
		}
		
		/* 接收数据 */
		/* 接收到的数据保存在*data */
		*data = I2C_ReceiveData(I2C1);
		data++;
	}
	
	/* 产生结束信号 */
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	/* 因为stm32的操作速度非常快 ，但是I2C的速度相比于I2C会慢 */
	/* 例如stm32发送完起始信号之后，立马就去发送EEPROM地址了。所以要等待I2C进行响应 */
	
	return 0;
	
	
}







//等待EEPROM内部写入操作完成
//为0表示正常等待完成，非0表示等待不到响应信号
uint8_t Wait_For_Standby(void)
{
	uint32_t check_count = 0xFFFFF;
	while(check_count--)
	{	
	/* 产生起始信号 */
	I2C_GenerateSTART(I2C1,ENABLE);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV5事件直到成功(如果不成功将会一直卡在这) */
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS)
	{
		count_wait--;
		if(count_wait == 0)
		{
			return Error_CallBack(11);
		}
	}
	
	
	/* 要发送的EEPROM设备地址，并设置为写方向 */
	I2C_Send7bitAddress(I2C1,EEPROM_I2C1_ADDR,I2C_Direction_Transmitter);
	
	/* 重置 count_wait */
	count_wait = TIME_OUT;
	/* 等待EV6事件直到成功(如果不成功将会一直卡在这) */
	while(count_wait--)
		{
			//若检测到响应，说明内部写时序完成，跳出等待函数
			if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == SUCCESS)
			{
				
				/* 退出前，停止本次通讯 */
				I2C_GenerateSTOP(I2C1,ENABLE);				
				return 0;
			}
		}
	}
	
	/* 退出前，停止本次通讯 */
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	/* 等待写入完成 */
	return Wait_For_Standby();
}

//code:错误编码
uint8_t Error_CallBack(uint8_t code)
{
	printf("I2C error occur,code=%d\n",code);
	return code;
}




