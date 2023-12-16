#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "core_cm4.h"
#include "led.h"

uint8_t LED=0;
uint8_t n=0;

void TIM3_Initialize(){
	//定义结构体变量用来存储TIM3定时器的初始化参数
	TIM_TimeBaseInitTypeDef StructInit;
	//调用RCC库中函数，使能GPIOA总线时钟，以便访问TIM3定时器所在区域
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	StructInit.TIM_Prescaler=9999;
	StructInit.TIM_CounterMode=TIM_CounterMode_Up;
	StructInit.TIM_Period=69;
	//TIM3时钟分频系数设置为默认分频系数
	StructInit.TIM_ClockDivision=TIM_CKD_DIV1;
	//初始化时钟分频系数，计数模式，预分频系数等参数
	TIM_TimeBaseInit(TIM3,&StructInit);
	//定时器中断功能，在计数达到预设值时产生中断(IT:interrupt)
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//启动TIM3定时器
	TIM_Cmd(TIM3,ENABLE);
}

//Enable a specific interrupt to allow the corresponding interrupt request to trigger
void NVIC_Initialize(void){
	NVIC_EnableIRQ(TIM3_IRQn);//Interrupt Request Number
}

void TIM3_IRQHandler(void){
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)){
		if(LED==0){
			LED=1;
			for(n=0;n<4;n++)LED_On(n);
		}
		else{
			LED=0;
			for(n=0;n<4;n++)LED_Off(n);
		}
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	}
}

int main(){
	TIM3_Initialize();
	NVIC_Initialize();
	LED_Initialize();
	while(1);
}
