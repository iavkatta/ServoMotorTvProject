#include "timers.h"


void InitializeTimer()
{
/*Timer setup*/
//	/* Enable timer 2, using the Reset and Clock Control register */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//
//	SetupTimer.TIM_Prescaler = 36-1; // Prescale to 400Hz
//	SetupTimer.TIM_CounterMode = TIM_CounterMode_Up;
//	SetupTimer.TIM_Period = 100-1; // x-seconds period
//	SetupTimer.TIM_ClockDivision = TIM_CKD_DIV1;
//	SetupTimer.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(Timers_Motor_Timer, &SetupTimer);

/*Pwm channels init*/
	/*====================================*/
	/***      Setup TM PWM CH1         ***/
	/*====================================*/
//    TIM_OCInitTypeDef outputChannelInit;
//	TIM_OCStructInit(&outputChannelInit);
//    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
//    outputChannelInit.TIM_Pulse = 0;
//    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
//    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
//
//    TIM_OC1Init(Timers_Motor_Timer, &outputChannelInit);
//    TIM_OC1PreloadConfig(Timers_Motor_Timer, TIM_OCPreload_Enable);

	/*====================================*/
	/***       Setup TM PWM CH1        ***/
    /*****************END******************/
	/*====================================*/

	/*====================================*/
	/***      Setup TM PWM CH2         ***/
	/*====================================*/
    //TIM_OCInitTypeDef outputChannelInitCH3;
//    TIM_OCStructInit(&outputChannelInit);
//    outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
//    outputChannelInit.TIM_Pulse = 0;
//    outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
//    outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;
//
//    TIM_OC2Init(Timers_Motor_Timer, &outputChannelInit);
//    TIM_OC2PreloadConfig(Timers_Motor_Timer, TIM_OCPreload_Enable);

	/*====================================*/
	/***      Setup TM PWM CH2         ***/
    /*****************END******************/
	/*====================================*/

/*Enable timer*/
//	TIM_Cmd(Timers_Motor_Timer, ENABLE); // start counting by enabling CEN in CR1 */
}
