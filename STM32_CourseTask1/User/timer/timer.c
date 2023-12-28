#include "timer.h"
#include "led.h"
#include "usart.h"
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	/* PWM模式配置 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//配置为PWM模式1
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能输出
  	TIM_OCInitStructure.TIM_Pulse = 0;				 						  			//设置初始PWM脉冲宽度为0	
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //当定时器计数值小于CCR_Val时为低电平
	
	//RGB通道
  	TIM_OC2Init ( TIM3, &TIM_OCInitStructure );	
  	TIM_OC2PreloadConfig ( TIM3, TIM_OCPreload_Enable );						//使能预装载	

 	  TIM_OC3Init ( TIM3, &TIM_OCInitStructure );	
  	TIM_OC3PreloadConfig ( TIM3, TIM_OCPreload_Enable );						//使能预装载	

  	TIM_OC4Init ( TIM3, &TIM_OCInitStructure );	
  	TIM_OC4PreloadConfig ( TIM3, TIM_OCPreload_Enable );						//使能预装载	
	
	  TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器			 
}

const uint16_t R[] = {
	255, 226, 198, 170, 141, 113,  85,  56,  28,   0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,  28,  56,  85, 113, 141, 170, 198, 226, 255
};
const uint16_t G[] = {
	0,  28,  56,  85, 113, 141, 170, 198, 226, 255,
	255, 226, 198, 170, 141, 113,  85,  56,  28,   0,
	0, 0, 0, 0, 0, 0, 0, 0, 0,0
};
const uint16_t B[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0,  28,  56,  85, 113, 141, 170, 198, 226, 255,
	255, 226, 198, 170, 141, 113,  85,  56,  28, 0
};

extern uint16_t rgb_led_time;
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	static uint16_t pwm_index = 0;		//用于PWM查表
	static uint16_t amplitude_cnt = 0;	//持续时间
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		amplitude_cnt++;
				
		// 每个颜色持续的时间
		if(amplitude_cnt>rgb_led_time)		 						
		{		
			pwm_index++; //标志PWM表指向下一个元素
			//若PWM表已到达结尾，重新指向表头
			if( pwm_index >  (30-1))			
			{
				pwm_index=0;					
			}
				amplitude_cnt=0; //重置计数标志
		}
		else
		{	
			TIM3->CCR2 = R[pwm_index]; // PWM
			TIM3->CCR3 = G[pwm_index]; // PWM
			TIM3->CCR4 = B[pwm_index]; // PWM
		}	
	}
}
