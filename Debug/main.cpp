#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include "Avr5Lib.cpp"
#include "CppHelpers.h"
#include "Atmega16A_SPL.hpp"
#include "TaskManager.hpp"

volatile static int16_t ledCounter = 0;
volatile static bool BlinkFlag =false;
volatile uint16_t ButtonPressTime = 0;

PIN_D7 Button;
PIN_B2 GreenLed;
PIN_B0 RedLed	;	//Controled by -, if Cleared - it shine
PIN_B1 YellowLed;		//Controled by -, if Cleared - it shine
/*
template<typename Sck,typename Mosi,typename Reset,typename DC>
class DisplaySH1106WithoutReset{
private:

public:
	void InitializeSync()//Synchronous version - dont place it inside task!!!!!!!!!!!!!!!1
	{
	Mosi.FastAsOutput();
	Sck.FastAsOutput();
	DC.FastAsOutput();

	SPI::SpiMode3();
	SPI::AsMaster();
	SPI::SPIEnable();
	}

	class enum DataCategory{
	Command=0,
	Data = 1,
	};

	void SendSynchronous(uint8_t byteFrame,const DataCategory dataCategory){
	//CS HIGH
	DC.BoolSet((bool)dataCategory);

	while(!SPI::CheckOperationCompleteFlag();)
	SPI << byteFrame;
	}
};*/

void LedBlinkPerDelayVariableMs(Task * task)
{
	if (BlinkFlag)
	{
		if (ledCounter > 700)
		{
			ledCounter = 0;
			YellowLed.Toggle();
		}
		++ledCounter;
	}
	else
	{
		YellowLed.Set();
	}
}
void Test(Task * task);

void KeyboardHandler(Task * task)
{
task->currentMethod = Test;
if (!Button.Check()) //Atmega has only internal pull ups - so button must be setted (1), and button must ground it. If pin == 0 then is pressed
	{
		++ButtonPressTime;
	}
	else 
	{
		if (ButtonPressTime > 220)
		{
			BlinkFlag = !BlinkFlag;
		}
		else if (ButtonPressTime > 15)
		{
			GreenLed.Toggle();
		}
			ButtonPressTime = 0;
	}
}

void Test(Task * task)
{
task->currentMethod = KeyboardHandler;
}

Task taskName;
Task keyboardTask;
void RegisterTasks()
{
	taskName.currentMethod = LedBlinkPerDelayVariableMs;
	Taskmanager.Register(taskName);
	keyboardTask.currentMethod = KeyboardHandler;
	Taskmanager.Register(keyboardTask);
}

using namespace CodeEasyAvr;
void static EnableAsync()
{  //2000 instructions per task - 1ms task resolution CAREFULL: Exit mail will erase interrupt SREG
	Timer0::SetCompareValue(7); // Why 7? When we get 16mhz, divide it 8 (8 tasks), we get 2mhz, divide it 8*256 its 2048 - 1 ms -
	Timer0::WaveFormMode(Timer0::WaveForms::CTC);
	Timer0::SetInterrupts(CodeEasyAvr::Timer0::CompareMatchInterrupt);
	Timer0::SetPrescaler(Timer0::Prescalers::Prescaler_256);
	sei();
}

void InitializeProgram()
{
	Button.AsInput();

	Button.Set();

	 GreenLed.FastAsOutput();
	   RedLed.FastAsOutput();
	YellowLed.FastAsOutput();

	 GreenLed.FastSet();
	   RedLed.FastSet();
	YellowLed.FastSet();
	RegisterTasks();
	EnableAsync();
}

int main()
{
	InitializeProgram();
	while(1){}
}	//Important, if program get out of main SREG bit will be cleared!

