#include "led.h"

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void) 
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	/* GPIO clock enable */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
  	/*IO设置*/
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 
  	/* 配置LED灯用到的引脚 */
	//红
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 ;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//绿
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	//蓝
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
