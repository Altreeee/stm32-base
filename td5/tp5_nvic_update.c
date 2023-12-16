#include "led.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "core_cm4.h"
#include "misc.h"
#include "GPIO_STM32F4xx.h"
#include "EXTI_STM32F4xx.h"

int8_t cpt;
uint8_t turn;
uint32_t i;

void LED_shift_clockwise(void){
    if(cpt==4 || cpt==-1) cpt=0;
    LED_On(cpt);
    if(cpt==0) LED_Off(3);
    else LED_Off(cpt-1);
    cpt++;
    }

void LED_shift_counterclockwise(void){
    if(cpt==-1 || cpt==4) cpt=3;
    LED_On(cpt);
    if(cpt==3) LED_Off(0);
    else LED_Off(cpt+1);
    cpt--;
    }

void TIM3_Initialize (void){
    TIM_TimeBaseInitTypeDef InitStruct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    InitStruct.TIM_Prescaler=9999;
    InitStruct.TIM_Period=4199;
    InitStruct.TIM_CounterMode=TIM_CounterMode_Up;
    InitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &InitStruct);
    //使能 TIM3 定时器的更新中断 
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
    }

void NVIC_Initialize(void){
    NVIC_InitTypeDef Struct;
    //设置中断通道号 
    Struct.NVIC_IRQChannel=EXTI0_IRQn;
    //启用中断通道 
    Struct.NVIC_IRQChannelCmd=ENABLE;
    Struct.NVIC_IRQChannelPreemptionPriority=0;
    Struct.NVIC_IRQChannelSubPriority=0;
    NVIC_Init(&Struct);
    Struct.NVIC_IRQChannel=TIM3_IRQn;
    Struct.NVIC_IRQChannelCmd=ENABLE;
    Struct.NVIC_IRQChannelPreemptionPriority=0;
    Struct.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&Struct);
    }

  
void EXTI0_Initialize(void){
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
  GPIO_PinConfigure(GPIOA,0,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_50MHz,GPIO_NO_PULL_UP_DOWN);
  //SYSCFG 外设提供了寄存器和函数来配置和管理外部中断线和对应的 GPIO 引脚映射。可以通过 SYSCFG 外设将外部中断线连接到不同的 GPIO 引脚上 
  
  //外部中断输入通道EXTI0，连接到微控制器的某个 GPIO 引脚上，并用于检测该引脚上的外部事件触发。 
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
  //配置 EXTI0 的中断线路 
  EXTI_ConfigureLine(0,EXTI_MODE_INTERRUPT,EXTI_TRIGGER_RISING);
}

void EXTI0_IRQHandler(void){
  //清除外部中断线上的挂起标志位，具体来说，这里的参数 0 表示要清除的中断线编号。 
  //当外部中断线上的中断事件发生时，相应的挂起标志位会被设置为表示中断挂起状态。 
  EXTI_ClearPendingBit(0);
  if(turn==0)turn=1;
  else turn=0;
}

void TIM3_IRQHandler(void){
   if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)){
     if(turn==0) LED_shift_clockwise();
     else LED_shift_counterclockwise();
   }
   TIM_ClearFlag(TIM3,TIM_FLAG_Update);

} 
int main(){
	LED_Initialize();
	TIM3_Initialize();
	NVIC_Initialize();
	EXTI0_Initialize();
	while(1){
//	LED_shift_clockwise();
//	for(i=0;i<1000000;i++);
	}
}
