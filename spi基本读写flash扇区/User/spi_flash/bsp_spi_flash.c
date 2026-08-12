/* brief:EEPROM驱动程序 */


#include "bsp_spi_flash.h"

/*
1.初始化相关引脚及SPI工作模式
2.编写基本的读写单个字节的流程函数
3.利用2的函数来组成各种命令
4.利用命令读写FLASH并校验数据
*/
static uint8_t SPI_Error_CallBack(uint8_t code);

static __IO uint32_t  SPITIMEOUT = SPI_FLAG_TIMEOUT;
static __IO uint32_t  SPITimeout = SPI_LONG_TIMEOUT;  
//初始化SPI相关引脚
//static是静态函数，表示此函数或变量只能作用于当前文件
static void SPI_GPIO_Config(void)
{


	//第一步：开GPIO时钟
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
	
	/* 第二步：引脚复用映射 */
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
	
	/* 第三步：配置GPIO并Init(每个引脚都需要配置结构体参数) */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	
	//初始化片选引脚PG6，使用软件控制CS引脚，则GPIO就是他自己的功能配置成输出模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
}


//初始化SPI工作模式
static void SPI_Mode_Config(void)
{
	RCC_APB2PeriphClockCmd(SPI_CLK,ENABLE);
	
	//flash支持模式0和模式3
    SPI_InitTypeDef  SPI_InitStructure; 

	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //最高的波特率，配置成二分频
	//一般选择模式0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	
	SPI_InitStructure.SPI_CRCPolynomial = 7;//不使用CRC校验
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//数据帧长度一般配置为8个数据位
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//数据传输方向配置为双线全双工
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主从模式选择为主机
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//片选信号控制方式，选择软件控制片选信号

	SPI_Init(SPI1,&SPI_InitStructure);
    //使能SPI外设
	SPI_Cmd(SPI1,ENABLE);
}



//SPI外设(flash)初始化
void SPI_FLASH_Init(void)
{
	SPI_GPIO_Config();
	
	SPI_Mode_Config();
}



/*
通过SPI发送一个字节
return:返回值是接收到的数据
data:要发送的数据
*/
uint8_t SPI_FLASH_Byte_Write(uint8_t data)
{
	uint8_t re_data;
	
	//等待TXE标志为1，即发送缓冲区为空
	SPITimeout = SPI_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) != SET)
	{
		//如果TXE没有被置1，则会一直进入循环，然后SPITimeout将会一次次的减1
		//直到SPITimeout为0，则表示超时了返回SPI_Error_CallBack()函数
		if(SPITimeout-- == 0)
			return SPI_Error_CallBack(1);
	}
	
	//将数据写入数据寄存器
	SPI_I2S_SendData(SPI1,data);
	
	//等待RXNE标志位置1,（说明时钟发送完毕，且接收缓冲区已收到对方返回的数据）
	//重置SPITimeout，不然就会跟前面的SPITimeout共用一个数据了
	SPITimeout = SPI_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) != SET)
	{
		if(SPITimeout-- == 0)
			return SPI_Error_CallBack(2);
	}
	
	re_data = SPI_I2S_ReceiveData(SPI1);
	
	//返回读取到的数据
	return re_data;
	
}

//读ID0 - ID7
uint8_t SPI_FLASH_Read_ID(void)
{
	uint8_t id;
	
	//控制片选引脚，CS由高电平变为低电平
	SPI_FLASH_LOW();
	
	//指令代码
	SPI_FLASH_Byte_Write(W25X_DeviceID);
	//发送3个垃圾字节
	SPI_FLASH_Byte_Write(DUMMY);
	SPI_FLASH_Byte_Write(DUMMY);
	SPI_FLASH_Byte_Write(DUMMY);
	
	/*
	接收读取到的内容（想要读取一个字节的内容的时候，就需要写入一个字节由此主机产生SCK时钟）
	主要就是触发时钟的运行，然后flash在时钟下就会返回一个数据,
	数据存储在re_data = SPI_I2S_ReceiveData(SPI1)
	*/
	id = SPI_FLASH_Byte_Write(DUMMY);
	
	//控制片选引脚拉高为高电平
	SPI_FLASH_HIGH();
	
	
	return id;
}


//擦除扇区，擦除扇区地址要24位的数据，所以需要比较大的数据来存储要擦除的地址
//需要告诉要擦除哪个地址的扇区，所以函数需要传入参数，函数没有返回值，所以不需要return
//擦除扇区没有返回值
//addr:必须要对其要擦除扇区的首地址
//在写入或者擦除的命令时，一定要先执行Write Enable指令
void SPI_FLASH_Erase_Sector(uint32_t addr)
{	
	//使能Write_Enable
	SPI__FLASH_Write_Enable();
	
	//拉低片选引脚电平
	SPI_FLASH_LOW();
	
	//发送指令代码
	SPI_FLASH_Byte_Write(W25X_SectorErase);
	
	//发送要擦除扇区的首地址
	//第一个发送字节是高8位，把这个32位的地址的最高8位发送过去，则是右移16位
	SPI_FLASH_Byte_Write((addr >> 16) & 0xff);
	//第二个是中8位
	SPI_FLASH_Byte_Write((addr >> 8) & 0xff);
	//第三个是低8位
	SPI_FLASH_Byte_Write((addr >> 0) & 0xff);
	
	//控制片选引脚拉高为高电平
	SPI_FLASH_HIGH();
	
	//检测busy位，来确保擦除操作的完成
	SPI__FLASH_Wait_For_Standby();
}



