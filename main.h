
#ifndef MAIN_H_
#define MAIN_H_

#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "DIO_interface.h"
#include "PORT_interface.h"
#include "CLCD_interface.h"
#include "KEYPAD_interface.h"
#include "ADC_interface.h"
#include "H_bridge_interface.h"
#include "SERVO_interface.h"
#include "USART_interface.h"
#include "util/delay.h"

#define MAX_Trials    3
u8 GetID();
void GetPASS(u8 arrpass[]);
u8 Checkuser(u8 Id,u8 arrpass[]);
void Right_user();
void Wrong_user();

void ADCOperation();
void LED2(u8 states);
void LED1(u8 states);

void PowerLED3(u8 POWER);

u8 convert(u8 Copy_u8number);

#endif /* MAIN_H_ */
