#include "MotorDriver.h"
#include "gpio.h"
#include "timers.h"

/*Motor driver TMx defines*/
#define 	MotorDriver_MotorFW_Priph_TM		RCC_APB1Periph_TIM3
#define 	MotorDriver_Motors_TMx				TIM3
#define		MotorDriver_Motors_TMx_Prescaler	(uint16_t)(36-1)
#define		MotorDriver_Motors_TMx_Period		(uint16_t)(100-1)

/*Motor driver PWM channels*/
#define		MotorDriver_Motor_PWM_CH1			GPIO_Pin_6
#define		MotorDriver_Motor_PWM_CH2			GPIO_Pin_7
#define		MotorDriver_Motors_PWM_PORT			GPIOA

/*Motor driver control outputs*/
#define 	MotorDriver_MotorPort				GPIOB
#define		MotorDriver_Mout11					GPIO_Pin_12
#define		MotorDriver_Mout12					GPIO_Pin_13
#define		MotorDriver_Mout21					GPIO_Pin_14
#define		MotorDriver_Mout22					GPIO_Pin_15


typedef void (*tTimerPwmChannelInit)	(TIM_TypeDef*, TIM_OCInitTypeDef*);
typedef void (*tMotTimrPreloadCfg)		(TIM_TypeDef*, uint16_t);
typedef void (*tCCPfunction)			(TIM_TypeDef*, uint16_t);

enum
{
	FW = 0,
	BW = 1,
	nmbrDirections
};

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
	uint16_t					PWMvalue;
	tPWMFuncs					PWMChanFuncs;
}tMotorPWMvalue;

typedef struct
{

}tDirections;

static const tMotorTimer motorsTimerPwm =
{
	MotorDriver_MotorFW_Priph_TM,
	MotorDriver_Motors_TMx_Prescaler,
	MotorDriver_Motors_TMx_Period,
};

static const tMotorPWMvalue	MotorFW =
{
	0x0000,
	{
		&TIM_OC1Init,    &TIM_OC1PreloadConfig,		&TIM_SetCompare1
	}
};

static const tMotorPWMvalue	MotorBW =
{
	0x0000,
	{
		&TIM_OC2Init,    &TIM_OC2PreloadConfig,		&TIM_SetCompare2
	}
};

TIM_TimeBaseInitTypeDef 	MotorDriver_MotorFW;
TIM_OCInitTypeDef			MotorDriver_Motors_PWM;

void MotorDrv_init()
{
	/*====================================*/
	/***      	Setup GPIO PWM          ***/
	/*====================================*/
	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef gpioStructure;
	gpioStructure.GPIO_Pin = MotorDriver_Motor_PWM_CH1 | MotorDriver_Motor_PWM_CH2;
	gpioStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MotorDriver_Motors_PWM_PORT, &gpioStructure);

	/*====================================*/
	/***      	Setup Motor GPIOs       ***/
	/*====================================*/
	// Enable clock for GPIOB
	GPIO_InitTypeDef	led;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_StructInit(&led);
	led.GPIO_Pin = MotorDriver_Mout11 | MotorDriver_Mout12 | MotorDriver_Mout21 | MotorDriver_Mout22;
	led.GPIO_Speed = GPIO_Speed_50MHz;
	led.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(MotorDriver_MotorPort, &led);

	/*====================================*/
	/***      	Setup TMx PWM           ***/
	/*====================================*/
	RCC_APB1PeriphClockCmd(MotorDriver_MotorFW_Priph_TM, ENABLE);

	TIM_TimeBaseStructInit(&MotorDriver_MotorFW);
	MotorDriver_MotorFW.TIM_Prescaler = motorsTimerPwm.prescalerValue; // Prescale to 400Hz
	MotorDriver_MotorFW.TIM_CounterMode = TIM_CounterMode_Up;
	MotorDriver_MotorFW.TIM_Period = motorsTimerPwm.timerPeriod; // x-seconds period
	MotorDriver_MotorFW.TIM_ClockDivision = TIM_CKD_DIV1;
	MotorDriver_MotorFW.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(MotorDriver_Motors_TMx, &MotorDriver_MotorFW);

	/*====================================*/
	/***      Setup TMx PWM CH1 FW      ***/
	/*====================================*/

    TIM_OCStructInit(&MotorDriver_Motors_PWM);
    MotorDriver_Motors_PWM.TIM_OCMode = TIM_OCMode_PWM1;
    MotorDriver_Motors_PWM.TIM_Pulse = MotorFW.PWMvalue;
    MotorDriver_Motors_PWM.TIM_OutputState = TIM_OutputState_Enable;
    MotorDriver_Motors_PWM.TIM_OCPolarity = TIM_OCPolarity_High;

    MotorFW.PWMChanFuncs.TimerPwmChannelInit(MotorDriver_Motors_TMx, &MotorDriver_Motors_PWM);
    MotorFW.PWMChanFuncs.MotTimrPreloadCfg(MotorDriver_Motors_TMx, TIM_OCPreload_Enable);

	/*====================================*/
	/***      Setup TMx PWM CH2 BW      ***/
	/*====================================*/
    MotorDriver_Motors_PWM.TIM_Pulse = MotorBW.PWMvalue;

    MotorBW.PWMChanFuncs.TimerPwmChannelInit(MotorDriver_Motors_TMx, &MotorDriver_Motors_PWM);
    MotorBW.PWMChanFuncs.MotTimrPreloadCfg(MotorDriver_Motors_TMx, TIM_OCPreload_Enable);

    TIM_Cmd(MotorDriver_Motors_TMx, ENABLE); // start counting by enabling CEN in CR1 */
}

