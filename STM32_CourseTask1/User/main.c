#include "stm32f1xx.h"
#include "key.h"
#include "led.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"


uint16_t rgb_led_time=20; //600ms��ÿ������300ms


int main(void)
{		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
 	TIM3_Int_Init(1000,72);	 //����Ƶ��PWMƵ��=72000000/72=1000Khz 1us*1000=1ms
	KEY_Init();
  while(1)
	{
		if(KEY_Scan(GPIOA,GPIO_Pin_0) == KEY_ON  )
		{
			rgb_led_time -= 10;
			if(rgb_led_time<=10)
				rgb_led_time = 10;
		} 
		if(KEY_Scan(GPIOC,GPIO_Pin_13) == KEY_ON  )
		{
			rgb_led_time += 10;
			if(rgb_led_time >= 500)
				rgb_led_time = 500;
		}	
	}	 
}


