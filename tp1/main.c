
#include "stm32f4xx.h"

/*RCC_AHB1ENR pour activer l抙orloge du port GPIOH. 
o GPIOx_MODER pour configurer le port PH.3 en General purpose output mode. 
o GPIOx_OTYPER pour configurer le port PH.3 en Output push-pull. 
o GPIOx_OSPEEDR pour configurer le port PH.3 en 50 MHz Fast speed. 
o GPIOx_PUPDR pour configurer le port PH.3 en Pull-down. 
	|:ou, &:et, ~:取反
*/
void LD4_Initialize(void){
	//set bit 3, P146 du referencce manuel,activer l'horloge du port GPIOD,
	//mettre GPIOD_EN a 1 dans le registre RCC_AHB1ENR
	RCC->AHB1ENR|=(1<<3);
	
	//01: General purpose output mode
	//配置GPIOD的第12个引脚为output
	//LED4被连接到GPIOD的12引脚(in STM32F401C Discovery Board p18)
	//注意set前先清空
	GPIOD->MODER&=~(0x3<<2*12);//clear bits 24 et 25
	GPIOD->MODER|=(0x1<<2*12);//set bits 24
	
	//0: Output push-pull (reset state)
	//https://zhuanlan.zhihu.com/p/41942876
	GPIOD->OTYPER&=~(1<<12);//CLEAR->push-pull sur PD12 (envoie de 0 et i possible)
	
	//10: 50 MHz Fast speed
	GPIOD->OSPEEDR&=~(0x3<<2*12);
	GPIOD->OSPEEDR|=(0x2<<2*12);
	
	//10: Pull-down
	GPIOD->PUPDR&=~(0x3<<2*12);
	GPIOD->PUPDR|= (0x2<<2*12);
}

void LD4_On(){
	//GPIO port bit set/reset low register(in stm32f4xx.h)
	GPIOD->BSRRL|=(1<<12);
}

void LD4_Off(){
	GPIOD->BSRRH|=(1<<12);
}

int main(){
	unsigned int i;
	LD4_Initialize();
	
	while(1){
		LD4_On();
		for(i=0; i<1000000; i++);
		LD4_Off();
		for(i=0; i<1000000; i++);
	}
}