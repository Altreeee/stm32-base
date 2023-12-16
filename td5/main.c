#include "led.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "core_cm4.h"
#include "misc.h"
#include "GPIO_STM32F4xx.h"
#include "EXTI_STM32F4xx.h"

#include "stm32f4xx_syscfg.h"

int8_t cpt;
uint8_t turn;
uint32_t i;

void LED_shift_clockewise(){
	if(cpt==4 || cpt ==-1)cpt=0;
	LED_On(cpt);
	if(cpt==0)LED_Off(3);
	else LED_Off(cpt-1);
	cpt++;
}


void LED_shiftcounter(){
	if(cpt==-1 || cpt==4)cpt=3;
	LED_On(cpt);
	if(cpt==3)LED_Off(0);
	else LED_Off(cpt+1);
	cpt--;
}

void TIM3_Initialize(){
	TIM_TimeBaseInitTypeDef Struct;
	//control the enabling and disabling of peripheral clocks on the APB1 bus.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	Struct.TIM_Prescaler=9999;//
	Struct.TIM_CounterMode=TIM_CounterMode_Up;
	Struct.TIM_Period=4199; //0.25s
	Struct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&Struct);
	//TIM_ITConfig function is used to configure interrupt enablement and disablement of the timer
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	//TIM_Cmd is a function that controls the turn of a timer on or off. 
	//By passing TIM3 parameters and ENABLE, the function starts the TIM3 timer.
	TIM_Cmd(TIM3,ENABLE);
}

void NVIC_Initialize(void){
	NVIC_InitTypeDef Struct;
	//TIM3 interrupts are enabled. NVIC_EnableIRQ function is used to enable the specified interrupt channel
	NVIC_EnableIRQ(TIM3_IRQn);
	//The interrupt channel is set to EXTI0_IRQn. The interrupt channel represents a specific interrupt source, 
	//and here set to EXTI0 represents the external interrupt line 0.
	Struct.NVIC_IRQChannel=EXTI0_IRQn;
	Struct.NVIC_IRQChannelCmd=ENABLE;
	//This line sets the preemption priority of the interrupt channel to 0
	//The lower the number, the higher the priority.
	Struct.NVIC_IRQChannelPreemptionPriority=0;
	//The subpriority of the interrupt channel is set to 0. 
	//Sub-priority is used to distinguish interrupts with the same preemption priority, 
	//with lower values having higher priority.
	Struct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&Struct);
	Struct.NVIC_IRQChannel=TIM3_IRQn;
	Struct.NVIC_IRQChannelCmd=ENABLE;
	Struct.NVIC_IRQChannelPreemptionPriority=0;
	Struct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&Struct);
}


/*
Initialize the external interrupt EXTI0. 
It turns on clock enable for GPIOA and SYSCFG, 
configures GPIOA's pin 0 as input mode, and then configures the pin source and trigger mode of EXTI0 
so that external interrupt line 0 can respond to rising edge trigger events on GPIOA's pin 0.
EXIT¼à²âGPIOÒý½Å±ä»¯
*/
void EXTI0_Initialize(){
	//active l'horloge peripherique RCC AHB1 connecte le bouton B1
	//enabled clock of GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	//configured Pin 0 of GPIOA is input mode, push-pull output type, output speed is 50MHz, no pull-up/pull-down.
	GPIO_PinConfigure(GPIOA,0,GPIO_MODE_INPUT,GPIO_OUTPUT_PUSH_PULL,GPIO_OUTPUT_SPEED_50MHz,GPIO_NO_PULL_UP_DOWN);
	//configures the mapping between the external interrupt line and the corresponding GPIO pin.
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,EXTI_PinSource0);
	//configures the operating mode of the outer interrupt line 0 to break mode and the trigger mode to rising edge triggering.
	EXTI_ConfigureLine(0,EXTI_MODE_INTERRUPT,EXTI_TRIGGER_RISING);
}


/*When an external interrupt EXTI0 occurs, the interrupt service program is executed. 
It first clears the interrupt's suspend flag bit, and then performs a simple flip switch operation based on 
the value of the turn variable*/
void EXTI0_IRQHandler(void){
	//Clear the suspend flag bit that specifies the outer interrupt line, indicating that the interrupt has been processed.
	EXTI_ClearPendingBit(0);
	if(turn==0) turn=1;
	else turn=0;
}
	
void TIM3_IRQHandler(void) {
	//Check that the update flag bit (TIM_FLAG_Update) of the TIM3 timer is set. 
	//If the update flag bit is set, a timer update event has occurred
	if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update)){
		if(turn==0) LED_shift_clockewise();
		else LED_shiftcounter();
	}
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
}

int main(){
	LED_Initialize();
	TIM3_Initialize();
	NVIC_Initialize();
	EXTI0_Initialize();
	while(1){
		
	}
}
