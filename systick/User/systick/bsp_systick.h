#ifndef _SYSTICK_H
#define _SYSTICK_H

#include "stm32f4xx.h"
void Delay_ms(__IO u32 nTime);

extern __IO uint32_t TimingDelay;

void SysTick_Init(void);




















#endif  /* _SYSTICK_H */










