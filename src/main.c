/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "gpio.h"
#include "MotorDriver.h"
#include "MotorControl.h"
			

int main(void)
{
//	SystemInit();
//	SystemCoreClockUpdate();
	GPIO_init();
	Gpio_OutputTest();
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
//	 EXTI_GenerateSWInterrupt(EXTI_Line4);
//	 EXTI_GenerateSWInterrupt(EXTI_Line3);
	PWR_WakeUpPinCmd(ENABLE);

	while(1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//		result = MotDrv_MoveFW(60);
//		for (i = 0; i < 15000000; i++);
//		result = MotDrv_MoveBW(30);
//		for (i = 0; i < 15000000; i++);
//		result = MotDrv_Stop();
//		for (i = 0; i < 15000000; i++);
	}
}

void EXTI1_IRQHandler(void) //EXTI1_IRQn
{
    /* Make sure that interrupt flag is set */
    if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
        /* Do your stuff when PB3 is changed */
    	if (!(GPIO_ReadInputData(GPIOB) & GPIO_Pin_12))
    	{
    		// Turn on LED on PB12 (LED circuit is active low)
    		GPIO_SetBits(GPIOB, GPIO_Pin_12);
    	}
    	else
		{
			// Turn off LED on PB12
    		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		}

        /* Clear interrupt flag */
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}

void EXTI0_IRQHandler(void) //EXTI4_IRQn
{
	/* Make sure that interrupt flag is set */
	    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
	        /* Do your stuff when PB4 is changed */
	    	if (!(GPIO_ReadInputData(GPIOB) & GPIO_Pin_13))
	        	{
	        		// Turn on LED on PB12 (LED circuit is active low)
	        		GPIO_SetBits(GPIOB, GPIO_Pin_13);
	        	}
	        	else
	        	{
	        		GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	        	}

	        /* Clear interrupt flag */
	        EXTI_ClearITPendingBit(EXTI_Line0);
	    }
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	  {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		if (!(GPIO_ReadInputData(GPIOC) & GPIO_Pin_13))
		{
			// Turn on LED on PA0 (LED circuit is active low)
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		}
		else
		{
			// Turn off LED on PA0
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		}
	  }
}
