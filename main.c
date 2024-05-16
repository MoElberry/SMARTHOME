#include "main.h"

/********************************************************************/
void main(void) {

	u8 ID=0;
	u8 Password[4];
	u8 userstate=2;

	/* peripherals Initialization */
	PORT_voidInit();
	SERVO_voidInit();
	CLCD_voidInit();
	USART_voidInit();
	ADC_voidInit();
	TIMER2_voidSetDyty(1);
	TIMER2_voidInit();
	TIMER2_voidStart();

	/* Start Interfacing with user */
	CLCD_voidGoToxy(0,0);
	CLCD_voidSendString("Login");
	_delay_ms(100);

	while(1){
		/* Take User ID */
		ID=GetID();
		/* Take User Password */
		GetPASS(Password);
		/*Check is valid User ID*/
		userstate=Checkuser(ID,Password);
		switch(userstate)
		{
		/* If Wrong User */
		case 0:Wrong_user(); break;
		/* If Valid User */
		case 1:Right_user(); break;
		default: break;
		}
	}
}

/********************************************************************/
/* Function to Take User ID */
u8 GetID()
{
	CLCD_voidClearDisplay();
	CLCD_voidSendString("ID:");
	u8 Keypad_data;
	u8 id;
	while(1)
	{
		/*Blocking until take valid key */
		do {
			Keypad_data = KPD_u8PressedKey();
			_delay_ms(10);

		} while (Keypad_data == 0xff);

		CLCD_voidGoToxy(1,0);
		if (Keypad_data == '=')
		{
			return id;
		}
		id=Keypad_data;
		CLCD_voidDisplayNumber(Keypad_data);

	}
}
/********************************************************************/


/* Function to Take User Password */
void GetPASS(u8 arrpass[])
{
	u8 Keypad_data;
	u8 itteration=0;

	CLCD_voidClearDisplay();
	CLCD_voidSendString("PASS:");
	while(1)
	{
		/* Function to Take User Password */
		do {
			Keypad_data = KPD_u8PressedKey();
			_delay_ms(10);

		} while (Keypad_data == 0xff);

		if (Keypad_data == '=')
		{
			break;
		}
		arrpass[itteration]=Keypad_data;
		itteration++;
		CLCD_voidDisplayNumber(Keypad_data);
		_delay_ms(10);

	}
}
/********************************************************************/


/* Function to Check is valid User ID*/
u8 Checkuser(u8 Id,u8 arrpass[])
{
	u8 receive=0;
	/*Convert Data to Ascii*/
	u8 arr[]={convert(Id),convert(arrpass[0]),convert(arrpass[1]),convert(arrpass[2]),convert(arrpass[3]),'#'};

	/*Send ID & Password to admin */
	USART_voidSendString(arr);
	_delay_ms(1000);
	/*receive response from admin*/
	receive=USART_u8Receive();

	if(receive=='1')
	{
		/*if valid user*/
		return 1;
	}

	else if(receive=='0')
	{
		/*if wrong user */
		return 0;
	}
	else
	{
		/*if NACK*/
		return 0;
	}

}
/********************************************************************/

/*If Valid User Will enter To System*/
void Right_user()
{
	u8 Keypad_data=0;
	u8 PowerLEd=0;

	CLCD_voidClearDisplay();
	CLCD_voidSendString("Right user");
	/*OPEN DOOR*/
	SERVO_voidSetAngle(90);
	_delay_ms(100);

	while(1)
	{
		/*List Display*/
		CLCD_voidClearDisplay();
		CLCD_voidGoToxy(0,0);
		CLCD_voidSendString("DOOR-1");
		CLCD_voidSendString(" ADC-2");
		CLCD_voidSendString(" L1-3");
		CLCD_voidGoToxy(1,0);
		CLCD_voidSendString( "L2-4 L3-5 Ex-6");
		/*Blocking until take choice */
		do {
			Keypad_data = KPD_u8PressedKey();
			_delay_ms(10);

		} while (Keypad_data == 0xff);

		switch(Keypad_data)
		{
		/*Door controlling*/
		case 1:
			CLCD_voidClearDisplay();
			CLCD_voidGoToxy(0,0);
			CLCD_voidSendString("Open Door-1");
			CLCD_voidGoToxy(1,0);
			CLCD_voidSendString("Close Door-0");
			/*Blocking until take choice*/
			do {
				Keypad_data = KPD_u8PressedKey();
				_delay_ms(10);

			} while (Keypad_data == 0xff);

			if(Keypad_data==1)
			{
				/*Open door*/
				SERVO_voidSetAngle(90);

			}
			else if(Keypad_data==0)
			{
				/*close door*/
				SERVO_voidSetAngle(0);
			}
			break;
			/*ADC Trigger choice*/
		case 2:
			ADCOperation();
			break;
		case 3:
			/*LED1 choice*/
			CLCD_voidClearDisplay();
			CLCD_voidGoToxy(0,0);
			CLCD_voidSendString("Open LED-1");
			CLCD_voidGoToxy(1,0);
			CLCD_voidSendString("Close LED-0");
			do {
				Keypad_data = KPD_u8PressedKey();
				_delay_ms(10);

			} while (Keypad_data == 0xff);
			LED1(Keypad_data);
			break;
		case 4:
			/*LED2 choice*/
			CLCD_voidClearDisplay();
			CLCD_voidGoToxy(0,0);
			CLCD_voidSendString("Open LED-1");
			CLCD_voidGoToxy(1,0);
			CLCD_voidSendString("Close LED-0");
			do {
				Keypad_data = KPD_u8PressedKey();
				_delay_ms(10);

			} while (Keypad_data == 0xff);
			LED2(Keypad_data);
			break;
		case 5:
			/*LED3 choice*/
			CLCD_voidClearDisplay();
			CLCD_voidGoToxy(0,0);
			CLCD_voidSendString("Enter LEd Pwr");
			u8 key2=0;
			/*Taking PWM For LED3 blocking until see '='*/
			while(1)
			{
				key2=KPD_u8PressedKey();
				if(key2!=0xff)
				{
					if(key2=='=')
					{
						break;
					}
					else
					{
						PowerLEd=PowerLEd*10+key2;
					}
				}
			}


			CLCD_voidGoToxy(1,0);
			CLCD_voidDisplayNumber(PowerLEd);
			PowerLED3(PowerLEd);
			_delay_ms(1000);
			break;
			/*Exit From System*/
		case 6:
			CLCD_voidClearDisplay();
			CLCD_voidGoToxy(0,0);
			CLCD_voidSendString("pay pay");
			_delay_ms(1000);
			return;

		}


	}
}
/********************************************************************/


