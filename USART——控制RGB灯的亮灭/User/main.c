#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "bsp_usart.h"
#include <stdio.h>
static void Show_Message(void);

/*
这个是USART控制RGB灯实验
所以要把所有中断函数关闭掉，包括stm32f4xx_it.c和bsp_usart.h
*/



int main(void)
{
	char ch;
	//Usart_SendByte( DEBUG_USART, 'a');
	//USsart_SendString(DEBUG_USART, "串口调试测试\n");
	//printf("Hello World!\n");
	
	/* 初始化RGB彩灯 */
	LED_GPIO_Config();
	
	Debug_USART_Config();
	
	Show_Message();
		
	while (1)
	{
	/* getchar()是一个C库函数，所以想要使用这个要重定义fgetc()函数  */
		/* 获取字符指令 */
    ch=getchar();
    printf("接收到字符：%c\n",ch);
	
	 /* 根据字符指令控制RGB彩灯颜色 */
    switch(ch)
    {
      case '1':
        LED_RED;
      break;
      case '2':
        LED_GREEN;
      break;
      case '3':
        LED_BLUE;
      break;
      case '4':
        LED_YELLOW;
      break;
      case '5':
        LED_PURPLE;
      break;
      case '6':
        LED_CYAN;
      break;
      case '7':
        LED_WHITE;
      break;
      case '8':
        LED_RGBOFF;
      break;
      default:
        /* 如果不是指定指令字符，打印提示信息 */
        Show_Message();
        break;      
    }   	
		
	}
}


/*
printf打印一堆字符显示在电脑上
所以这个是单片机发送数据给电脑
*/
static void Show_Message(void)
{
  printf("\r\n   这是一个通过串口通信指令控制RGB彩灯实验 \n");
  printf("使用  USART1  参数为：%d 8-N-1 \n",DEBUG_USART_BAUDRATE);
  printf("开发板接到指令后控制RGB彩灯颜色，指令对应如下：\n");
  printf("   指令   ------ 彩灯颜色 \n");
  printf("     1    ------    红 \n");
  printf("     2    ------    绿 \n");
  printf("     3    ------    蓝 \n");
  printf("     4    ------    黄 \n");
  printf("     5    ------    紫 \n");
  printf("     6    ------    青 \n");
  printf("     7    ------    白 \n");
  printf("     8    ------    灭 \n");  
}


/*********************************************END OF FILE**********************/