//写入数据命令用Page Program
//addr:要写入的数据起始地址
//buffer:写入数据的指针
//size:要写入数据的个数,Page Program写入的数据个数不能超过256
void SPI_FLASH_Page_Write(uint32_t addr,uint8_t *buffer,uint32_t size)
{	
	uint32_t count = 0;
	while(size--)
	{
		count++;
		//下面if这一部分表示每当写完256个字节后，就得重新使能写指令，
		//就得重新使能写指令，拉低CS电平，发送falsh指令，再发送3个地址
		//因为page Program写完256个字节之后就得重新操作
		if(count == 1 || (count%256) == 1 || (addr%4096) == 0 )
		{
			//结束上一次的页写入指令
			//写完256个字节，进入第二页写入的时候之前就得先拉高CS电平,等待内部时序写入完成
			SPI_FLASH_HIGH();
			
			//等待内部时序写入完成
			SPI__FLASH_Wait_For_Standby();
			
			//使能Write_Enable
			SPI__FLASH_Write_Enable();
			
			//拉低片选引脚电平
			SPI_FLASH_LOW();
			
			//发送指令代码
			SPI_FLASH_Byte_Write(W25X_PageProgram);
			
			//发送要写入数据的首地址
			//第一个发送字节是高8位，把这个32位的地址的最高8位发送过去，则是右移16位
			SPI_FLASH_Byte_Write((addr >> 16) & 0xff);
			//第二个是中8位
			SPI_FLASH_Byte_Write((addr >> 8) & 0xff);
			//第三个是低8位
			SPI_FLASH_Byte_Write((addr >> 0) & 0xff);
		}
			
		//发送写入数据
		//buffer实际上是存数据的指针，*buffer解应用，取出实际存放的数据
		//这一部分是发送写入一个字节的数据
		SPI_FLASH_Byte_Write(*buffer);
		buffer++;
		addr++;
	}
	
	//执行完写入指令，才执行以下的结束指令
	//控制片选引脚拉高为高电平
	SPI_FLASH_HIGH();
	
	//检测busy位，来确保擦除操作的完成
	SPI__FLASH_Wait_For_Standby();
}




//读取数据
//因为flash地址数据为24位，存储只能用比24位要大的，所以要用32位的整型数据去存储，
//addr:要读取的数据起始地址（Flash地址为 24 位，采用uint32_t存储）
//buffer:存储读取到的数据的指针
//size:要读取数据的个数
void SPI_FLASH_Read_buffer(uint32_t addr,uint8_t *buffer,uint32_t size)
{
	//拉低片选引脚电平
	SPI_FLASH_LOW();
	
	//发送指令代码
	SPI_FLASH_Byte_Write(W25X_ReadData);
	
	//发送要擦除扇区的首地址
	//第一个发送字节是高8位，把这个32位的地址的最高8位发送过去，则是右移16位
	SPI_FLASH_Byte_Write((addr >> 16) & 0xff);
	//第二个是中8位
	SPI_FLASH_Byte_Write((addr >> 8) & 0xff);
	//第三个是低8位
	SPI_FLASH_Byte_Write((addr >> 0) & 0xff);
	
	while(size--)
	{
		//这里接收数据的时候，主机还需要发送一个dummy字节，产生时钟，同时才能接收数据
		//所以这里的参数dummy就代表了主机发送dummy字节
		*buffer = SPI_FLASH_Byte_Write(DUMMY);
		buffer++;
	}	
	
	//控制片选引脚拉高为高电平
	SPI_FLASH_HIGH();
	
	//检测busy位，来确保擦除操作的完成
	SPI__FLASH_Wait_For_Standby();	
}




/*-------------------------------------函数封装----------------------------------------*/
void SPI__FLASH_Write_Enable(void)
{
	//控制片选引脚，CS由高电平变为低电平
	SPI_FLASH_LOW();
	
	//发送指令代码
	SPI_FLASH_Byte_Write(W25X_WriteEnable);
	
	//控制片选引脚拉高为高电平
	SPI_FLASH_HIGH();
}


//等待busy，直到空闲状态
void SPI__FLASH_Wait_For_Standby(void)
{
	uint8_t status;
	//控制片选引脚，CS由高电平变为低电平
	SPI_FLASH_LOW();
	//先发送指令代码，后面读取数据就是循环了
	SPI_FLASH_Byte_Write(W25X_ReadStatusReg);
	
	//不希望一直死循环，导致程序不可用，所以加个程序等待超时功能
	SPITimeout = SPI_FLAG_TIMEOUT;
	while(1)
	{
		status = SPI_FLASH_Byte_Write(DUMMY);
		//如果if条件成立(busy为0)，则表示空闲状态
		//&0x01作用：status 的高 7 位全部变成0，只保留最低的那 1 位
		if((status & 0x01) == 0) 
			break;
		//若SPITimeout为0，则说明检测SPITimeout次busy还是忙碌，则跳出循环，告诉出错了
		if((SPITimeout--) == 0)
		{
			//因为这个函数是空函数，所以不能return错误代码
			SPI_Error_CallBack(3);
			break;
		}
	}
	
	//控制片选引脚拉高为高电平
	SPI_FLASH_HIGH();
}




//返回0，表示SPI读取错误
uint8_t SPI_Error_CallBack(uint8_t code)
{
	printf("SPI error occur,code=%d\n",code);
	return 0;
}

	






