#include "timer.h"
#include "led.h"
#include "usart.h"
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	/* PWMģʽ���� */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//����ΪPWMģʽ1
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ʹ�����
  	TIM_OCInitStructure.TIM_Pulse = 0;				 						  			//���ó�ʼPWM������Ϊ0	
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //����ʱ������ֵС��CCR_ValʱΪ�͵�ƽ
	
	//RGBͨ��
  	TIM_OC2Init ( TIM3, &TIM_OCInitStructure );	
  	TIM_OC2PreloadConfig ( TIM3, TIM_OCPreload_Enable );						//ʹ��Ԥװ��	

 	  TIM_OC3Init ( TIM3, &TIM_OCInitStructure );	
  	TIM_OC3PreloadConfig ( TIM3, TIM_OCPreload_Enable );						//ʹ��Ԥװ��	

  	TIM_OC4Init ( TIM3, &TIM_OCInitStructure );	
  	TIM_OC4PreloadConfig ( TIM3, TIM_OCPreload_Enable );						//ʹ��Ԥװ��	
	
	  TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���			 
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
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	static uint16_t pwm_index = 0;		//����PWM���
	static uint16_t amplitude_cnt = 0;	//����ʱ��
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		amplitude_cnt++;
				
		// ÿ����ɫ������ʱ��
		if(amplitude_cnt>rgb_led_time)		 						
		{		
			pwm_index++; //��־PWM��ָ����һ��Ԫ��
			//��PWM���ѵ����β������ָ���ͷ
			if( pwm_index >  (30-1))			
			{
				pwm_index=0;					
			}
				amplitude_cnt=0; //���ü�����־
		}
		else
		{	
			TIM3->CCR2 = R[pwm_index]; // PWM
			TIM3->CCR3 = G[pwm_index]; // PWM
			TIM3->CCR4 = B[pwm_index]; // PWM
		}	
	}
}
