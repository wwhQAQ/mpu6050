#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
//#include "stdio.h"
void usart_init(u32 bound);
void USART1_IRQHandler(void);
void usart1_send(u8 xx);

#endif
