#include "MotorControl.h"
#include "stm32f10x_gpio.h"

#define 	MotorControl_MotorFW_TimerN					TIM3	/*timer used to drive the motors PWM*/
#define		MotorControl_MotorFW_TimerN_Periph			RCC_APB1Periph_TIM3
#define 	MotorControl_MotorFW_TimerN_Prescaler		(uint16_t)(36-1)	//@20kHz
#define		MotorControl_MotorFW_TimerN_Period			(uint16_t)(100-1)	//@20kHz

#define		MotorControl_MotorFW_Port					GPIOA
#define     MotorControl_MotorFW_Port_Periph			RCC_APB2Periph_GPIOA
#define		MotorControl_MotrorFW_PWM_CH1				GPIO_Pin_6
#define		MotorControl_MotrorFW_PWM_CH2				GPIO_Pin_7

#define 	MotorControl_MotorBW_TimerN					TIM3	/*timer used to drive the motors PWM*/
#define		MotorControl_MotorBW_TimerN_Periph			RCC_APB1Periph_TIM3
#define 	MotorControl_MotorBW_TimerN_Prescaler		(uint16_t)(36-1)	//@20kHz
#define		MotorControl_MotorBW_TimerN_Period			(uint16_t)(100-1)	//@20kHz


#define	Mout11			EXTI_Line12
#define	Mout12			EXTI_Line13
#define	Mout21			EXTI_Line14
#define	Mout22			EXTI_Line15

enum
{
	motorFw = 0,
	motorBw,
	NmbrOfMotors
}MotorDirections;

typedef void (*tTimerPwmChannelInit)(TIM_TypeDef*, TIM_OCInitTypeDef*);
typedef void (*tMotTimrPreloadCfg)(TIM_TypeDef*, uint16_t);


typedef struct
{
	tTimerPwmChannelInit		TimerPwmChannelInit;
	tMotTimrPreloadCfg			MotTimrPreloadCfg;
}tPWMInitFuncs;

typedef struct
{
	uint32_t					timerUsed;
	uint32_t					timerPerif;
	TIM_TimeBaseInitTypeDef		SetupTimer;
	TIM_OCInitTypeDef			outputChannelInit;
	tPWMInitFuncs				InitFuncs;
}tTimer;

const tTimer MotorTimerFW =
{
	/*timerUsed							TimerPeriph									TIM_TimeBaseInitTypeDef					TIM_OCInitTypeDef				tPWMInitFuncs*/
	//MotorControl_MotorFW_TimerN, 		MotorControl_MotorFW_TimerN_Periph,			MotorFW_TIM_TimeBaseInitTypeDef,		MotorFW_TIM_OCInitTypeDef,		PWMInitFuncs[motorFw]
			MotorControl_MotorFW_TimerN			, /*timerUsed*/
			MotorControl_MotorFW_TimerN_Periph	,
		{
				/*TIM_Prescaler								TIM_CounterMode				TIM_Period								TIM_ClockDivision		TIM_RepetitionCounter*/
				MotorControl_MotorFW_TimerN_Prescaler,		TIM_CounterMode_Up,			MotorControl_MotorFW_TimerN_Period,		TIM_CKD_DIV1,			0
		},
		{
				/*TIM_OCMode		TIM_OutputState			TIM_OutputNState			TIM_Pulse	TIM_OCPolarity			TIM_OCNPolarity		 TIM_OCIdleState		TIM_OCNIdleState*/
				TIM_OCMode_PWM1,	TIM_OutputState_Enable,	TIM_OutputNState_Disable,	50,		TIM_OCPolarity_High,	TIM_OCPolarity_High, TIM_OCIdleState_Reset,	TIM_OCNIdleState_Reset
		},
		{
				&TIM_OC1Init			,			&TIM_OC1PreloadConfig
		}

};

