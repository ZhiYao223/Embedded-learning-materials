#include "stm32f1xx.h"
#include "key.h"
#include "led.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"


uint16_t rgb_led_time=20; //600ms后每次增加300ms


int main(void)
{		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化
 	TIM3_Int_Init(1000,72);	 //不分频。PWM频率=72000000/72=1000Khz 1us*1000=1ms
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


