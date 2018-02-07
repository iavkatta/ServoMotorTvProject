#include "MotorDriverMX1508.h"

#ifdef MOTDRV_MX1508_DEBUG
/*Motor driver TMx defines*/
#define 	MotorDriver_MotorFW_Priph_TM		RCC_APB1Periph_TIM3
#define 	MotorDriver_Motors_TMx				TIM3
#define		MotorDriver_Motors_TMx_Prescaler	(uint16_t)(36-1)
#define		MotorDriver_Motors_TMx_Period		(uint16_t)(10000-1)
#define		MotorDriver_MotorLF1_CCxCHx			TIM_Channel_1
#define		MotorDriver_MotorLF2_CCxCHx			TIM_Channel_2
#define		MotorDriver_MotorRH1_CCxCHx			TIM_Channel_3
#define		MotorDriver_MotorRH2_CCxCHx			TIM_Channel_4

/*Motor driver left and right motor PWM channels*/
#define		MotorDriver_MotorLF1_PWM_CH1			GPIO_Pin_6
#define		MotorDriver_MotorLF2_PWM_CH2			GPIO_Pin_7
#define		MotorDriver_MotorLF_PWM_PORT			GPIOA
#define		MotorDriver_MotorLF_APBPeriph			RCC_APB2Periph_GPIOA

#define		MotorDriver_MotorRH1_PWM_CH1			GPIO_Pin_0
#define		MotorDriver_MotorRH2_PWM_CH2			GPIO_Pin_1
#define		MotorDriver_MotorRH_PWM_PORT			GPIOB
#define		MotorDriver_MotorRH_APBPeriph			RCC_APB2Periph_GPIOB

typedef void (*tTimerPwmChannelInit)	(TIM_TypeDef*, TIM_OCInitTypeDef*);
typedef void (*tMotTimrPreloadCfg)		(TIM_TypeDef*, uint16_t);
typedef void (*tCCPfunction)			(TIM_TypeDef*, uint16_t);

typedef struct
{
	tTimerPwmChannelInit		TimerPwmChannelInit;
	tMotTimrPreloadCfg			MotTimrPreloadCfg;
	tCCPfunction				CCPFunction;
}tPWMFuncs;

typedef struct
{
	uint16_t					timerPeriph;
	uint16_t 					prescalerValue;
	uint16_t					timerPeriod;
}tMotorTimer;

typedef	struct
{
	uint16_t					PWMvalue1;
	uint16_t					PWMvalue2;
	uint16_t					CCxCHx1;
	uint16_t					CCxCHx2;
	tPWMFuncs					PWMChan1Funcs;
	tPWMFuncs					PWMChan2Funcs;
}tMotorPWMvalue;

static const tMotorTimer motorsTimerPwm =
{
	MotorDriver_MotorFW_Priph_TM,
	MotorDriver_Motors_TMx_Prescaler,
	MotorDriver_Motors_TMx_Period,
};

static const tMotorPWMvalue	Motor_LF =
{
	2999,							/*PWMvalue1*/
	0,								/*PWMvalue2*/
	MotorDriver_MotorLF1_CCxCHx,
	MotorDriver_MotorLF2_CCxCHx,
	{
		&TIM_OC1Init,    &TIM_OC1PreloadConfig,		&TIM_SetCompare1	/*PWMChan1Funcs*/
	},
	{
		&TIM_OC2Init,    &TIM_OC2PreloadConfig,		&TIM_SetCompare2	/*PWMChan2Funcs*/
	}
};

static const tMotorPWMvalue	Motor_RH =
{
	2999,							/*PWMvalue1*/
	0,								/*PWMvalue2*/
	MotorDriver_MotorRH1_CCxCHx,
	MotorDriver_MotorRH2_CCxCHx,
	{
		&TIM_OC3Init,    &TIM_OC3PreloadConfig,		&TIM_SetCompare3	/*PWMChan3Funcs*/
	},
	{
		&TIM_OC4Init,    &TIM_OC4PreloadConfig,		&TIM_SetCompare4	/*PWMChan4Funcs*/
	}
};

TIM_TimeBaseInitTypeDef 	MotorDriver_MotorsTMX;
TIM_OCInitTypeDef			MotorDriver_Motors_PWM;
GPIO_InitTypeDef 			gpioStructure;

void MotorDriverMX1508_Init()
{
	/*====================================*/
	/*** Setup GPIO LF motor PWM GPIOs  ***/
	/*====================================*/
	RCC_APB2PeriphClockCmd(MotorDriver_MotorLF_APBPeriph, ENABLE);

	GPIO_StructInit(&gpioStructure);
	gpioStructure.GPIO_Pin = MotorDriver_MotorLF1_PWM_CH1 | MotorDriver_MotorLF2_PWM_CH2;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MotorDriver_MotorLF_PWM_PORT, &gpioStructure);

	/*====================================*/
	/*** Setup GPIO RH motor PWM GPIOs  ***/
	/*====================================*/
	RCC_APB2PeriphClockCmd(MotorDriver_MotorRH_APBPeriph, ENABLE);

	GPIO_StructInit(&gpioStructure);
	gpioStructure.GPIO_Pin = MotorDriver_MotorRH1_PWM_CH1 | MotorDriver_MotorRH2_PWM_CH2;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MotorDriver_MotorRH_PWM_PORT, &gpioStructure);

	/*====================================*/
	/***      	Setup TMx PWM           ***/
	/*====================================*/
	RCC_APB1PeriphClockCmd(MotorDriver_MotorFW_Priph_TM, ENABLE);

	TIM_TimeBaseStructInit(&MotorDriver_MotorsTMX);
	MotorDriver_MotorsTMX.TIM_Prescaler = motorsTimerPwm.prescalerValue; // Prescale to 400Hz
	MotorDriver_MotorsTMX.TIM_CounterMode = TIM_CounterMode_Up;
	MotorDriver_MotorsTMX.TIM_Period = motorsTimerPwm.timerPeriod; // x-seconds period
	MotorDriver_MotorsTMX.TIM_ClockDivision = TIM_CKD_DIV1;
	MotorDriver_MotorsTMX.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(MotorDriver_Motors_TMx, &MotorDriver_MotorsTMX);

	/*====================================*/
	/*** Setup  LF Motor TMx PWM CHs FW ***/
	/*====================================*/
