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
    //ʹ�� TIM3 ��ʱ���ĸ����ж� 
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
    }

void NVIC_Initialize(void){
    NVIC_InitTypeDef Struct;
    //�����ж�ͨ���� 
    Struct.NVIC_IRQChannel=EXTI0_IRQn;
    //�����ж�ͨ�� 
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
  //SYSCFG �����ṩ�˼Ĵ����ͺ��������ú͹����ⲿ�ж��ߺͶ�Ӧ�� GPIO ����ӳ�䡣����ͨ�� SYSCFG ���轫�ⲿ�ж������ӵ���ͬ�� GPIO ������ 
  
  //�ⲿ�ж�����ͨ��EXTI0�����ӵ�΢��������ĳ�� GPIO �����ϣ������ڼ��������ϵ��ⲿ�¼������� 
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
  //���� EXTI0 ���ж���· 
  EXTI_ConfigureLine(0,EXTI_MODE_INTERRUPT,EXTI_TRIGGER_RISING);
}

void EXTI0_IRQHandler(void){
  //����ⲿ�ж����ϵĹ����־λ��������˵������Ĳ��� 0 ��ʾҪ������ж��߱�š� 
  //���ⲿ�ж����ϵ��ж��¼�����ʱ����Ӧ�Ĺ����־λ�ᱻ����Ϊ��ʾ�жϹ���״̬�� 
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
