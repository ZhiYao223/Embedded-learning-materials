#include "led.h"

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void) 
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	/* GPIO clock enable */
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
  	/*IO����*/
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 
  	/* ����LED���õ������� */
	//��
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 ;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//��
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	//��
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
