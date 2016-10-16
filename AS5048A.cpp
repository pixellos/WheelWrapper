#include "AS5048A.hpp"

using namespace CodeEasyAvr;
class AS5048A
{
public:
	uint16_t CalculatedPosition = 0;
	uint16_t Offset = 0;
private:
	uint16_t RealPosition = 0;
	
	uint8_t AngleValue = 0;
	uint8_t lastAngleValue =0;
	uint8_t Spin=0;
	uint8_t PureHighByte=0;
	uint8_t PureLowByte = 0;
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
		PureHighByte = SPDR;
		AngleValue = (PureHighByte)<<4 ;//First  byte
		SPDR = 0xff;
		while(!(SPSR & (1<< SPIF)));
		PureLowByte = SPDR;
		AngleValue |= (PureLowByte)>>4;//Second Byte
		Disable();
	}
	///It's invoked inside compute position
	const uint16_t CenterValue = 0x8000;
	const uint16_t LeftValue = CenterValue - 0x5000;
	const uint16_t RightValue = CenterValue + 0x5000;
	
	
	void SetProperPositionToComputePosition()
	{
		if (Offset == 0) //Initialize
		{
			if (RealPosition > CenterValue)
			{
				Offset = RealPosition - CenterValue;
			}
			else
			{
				Offset = CenterValue - RealPosition;
			}
		}
		CalculatedPosition = RealPosition - Offset ;
		if (CalculatedPosition < LeftValue)
		{
			Offset -= (LeftValue - CalculatedPosition);
		}

		if (CalculatedPosition > RightValue)
		{
			Offset += (CalculatedPosition - RightValue);
		}
	}

	void ComputePosition()
	{
		ReadData();
		if (lastAngleValue > 192 && AngleValue <63 ) ++Spin;
		if (AngleValue > 192 && lastAngleValue<63 )	--Spin;

		RealPosition = (uint16_t)((Spin<<9) |(AngleValue<<1) | (PureLowByte >>3));
		SetProperPositionToComputePosition();
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
