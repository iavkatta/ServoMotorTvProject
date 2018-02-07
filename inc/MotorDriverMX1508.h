#include "stm32f10x.h"
//#include "moduleSwitches.h"

#ifndef MotorDriverMX1508_H_
#define MotorDriverMX1508_H_

#ifdef	MOTDRV_MX1508_DEBUG

typedef enum
{
	FW = 0,
	BW,
	nmbrDirections
}MotorDriver_tDirection;

typedef enum
{
	E_NOK,
	E_OK
}MotorDriver_tResult;

void MotorDriverMX1508_Init();
void MotorDriverMX1508_MotorOutputTest();
MotorDriver_tResult	MotorDriverMX1508_Move(uint16_t PWMvalue, MotorDriver_tDirection direction);
uint8_t MotorDriverMX1508_Stop();

#endif


#endif /* MotorDriverMX1508_H_ */
/*EOF*/
