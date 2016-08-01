#include "SPL/Atmega16A_SPL.hpp"

class ShiftRegisterButtons
{
	private:
	PinSBase *_RegisterClk,*_RegisterPL,*_RegisterQ7;
	public:
	volatile uint16_t lastData;
	ShiftRegisterButtons(PinSBase &Clk, PinSBase &PL, PinSBase &Q)
	{
		_RegisterClk = &Clk;
		_RegisterPL = &PL;
		_RegisterQ7 = &Q;
	}

	void Initialize()
	{
		_RegisterClk->AsOutput();
		_RegisterPL->AsOutput();
		_RegisterQ7->AsInput();
		_RegisterPL->Clear();
	}

	uint8_t FirstReadStage()
	{
		_RegisterPL->Set();
		asm("nop");
		_RegisterPL->Clear();	
		uint8_t data = 0;
		for (int i = 0;i < 15;i++)
		{
			_RegisterClk->Clear();
			data <<= 1;
			if (!_RegisterQ7->Check())
			{
				data++;
			}
			_RegisterClk->Set();
		}
		return data;
	}
	uint8_t SecondReadStage()
	{
		uint8_t data = 0;
		for (int i = 0;i < 8;i++)
		{
			_RegisterClk->Clear();
			data <<= 1;
			if (!_RegisterQ7->Check())
			{
				data++;
			}
			_RegisterClk->Set();
		}
		return data;
	}
	
	uint16_t ReadData()
	{
		_RegisterPL->Set();
		_RegisterPL->Clear();
		lastData = 1;
		for (int i = 22;i >=0;i--)
		{
			_RegisterClk->Clear();
			lastData <<= 1;
			if (!_RegisterQ7->Check())
			{
				lastData++;
			}
			_RegisterClk->Set();
		}
		return lastData;
	}
};

