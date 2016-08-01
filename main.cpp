#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "CppHelpers.h"
#include "avr/interrupt.h"
#include "SPL/Avr5Lib.hpp"
#include "SPL/Atmega16A_SPL.hpp"
#include "TaskManager/TaskManager.hpp"
#include "Variables.h"
#include "MyTypes.h"
void Initialize()
{
	DDRC = 0xff;//Port c Set as output
	PORTC = 0;//All low
	
	DDRD = 0x00;		// Set PD2 as input (Using for interupt INT0)
	PORTD = 0x00;
	
	GICR = 1<<INT0;					// Enable INT0
	MCUCR = 1<<ISC01 | 1<<ISC00;	// Trigger INT0 on rising edge
	
	//sei();				//Enable Global Interrupt
}

PIN_D6 firstByte;
PIN_D4 secondByte;
PIN_D3 thridByte;

uint8_t debugVar;

uint8_t volatile outBuffor[8];
ISR(INT0_vect)
{
	switch(debugVar = (thridByte.Check() << 2 | secondByte.Check() << 1 | firstByte.Check() <<0))
	{
		case 0:
		PORTC = AS5048AChip.CalculatedPosition;
		break;
		case 1:
		PORTC = AS5048AChip.CalculatedPosition>>8;
		break;
		case 2:
		PORTC = outBuffor[2];
		break;
		case 3:
		PORTC = outBuffor[3];
		break;
		case 4:
		PORTC = 0xf0;
		break;
		case 5:
		PORTC = 0xf1;
		break;
		case 6:
		PORTC = 0b10011001; //It's security transfer const 
		//- if this is corupted transminssion will be canceled
		break;
		case 7:
		PORTC = outBuffor[0] ^ outBuffor[1] ^ outBuffor[2] ^ outBuffor[3] ^ 
		outBuffor[4] ^ outBuffor[5] ^ outBuffor[6] ^ outBuffor[7]; //CRC
		break;
	}
}
void NextCalibrateStage()
{
	switch (calibrateStage)
	{
		case CalibrateStage::No:
			calibrateStage = CalibrateStage::Left;
			ICR1 = 0b1111111111;
		break;

		case CalibrateStage::Left:
			AS5048AChip.SetZeroPosition();
			calibrateStage = CalibrateStage::Right;
		break;

		case CalibrateStage::Right:
			calibrateStage = CalibrateStage::Calibrated;
			RotateLimit = (AS5048AChip.CalculatedPosition);
		break;
		case CalibrateStage::Calibrated:
			calibrateStage = CalibrateStage::No;
		break;
	}
}

void static EnableAsync()
{
	Timer0::SetCompareValue(7); 
	Timer0::WaveFormMode(Timer0::WaveForms::CTC);
	Timer0::SetInterrupts(CodeEasyAvr::Timer0::CompareMatchInterrupt);
	Timer0::SetPrescaler(Timer0::Prescalers::Prescaler_256);
	sei();
}


void DoFirstKeyboardByte(Task * task)
{
	outBuffor[3] = wheelButtons.FirstReadStage();
}


void DoSecondKeyboardByte(Task * task)
{
	outBuffor[4] = wheelButtons.SecondReadStage();
}

void DoDataTask(Task * task)
{
	switch(debugVar = (thridByte.Check() << 2 | secondByte.Check() << 1 | firstByte.Check() <<0))
	{
		case 0:
		PORTC = AS5048AChip.CalculatedPosition;
		break;
		case 1:
		PORTC = AS5048AChip.CalculatedPosition>>8;
		break;
		case 2:
		PORTC = outBuffor[2];
		break;
		case 3:
		PORTC = outBuffor[3];
		break;
		case 4:
		PORTC = 0xf0;
		break;
		case 5:
		PORTC = 0xf1;
		break;
		case 6:
		PORTC = 0b10011001; //It's security transfer const
		//- if this is corupted transminssion will be canceled
		break;
		case 7:
		PORTC = outBuffor[0] ^ outBuffor[1] ^ outBuffor[2] ^ outBuffor[3] ^
		outBuffor[4] ^ outBuffor[5] ^ outBuffor[6] ^ outBuffor[7]; //CRC
		break;
	}
}

void RegisterTasks()
{
	getFirstKeyboardByte.currentMethod = DoFirstKeyboardByte;
	getSecondKeyboardByte.currentMethod = DoSecondKeyboardByte;
	setDataTask.currentMethod = DoDataTask;
	Taskmanager.Register(getFirstKeyboardByte);
	Taskmanager.Register(getSecondKeyboardByte);
	//Taskmanager.Register(setDataTask);
	
	//keyboardTask.currentMethod = KeyboardTask;
	//Taskmanager.Register(keyboardTask);
}

void ComputePWM()
{
	AS5048AChip.ComputePosition();
	if ( (AS5048AChip.CalculatedPosition) <= RotateLimit)
	{
		OCR1A = (ICR1*((float)AS5048AChip.CalculatedPosition/RotateLimit));
	}
}

void InitializePorts()
{
	PORTD= PORTC = PORTB= DDRB = DDRD =  DDRC = DDRA = 0xff;
}

void InitializeProgram()
{
	InitializePorts();

	#ifdef DebugMode__
	debugDisplay.Enable();
	debugDisplay.Initialize();
	debugDisplay.Disable();
	#endif
	AS5048AChip.Initalize();
	AS5048AChip.Disable();
	#ifdef DebugMode__
	debugDisplay.Enable();
	#endif
	RegisterTasks();
	EnableAsync();
	//PWMInitialize();
	wheelButtons.Initialize();
	NextCalibrateStage();
	Initialize();
}

#ifdef DebugMode__
void Debug()
{
	debugDisplay.PrintDebugMessage(debugVar);
	debugDisplay.ChangeActiveLine(6);
	debugDisplay.PrintDataBin(AS5048AChip.CalculatedPosition);
}
#endif

int main()
{
	InitializeProgram();
	while("forever")
	{
		AS5048AChip.ComputePosition();
		#ifdef DebugMode__
		debugDisplay.Enable();
		Debug();
		debugDisplay.Disable();
		#endif
	}
}
