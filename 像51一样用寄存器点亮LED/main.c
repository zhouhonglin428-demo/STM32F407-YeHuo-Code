#include "STM32f4xx.h"

void Delay(unsigned int count)
{
  for(;count != 0;count--);
}

int main (void)
{
#if 0
    //打开GPIOF的时钟RCC
    *(unsigned int *)(0x40023800+0x30) |= (1 << 5); 
    //将GPIOF设置成通用输出模式
    //#1.先让GPIOF置0，其他位保持不变
    *(unsigned int *)(0x40021400+0x00) &= ~((0x03) << (2*6));
    //#2.后面再进行写1
    *(unsigned int *)(0x40021400+0x00) |= (1 << (2*6));
    //将PF6设置成高电平
    *(unsigned int *)(0x40021400+0x14) |= (1 << 6);
    //将PF6设置成低电平
    *(unsigned int *)(0x40021400+0x14) &= ~(1 << 6);

#elif 0
    //第一步：打开GPIOF的时钟RCC
    RCC_AHB1ENR |= (1 << 5); 

    //第二步：控制GPIOF的输出方向
    //将GPIOF设置成通用输出模式
    //#1.先让GPIOF置0，其他位保持不变
    GPIOF_MODER &= ~((0x03) << (2*6));
    //#2.后面再进行写1
    GPIOF_MODER |= (1 << (2*6));

    //第三步：控制GPIO的数据输出寄存器
    //将PF6设置成高电平
    GPIOF_ODR |= (1 << 6);
    //将PF6设置成低电平
    GPIOF_ODR &= ~(1 << 6);

		//开启GPIOF的时钟
    RCC_AHB1ENR |= (1 << 5);
    //设置成通用输出模式
    //先置0，然后再修改变成01
    GPIOF_MODER &= ~((0x03) << (2*6));
    GPIOF_MODER |= (1 << (2*6) );

   // GPIOF_MODER &= ~((0x03) << (2*7));
   // GPIOF_MODER |= (1 << (2*7) );

 //   GPIOF_MODER &= ~((0x03) << (2*8));
    //GPIOF_MODER |= (1 << (2*8) );
    //设置PF6为高电平
    GPIOF_ODR |= (1 << 6);
    //设置PF7为低电平
		GPIOF_ODR &= ~(1 << 6);

		//GPIOF_ODR |= (1 << 7);
    //GPIOF_ODR &= ~(1 << 7);

    //GPIOF_ODR &= ~(1 << 8);

#elif 1
    //第一步：打开GPIOF的时钟RCC
    RCC_AHB1ENR |= (1 << 5); 

    //第二步：控制GPIOF的输出方向
    //将GPIOF设置成通用输出模式
    //#1.先让GPIOF置0，其他位保持不变
    GPIOF_MODER &= ~((0x03) << (2*6));
    GPIOF_MODER |= (1 << (2*6));
    //第三步：控制GPIO的数据输出寄存器
    //将PF6设置成高电平
    GPIOF_ODR |= (1 << 6);
    GPIOF_ODR &= ~(1 << 6);

    //设置成通用输出模式
    //先置0，然后再修改变成01
    GPIOF_MODER &= ~((0x03) << (2*6));
    GPIOF_MODER |= (1 << (2*6) );

		GPIOF_MODER &= ~((0x03) << (2*7));
		GPIOF_MODER |= (1 << (2*7) );

		GPIOF_MODER &= ~((0x03) << (2*8));
    GPIOF_MODER |= (1 << (2*8) );
		
    //设置PF6为高电平
    GPIOF_ODR |= (1 << 6);
		//GPIOF_ODR &= ~(1 << 6);

		GPIOF_ODR |= (1 << 7);
    //GPIOF_ODR &= ~(1 << 7);

		GPIOF_ODR |= (1 << 8);
    //GPIOF_ODR &= ~(1 << 8);

    while(1)
    {
    GPIOF_ODR &= ~(1 << 6);
		Delay(0xfffff);
		GPIOF_ODR |= (1 << 6);
		Delay(0xfffff);
			
		GPIOF_ODR &= ~(1 << 7);
		Delay(0xfffff);
		GPIOF_ODR |= (1 << 7);
		Delay(0xfffff);
			
		GPIOF_ODR &= ~(1 << 8);
		Delay(0xfffff);
		GPIOF_ODR |= (1 << 8);
		Delay(0xfffff);
    }



#endif
}






void SystemInit(void)
{
    
}


