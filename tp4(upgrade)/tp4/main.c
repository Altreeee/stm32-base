#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "GPIO_STM32f4xx.h"

void USTART_Initialize(void){
 USART_InitTypeDef USART_InitStruct;
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
 USART_InitStruct.USART_BaudRate = 9600;
 USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
 USART_InitStruct.USART_Parity = USART_Parity_No;
 USART_InitStruct.USART_StopBits = USART_StopBits_1;
 USART_InitStruct.USART_WordLength = USART_WordLength_8b;
 USART_Init(USART3,&USART_InitStruct);
 USART_Cmd(USART3,ENABLE);
}
void GPIO_Initialize(void){
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
 GPIO_PinAF(GPIOB,10,GPIO_AF_USART3);
 GPIO_PinConfigure(GPIOB,10,GPIO_MODE_AF,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_100MHz,GPIO_PULL_UP);
 GPIO_PinAF(GPIOB,11,GPIO_AF_USART3);
  GPIO_PinConfigure(GPIOB,10,GPIO_MODE_AF,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_100MHz,GPIO_PULL_UP);
}

int USART_transmitter_empty(void){
 if(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==SET) return 1;
 else                                                 return 0;
}


void USART3_puts(char *s){
 while(*s){
  USART_SendData(USART3,*s++);
 while(USART_transmitter_empty()== 0){}
 }
}


int main(){
 uint32_t i;
 USTART_Initialize();
 GPIO_Initialize();
 while(1){
  USART_SendData(USART3,'A');
  for(i=0;1<100000000;i++);
 }
}
