#include "AS5048A.hpp"

using namespace CodeEasyAvr;
class AS5048A
{
public:
	uint16_t CalculatedPosition = 0;
private:
	uint8_t AngleValue = 0;
	uint8_t lastAngleValue =0;
	uint8_t Spin=0;
	uint8_t ErrorValue = 0;
	PinSBase * _Mosi;
	PinSBase * _ChipSelect;
	PinSBase * _Clk;
	PinSBase * _Miso;
	PinSBase * _SS;
public:
	AS5048A(PinSBase& Mosi, PinSBase &SS, PinSBase& Clk, PinSBase &Miso, PinSBase& ChipSelect)
	{
		_Miso = &Miso;
		_Mosi = &Mosi;
		_ChipSelect = &ChipSelect;
		_SS = &SS;
		_Clk = &Clk;
	}

	void Initalize()
	{
		_ChipSelect->AsOutput();
		_Miso->AsInput();
		_Mosi->AsOutput();
		_Clk->AsOutput();
		_SS->AsOutput();

		Disable();
		SPI::InitializeAsMaster();
		SPI::SetDoubleSpeed();
		Enable();
		
		SPDR = 0xff;
		while(!(SPSR & (1<< SPIF)));
		SPDR = 0xff;
		while(!(SPSR & (1<< SPIF)));

		Disable();
	}

	void ReadData()
	{
		Enable();
		lastAngleValue = AngleValue;
		SPDR = 0xff;
		while(!(SPSR & (1<< SPIF)));
		AngleValue = (SPDR&0b00011111)<<3 ;//First  byte
		SPDR = 0xff;
		while(!(SPSR & (1<< SPIF)));
		AngleValue |= (SPDR)>>5;//Second Byte
		Disable();
	}

	void ComputePosition()
	{
		ReadData();
		if (lastAngleValue > 192 && AngleValue <63 ) ++Spin;
		if (AngleValue > 192 && lastAngleValue<63 )	--Spin;

		CalculatedPosition = (uint16_t)(((Spin<<8) |(AngleValue)) - (ErrorValue));
	}

	void SetZeroPosition()
	{
		Spin = 0;
		ErrorValue = AngleValue;
	}

	void Enable()
	{
		_ChipSelect->Clear();
	}
	void Disable()
	{
		_ChipSelect->Set();
	}
};