void MotorDriver_MotorOutputTest()
{
	int i;
	GPIO_SetBits(MotorDriver_MotorPort, MotorDriver_Mout11);
	for (i = 0; i < 500000; i++);
	GPIO_ResetBits(MotorDriver_MotorPort, MotorDriver_Mout11);

	GPIO_SetBits(MotorDriver_MotorPort, MotorDriver_Mout12);
	for (i = 0; i < 500000; i++);
	GPIO_ResetBits(MotorDriver_MotorPort, MotorDriver_Mout12);

	GPIO_SetBits(MotorDriver_MotorPort, MotorDriver_Mout21);
	for (i = 0; i < 500000; i++);
	GPIO_ResetBits(MotorDriver_MotorPort, MotorDriver_Mout21);

	GPIO_SetBits(MotorDriver_MotorPort, MotorDriver_Mout22);
	for (i = 0; i < 500000; i++);
	GPIO_ResetBits(MotorDriver_MotorPort, MotorDriver_Mout22);
}

uint8_t	MotDrv_MoveFW(uint16_t PWMvalue)
{
	uint8_t result = E_OK;

	if(motorsTimerPwm.timerPeriod < PWMvalue)
	{
		PWMvalue = (motorsTimerPwm.timerPeriod/2);
		result = E_NOK;
	}
	else
	{
		/*Do nothing*/
	}

	GPIO_SetBits(MotorDriver_MotorPort, MotorDriver_Mout11);
	GPIO_ResetBits(MotorDriver_MotorPort, MotorDriver_Mout12);

	MotorFW.PWMChanFuncs.CCPFunction(MotorDriver_Motors_TMx, PWMvalue);

	GPIO_SetBits(MotorDriver_MotorPort, MotorDriver_Mout21);
	GPIO_ResetBits(MotorDriver_MotorPort, MotorDriver_Mout22);

	MotorBW.PWMChanFuncs.CCPFunction(MotorDriver_Motors_TMx, PWMvalue);

	return result;
}

uint8_t MotDrv_MoveBW(uint16_t PWMvalue)
{
	uint8_t result = E_OK;

	if(motorsTimerPwm.timerPeriod < PWMvalue)
	{
		PWMvalue = (motorsTimerPwm.timerPeriod/2);
		result = E_NOK;
	}
	else
	{
		/*Do nothing*/
	}

	GPIO_SetBits(MotorDriver_MotorPort, MotorDriver_Mout12);
	GPIO_ResetBits(MotorDriver_MotorPort, MotorDriver_Mout11);

	MotorFW.PWMChanFuncs.CCPFunction(MotorDriver_Motors_TMx, PWMvalue);

	GPIO_SetBits(MotorDriver_MotorPort, MotorDriver_Mout22);
	GPIO_ResetBits(MotorDriver_MotorPort, MotorDriver_Mout21);

	MotorBW.PWMChanFuncs.CCPFunction(MotorDriver_Motors_TMx, PWMvalue);

	return result;
}

uint8_t MotDrv_Stop()
{
	uint8_t result = E_OK;

	GPIO_ResetBits(MotorDriver_MotorPort, MotorDriver_Mout11);
	GPIO_ResetBits(MotorDriver_MotorPort, MotorDriver_Mout12);
	GPIO_ResetBits(MotorDriver_MotorPort, MotorDriver_Mout21);
	GPIO_ResetBits(MotorDriver_MotorPort, MotorDriver_Mout22);

	MotorFW.PWMChanFuncs.CCPFunction(MotorDriver_Motors_TMx, 0);
	TIM_CCxCmd(MotorDriver_Motors_TMx, TIM_Channel_1, TIM_CCx_Enable);

	MotorBW.PWMChanFuncs.CCPFunction(MotorDriver_Motors_TMx, 0);
	TIM_CCxCmd(MotorDriver_Motors_TMx, TIM_Channel_2, TIM_CCx_Enable);

	return result;
}
