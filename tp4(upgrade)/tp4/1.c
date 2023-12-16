#include "stm32f4xx_rcc.h"              
#include "stm32f4xx_usart.h"            
#include "GPIO_STM32F4xx.h"

void USART3_Initialize(void){
 USART_InitTypeDef  Usart_init_struct; 
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
 Usart_init_struct.USART_BaudRate = 9600; 
 Usart_init_struct.USART_StopBits = USART_StopBits_1; 
 Usart_init_struct.USART_WordLength = USART_WordLength_8b; 
 Usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 Usart_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 Usart_init_struct.USART_Parity = USART_Parity_No;
 USART_Init(USART3,&Usart_init_struct); 
  USART_Cmd(USART3,ENABLE); 
}

void GPIO_Initialize(){
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
 GPIO_PinAF(GPIOB,10,GPIO_AF_USART3);                   
 GPIO_PinConfigure(GPIOB,10,GPIO_MODE_AF,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_100MHz,GPIO_PULL_UP); //PB10“˝Ω≈≈‰÷√
 GPIO_PinAF(GPIOB,11,GPIO_AF_USART3);                  
 GPIO_PinConfigure(GPIOB,11,GPIO_MODE_AF,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_100MHz,GPIO_PULL_UP); //PB11“˝Ω≈≈‰÷√
}

int USART3_transmitter_empty(void){
 if (USART_GetFlagStatus(USART3,USART_FLAG_TXE) == SET) 
    return 1; 
 else return 0;
}


void USART3_puts(char *s){ 
 while(*s) {
      USART_SendData(USART3, *s++); 
   while ( !USART3_transmitter_empty() ) { }     
 }

}

int USART3_receiver_ready(void){
 if (USART_GetFlagStatus(USART3,USART_FLAG_RXNE) == SET) 
    return 1; 
 else return 0;
 
}


int main(){ 
 char d;
 char c; 
 USART3_Initialize(); 
 GPIO_Initialize(); 
 USART_SendData(USART3,'A');        
 while(!USART3_receiver_ready())   {}          
 c = USART_ReceiveData(USART3)-32; 
 USART_SendData(USART3, c);                     

}