const tTimer MotorTimerBW =
{
	/*timerUsed							TimerPeriph									TIM_TimeBaseInitTypeDef					TIM_OCInitTypeDef				tPWMInitFuncs*/
	//MotorControl_MotorFW_TimerN, 		MotorControl_MotorFW_TimerN_Periph,			MotorFW_TIM_TimeBaseInitTypeDef,		MotorFW_TIM_OCInitTypeDef,		PWMInitFuncs[motorFw]
			MotorControl_MotorBW_TimerN			, /*timerUsed*/
			MotorControl_MotorBW_TimerN_Periph	,
		{
				/*TIM_Prescaler								TIM_CounterMode				TIM_Period								TIM_ClockDivision		TIM_RepetitionCounter*/
				MotorControl_MotorBW_TimerN_Prescaler,		TIM_CounterMode_Up,			MotorControl_MotorBW_TimerN_Period,		TIM_CKD_DIV1,			0
		},
		{
				/*TIM_OCMode		TIM_OutputState			TIM_OutputNState			TIM_Pulse	TIM_OCPolarity			TIM_OCNPolarity		 TIM_OCIdleState		TIM_OCNIdleState*/
				TIM_OCMode_PWM1,	TIM_OutputState_Enable,	TIM_OutputNState_Disable,	10,		TIM_OCPolarity_High,	TIM_OCPolarity_High, TIM_OCIdleState_Reset,	TIM_OCNIdleState_Reset
		},
		{
				&TIM_OC2Init			,			&TIM_OC2PreloadConfig
		}

};

typedef struct
{
uint32_t	MotorPort;
uint32_t	MotorPort_prtiph;
GPIO_InitTypeDef	gpioStructure;
}tMotorPort;

tMotorPort	MotorFWPort =
{
		MotorControl_MotorFW_Port,
		MotorControl_MotorFW_Port_Periph,
		{
				MotorControl_MotrorFW_PWM_CH1,		GPIO_Mode_AF_PP,		GPIO_Speed_50MHz
		}
};

tMotorPort	MotorBWPort =
{
		MotorControl_MotorFW_Port,
		MotorControl_MotorFW_Port_Periph,
		{
				MotorControl_MotrorFW_PWM_CH2,		GPIO_Mode_AF_PP,		GPIO_Speed_50MHz
		}
};

void MotorControl_Init()
{
	RCC_APB2PeriphClockCmd(MotorFWPort.MotorPort_prtiph,ENABLE);
	RCC_APB2PeriphClockCmd(MotorBWPort.MotorPort_prtiph,ENABLE);
	GPIO_Init(MotorFWPort.MotorPort, &MotorFWPort.gpioStructure);
	GPIO_Init(MotorBWPort.MotorPort, &MotorBWPort.gpioStructure);

	RCC_APB1PeriphClockCmd(MotorTimerFW.timerPerif, ENABLE);
	TIM_TimeBaseInit(MotorTimerFW.timerUsed, &MotorTimerFW.SetupTimer);

	MotorTimerFW.InitFuncs.TimerPwmChannelInit(MotorTimerFW.timerUsed, &MotorTimerFW.outputChannelInit);
	MotorTimerFW.InitFuncs.MotTimrPreloadCfg(MotorTimerFW.timerUsed, TIM_OCPreload_Enable);

	MotorTimerBW.InitFuncs.TimerPwmChannelInit(MotorTimerBW.timerUsed, &MotorTimerBW.outputChannelInit);
	MotorTimerBW.InitFuncs.MotTimrPreloadCfg(MotorTimerBW.timerUsed, TIM_OCPreload_Enable);

	TIM_Cmd(MotorTimerFW.timerUsed, ENABLE);

	GPIO_SetBits(GPIOB, Mout11);
	GPIO_ResetBits(GPIOB, Mout12);

	TIM_SetCompare1(TIM3, 30);
//	TIM_CCxCmd(Timers_Motor_Timer, TIM_Channel_1, TIM_CCx_Enable);

	GPIO_SetBits(GPIOB, Mout21);
	GPIO_ResetBits(GPIOB, Mout22);

	TIM_SetCompare2(TIM3, 30);
//	TIM_CCxCmd(Timers_Motor_Timer, TIM_Channel_2, TIM_CCx_Enable);
}
