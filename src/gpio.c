/*
 * gpio.c
 *
 *  Created on: 10.11.2017 ã.
 *      Author: dzhen001
 */

#include "gpio.h"


void GPIO_init(void)
{
	EXTI_InitTypeDef 	EXTI_InitStruct;
	NVIC_InitTypeDef 	NVIC_InitStruct;
	GPIO_InitTypeDef 	led;

	/*====================================*/
	/***	Setup on board led		  *****/
	/*====================================*/
	// Enable clock for GPIOC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/*Define user led for debug*/
	GPIO_StructInit(&led);
	led.GPIO_Pin = GPIO_Pin_13;
	led.GPIO_Speed = GPIO_Speed_2MHz;
	led.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &led);

	/*====================================*/
	/***		Setup on board led		***/
	/*****************END******************/
	/*====================================*/

	/*====================================*/
	/***Setup on board B12 B13 B14 B15*****/
	/*====================================*/
	// Enable clock for GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_StructInit(&led);
	led.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	led.GPIO_Speed = GPIO_Speed_2MHz;
	led.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &led);
	/*====================================*/
	/***Setup on board B12 B13 B14 B15*****/
	/*****************END******************/
	/*====================================*/

	/*====================================*/
	/***  Setup PB0,1 input w interrupt ***/
	/*====================================*/
	/* Enable AFIO clock */
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* Enable clock for AFIO */

	GPIO_InitTypeDef	btn1;
	GPIO_StructInit(&btn1);
	btn1.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
	btn1.GPIO_Speed = GPIO_Speed_2MHz;
	btn1.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &btn1);

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	/* Connect EXTI1 Line to PB1 pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1); // | GPIO_PinSource4

	/* Configure EXTI1 line */
	EXTI_StructInit(&EXTI_InitStruct);
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	/* Enable and set EXTI1 Interrupt to the highest priority */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/* Connect EXTI4 Line to PB4 pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0); // | GPIO_PinSource4

	/* Configure EXTIx line */
	EXTI_StructInit(&EXTI_InitStruct);
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	/* Enable and set EXTIx Interrupt to the highest priority */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);
//	/*====================================*/
//	/***  Setup PB1 input w interrupt	***/
//	/*****************END******************/
//	/*====================================*/
//
///*PWM setup*/
//
//	/*====================================*/
//	/***	Setup TMx CH1,2 PWM out		***/
//	/*====================================*/
//
//	/* Enable AFIO clock */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//
//	GPIO_InitTypeDef gpioStructure;
//	gpioStructure.GPIO_Pin = Motor_PWM_CH1 | Motor_PWM_CH2;
//	gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(Motor_PWM_CH_PORT, &gpioStructure);
//
//	/*====================================*/
//	/***	Setup TMx CH1,2 PWM out		***/
//	/*****************END******************/
//	/*====================================*/
}

void Gpio_OutputTest()
{
	int i = 0;
	GPIO_SetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	for (i = 0; i < 1500000; i++);
	GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}

/*EOF*/


