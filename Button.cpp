#include "SPL/Atmega16A_SPL.hpp"
typedef void(*Action)();

class SingleButton // Button connected to gnd
{
	private:
	PinSBase * _ButtonPin;
	volatile uint16_t _PressTime;
	Action _ClickAction = nullptr;
	Action _HoldAction = nullptr;
	public:
	const uint8_t MinClickTime = 15;
	const uint8_t MinHoldTime = 220;
	SingleButton(PinSBase &pin, Action click,Action hold)
	{
		_ButtonPin = &pin;
		Initialize();
		_ClickAction = click;
		_HoldAction = hold;
	}

	void Initialize()
	{
		_ButtonPin->AsInput();
		_ButtonPin->Set();
		_PressTime = 0;
	}

	void CheckButtonState()
	{
		if (!_ButtonPin->Check()) //Atmega has only internal pull ups - so button must be setted (1), and button must ground it. If pin == 0 then is pressed
		{
			++_PressTime;
		}
		else
		{
			if (_PressTime > MinHoldTime)
			{
				_HoldAction();
			}
			else if (_PressTime > MinClickTime)
			{
				_ClickAction();
			}
			_PressTime = 0;
		}
	}
};