/*CH1 PWM*/
    TIM_OCStructInit(&MotorDriver_Motors_PWM);
    MotorDriver_Motors_PWM.TIM_OCMode = TIM_OCMode_PWM1;
    MotorDriver_Motors_PWM.TIM_Pulse = Motor_LF.PWMvalue1;
    MotorDriver_Motors_PWM.TIM_OutputState = TIM_OutputState_Enable;
    MotorDriver_Motors_PWM.TIM_OCPolarity = TIM_OCPolarity_High;

    Motor_LF.PWMChan1Funcs.TimerPwmChannelInit(MotorDriver_Motors_TMx, &MotorDriver_Motors_PWM);
    Motor_LF.PWMChan1Funcs.MotTimrPreloadCfg(MotorDriver_Motors_TMx, TIM_OCPreload_Enable);
/*CH2 PWM*/
    MotorDriver_Motors_PWM.TIM_Pulse = Motor_LF.PWMvalue2;
    Motor_LF.PWMChan2Funcs.TimerPwmChannelInit(MotorDriver_Motors_TMx, &MotorDriver_Motors_PWM);
    Motor_LF.PWMChan2Funcs.MotTimrPreloadCfg(MotorDriver_Motors_TMx, TIM_OCPreload_Enable);
	/*====================================*/
	/*** Setup  RH Motor TMx PWM CHs FW ***/
	/*====================================*/
/*CH3 PWM*/
    MotorDriver_Motors_PWM.TIM_Pulse = Motor_RH.PWMvalue1;
    Motor_RH.PWMChan1Funcs.TimerPwmChannelInit(MotorDriver_Motors_TMx, &MotorDriver_Motors_PWM);
    Motor_RH.PWMChan1Funcs.MotTimrPreloadCfg(MotorDriver_Motors_TMx, TIM_OCPreload_Enable);
/*CH4 PWM*/
    MotorDriver_Motors_PWM.TIM_Pulse = Motor_RH.PWMvalue2;
    Motor_RH.PWMChan2Funcs.TimerPwmChannelInit(MotorDriver_Motors_TMx, &MotorDriver_Motors_PWM);
    Motor_RH.PWMChan2Funcs.MotTimrPreloadCfg(MotorDriver_Motors_TMx, TIM_OCPreload_Enable);

	/*====================================*/
	/*** 	 Start  PWM Motor TMx   	***/
	/*====================================*/
    TIM_Cmd(MotorDriver_Motors_TMx, ENABLE); // start counting by enabling CEN in CR1 */
}

void MotorDriverMX1508_MotorOutputTest()
{
	int i;
	uint16_t	PWM1 = 2999;
	uint16_t	PWM2 = 2999;

	for (i = 0; i < 15000000; i++);
	Motor_LF.PWMChan1Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
	Motor_LF.PWMChan2Funcs.CCPFunction(MotorDriver_Motors_TMx, PWM1);
	Motor_RH.PWMChan1Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
	Motor_RH.PWMChan2Funcs.CCPFunction(MotorDriver_Motors_TMx, PWM2);

	for (i = 0; i < 15000000; i++);
	Motor_LF.PWMChan1Funcs.CCPFunction(MotorDriver_Motors_TMx, PWM1);
	Motor_LF.PWMChan2Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
	Motor_RH.PWMChan1Funcs.CCPFunction(MotorDriver_Motors_TMx, PWM2);
	Motor_RH.PWMChan2Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);

	for (i = 0; i < 15000000; i++);
	Motor_LF.PWMChan1Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
	Motor_LF.PWMChan2Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
	Motor_RH.PWMChan1Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
	Motor_RH.PWMChan2Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
}

MotorDriver_tResult	MotorDriverMX1508_Move(uint16_t PWMvalue, MotorDriver_tDirection direction)
{
	MotorDriver_tResult	result = E_OK;

	switch(direction)
	{
		case(FW):
			{
				Motor_LF.PWMChan1Funcs.CCPFunction(MotorDriver_Motors_TMx, PWMvalue);
				Motor_LF.PWMChan2Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
				Motor_RH.PWMChan1Funcs.CCPFunction(MotorDriver_Motors_TMx, PWMvalue);
				Motor_RH.PWMChan2Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
				break;
			}
		case(BW):
			{
				Motor_LF.PWMChan1Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
				Motor_LF.PWMChan2Funcs.CCPFunction(MotorDriver_Motors_TMx, PWMvalue);
				Motor_RH.PWMChan1Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
				Motor_RH.PWMChan2Funcs.CCPFunction(MotorDriver_Motors_TMx, PWMvalue);
				break;
			}
		default:
			result = E_NOK;
			break;
	}

	return result;
}

uint8_t MotorDriverMX1508_Stop()
{
	Motor_LF.PWMChan1Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
	Motor_LF.PWMChan2Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
	Motor_RH.PWMChan1Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);
	Motor_RH.PWMChan2Funcs.CCPFunction(MotorDriver_Motors_TMx, 0);

	return E_OK;
}
#endif
/*EOF*/
