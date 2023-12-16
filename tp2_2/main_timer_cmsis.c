#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "core_cm4.h"
#include "led.h"

uint8_t LED=0;
uint8_t n=0;

void TIM3_Initialize(){
	//����ṹ����������洢TIM3��ʱ���ĳ�ʼ������
	TIM_TimeBaseInitTypeDef StructInit;
	//����RCC���к�����ʹ��GPIOA����ʱ�ӣ��Ա����TIM3��ʱ����������
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	StructInit.TIM_Prescaler=9999;
	StructInit.TIM_CounterMode=TIM_CounterMode_Up;
	StructInit.TIM_Period=69;
	//TIM3ʱ�ӷ�Ƶϵ������ΪĬ�Ϸ�Ƶϵ��
	StructInit.TIM_ClockDivision=TIM_CKD_DIV1;
	//��ʼ��ʱ�ӷ�Ƶϵ��������ģʽ��Ԥ��Ƶϵ���Ȳ���
	TIM_TimeBaseInit(TIM3,&StructInit);
	//��ʱ���жϹ��ܣ��ڼ����ﵽԤ��ֵʱ�����ж�(IT:interrupt)
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//����TIM3��ʱ��
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
