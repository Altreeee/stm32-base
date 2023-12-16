/*�滻��system_stm32f4xx.c�ļ�*/
#include "led.h"
#include "stm32f4xx.h"//rcc et tim

uint8_t LED=0;
uint8_t n;

/*Ŀ¼P13*/
void TIM3_Init(void){
	RCC->APB1ENR|=(1<<1);//active horloge APB1 for tim3
	/*RCCģ�顣��APB1�����ϵ�TIM3ʱ��. APB1ENR��RCCģ���е�һ��ʹ�ܼĴ���*/
	TIM3->CR1=0;//��TIM3��ʱ���Ŀ��ƼĴ���CR1����
	
	TIM3->PSC	=9999;//DIV=10000(9999+1)=>84MHZ/10000=8400HZ			
									//PSCԤ��Ƶ��
	TIM3->ARR =4199;//PERIODE=4199=>4200 coup d'horloge a 8500HZ=>0,5s
									//ARR�Զ����ؼĴ���������ʱ��������4199ʱ������ʱ���Զ�����
	TIM3->DIER |=(1<<0);//UIF =1(update Interrupt enabled)
											//ʹ�ܸ����жϣ��ڼ�ʱ������ʱ�����ж�����
	TIM3->CR1 |=(1<<0);//start tim3
	
}

/*ʵ���ж�������ĳ�ʼ��������ISER[0]�ĵ�29λ��
��TIM�ж������򿪣��Ա��ڼ���������ʱ�����ж�����*/
void NVIC_Initialize(void){
	NVIC->ISER[0]|=(1<<29);
}

/*TIM3���жϴ�����*/
void TIM3_IRQHandler(void){
	//�����ж��ж������Ƿ�����TIM3���´����ģ����TIM3->SR�Ĵ�����0λ��
	if(TIM3->SR &(1<<0)){
		if(LED==0){
			LED=1;
			for(n=0;n<4;n++)LED_On(n);
		}
		else{
			LED=0;
			for(n=0;n<4;n++)LED_Off(n);
		}
		//���TIM3���жϱ�־
		TIM3->SR &=~(1<<0);
	}
}
	
int main(){
	LED_Initialize();
	TIM3_Init();
	NVIC_Initialize();
	
	while(1){
		
		
	}
}
