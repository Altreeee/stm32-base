/*替换了system_stm32f4xx.c文件*/
#include "led.h"
#include "stm32f4xx.h"//rcc et tim

uint8_t LED=0;
uint8_t n;

/*目录P13*/
void TIM3_Init(void){
	RCC->APB1ENR|=(1<<1);//active horloge APB1 for tim3
	/*RCC模块。打开APB1总线上的TIM3时钟. APB1ENR是RCC模块中的一个使能寄存器*/
	TIM3->CR1=0;//将TIM3定时器的控制寄存器CR1清零
	
	TIM3->PSC	=9999;//DIV=10000(9999+1)=>84MHZ/10000=8400HZ			
									//PSC预分频器
	TIM3->ARR =4199;//PERIODE=4199=>4200 coup d'horloge a 8500HZ=>0,5s
									//ARR自动重载寄存器，当计时器计数到4199时触发计时器自动重置
	TIM3->DIER |=(1<<0);//UIF =1(update Interrupt enabled)
											//使能更新中断，在计时器重置时产生中断请求
	TIM3->CR1 |=(1<<0);//start tim3
	
}

/*实现中断向量表的初始化，设置ISER[0]的第29位，
将TIM中断向量打开，以便在计数器重置时产生中断请求*/
void NVIC_Initialize(void){
	NVIC->ISER[0]|=(1<<29);
}

/*TIM3的中断处理函数*/
void TIM3_IRQHandler(void){
	//首先判断中断请求是否是由TIM3更新触发的（检查TIM3->SR寄存器第0位）
	if(TIM3->SR &(1<<0)){
		if(LED==0){
			LED=1;
			for(n=0;n<4;n++)LED_On(n);
		}
		else{
			LED=0;
			for(n=0;n<4;n++)LED_Off(n);
		}
		//清除TIM3的中断标志
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