/*if wrong user*/
void Wrong_user()
{
	u8 Localid;
	u8 localpass[4];
	u8  localit;
	u8 localStatus=2;

	CLCD_voidClearDisplay();
	CLCD_voidSendString("Wrong user");
	_delay_ms(1000);
	/*taking Id and password again until reach to three trials*/
	for(localit=1; localit<MAX_Trials;  localit++)
	{
		Localid=GetID();
		GetPASS(localpass);
		localStatus=Checkuser(Localid,localpass);

		switch(localStatus)
		{
		case 2:break;
		case 0:
			CLCD_voidClearDisplay();
			CLCD_voidSendString("Wrong user");
			CLCD_voidDisplayNumber(localit);
			_delay_ms(1000);
			continue;
			break;
		case 1:localit=0;Right_user(); break;
		}

	}
	CLCD_voidClearDisplay();
	/*if reach three trials System will locked*/
	/*open led and buzzer*/
	DIO_u8SetPinValue(DIO_u8PORTD,DIO_u8PIN4,DIO_u8PIN_HIGH);
	DIO_u8SetPinValue(DIO_u8PORTD,DIO_u8PIN3,DIO_u8PIN_HIGH);
	CLCD_voidSendString("Locked System");
	while(1);
}
/********************************************************************/


/*ADC work will open fan if reach threshold*/
void ADCOperation()
{
	u16 ADC_Read=0;
	ADC_u8StartConversionSynch(ADC_A7, &ADC_Read);
	CLCD_voidClearDisplay();
	CLCD_voidGoToxy(0,0);
	CLCD_voidDisplayNumber(ADC_Read);
	_delay_ms(1000);
	if(ADC_Read>13)
	{
		//open fan
		CLCD_voidClearDisplay();
		CLCD_voidGoToxy(0,0);
		CLCD_voidSendString("Fan Opened");

		H_BRIDGE_Start(90,90);
		_delay_ms(1000);
	}
	else if (ADC_Read<11)
	{
		//closed fan
		CLCD_voidClearDisplay();
		CLCD_voidGoToxy(0,0);
		CLCD_voidSendString("Fan Closed");
		H_BRIDGE_Stop();

		_delay_ms(1000);
	}
	else
	{
		CLCD_voidClearDisplay();
		CLCD_voidGoToxy(0,0);
		CLCD_voidSendString("Stable state");
		_delay_ms(1000);
	}
}
/********************************************************************/


/*Control Led1*/
void LED1(u8 states)
{
	if(states==1)
	{
		DIO_u8SetPinValue(DIO_u8PORTD,DIO_u8PIN2,DIO_u8PIN_HIGH);	}
	else
	{
		DIO_u8SetPinValue(DIO_u8PORTD,DIO_u8PIN2,DIO_u8PIN_LOW);
	}
}
/********************************************************************/

/*Control Led2*/
void LED2(u8 states)
{
	if(states==1)
	{
		DIO_u8SetPinValue(DIO_u8PORTD,DIO_u8PIN6,DIO_u8PIN_HIGH);	}
	else
	{
		DIO_u8SetPinValue(DIO_u8PORTD,DIO_u8PIN6,DIO_u8PIN_LOW);
	}
}
/********************************************************************/


/*Set Led3 Power*/
void PowerLED3(u8 POWER)
{
	TIMER2_voidSetDyty(POWER);

}
/********************************************************************/


/*convert to ascii*/
u8 convert(u8 Copy_u8number)
{
	switch(Copy_u8number)
	{
	case 0 :return('0');break;
	case 1 :return('1'); break;
	case 2 :return('2') ; break;
	case 3 :return('3') ; break;
	case 4 :return('4') ; break;
	case 5 :return('5') ; break;
	case 6 :return('6') ; break;
	case 7 :return('7') ; break;
	case 8 :return('8') ; break;
	case 9 :return('9') ; break;
	}
}
