
#include "stm32f10x.h"

enum
{
	E_NOK,
	E_OK
};

void MotorDrv_init();
void MotorDriver_MotorOutputTest();
uint8_t	MotDrv_MoveFW(uint16_t PWMvalue);
uint8_t MotDrv_MoveBW(uint16_t PWMvalue);
uint8_t MotDrv_Stop();
