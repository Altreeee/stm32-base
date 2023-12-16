#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "GPIO_STM32f4xx.h"

//Transmission d'un caractere 
//串口通信模块
void USART3_Initialize(void){
	USART_InitTypeDef Struct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	Struct.USART_BaudRate=9600;
	Struct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	//Enable both receive and send functions.
	Struct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	//不使用奇偶校验
	Struct.USART_Parity=USART_Parity_No;
	Struct.USART_StopBits=USART_StopBits_1;
	//数据长度8位
	Struct.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART1,&Struct);
	
	USART_Cmd(USART3,ENABLE);
}
	
	
/*
TXD->RXD
RXD(dongle USB)->TXD(Discovery)
TXD(USB)->PB11(RX carte)
RXD(USB)->PB10(TX carte)
GND->GND
*/
void GPIO_Initialize(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//active AHBI pour GPIOB

	GPIO_PinAF(GPIOB,11,GPIO_AF_USART3);//redirection USART3 ver PB11
	GPIO_PinAF(GPIOB,10,GPIO_AF_USART3);

	GPIO_PinConfigure(GPIOB,11,GPIO_MODE_AF,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_100MHz,GPIO_PULL_UP);
	GPIO_PinConfigure(GPIOB,10,GPIO_MODE_AF,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_100MHz,GPIO_PULL_UP);
	
}

/*发送缓冲器 set为空*/
int USART3_transmitter_empty(void){
	if(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==SET) return 1;
	else return 0;
}
	
	
void USART3_puts(char *s){
	while(*s){
		USART_SendData(USART3,*s++);
		while(USART3_transmitter_empty()==0){}
	}
}
	
int main(){
	uint32_t i;
	USART3_Initialize();
	GPIO_Initialize();
	while(1){
		USART_SendData (USART3,'A');
		for(i=0;i<1000000;i++);
	}
}
