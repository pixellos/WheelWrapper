/* 
* Sh1106Object.cpp
*
* Created: 10/02/2016 16:33:10
* Author: rogoz
*/
#include "SPL/Atmega16A_SPL.hpp"
#include "SPL/Avr5Lib.hpp"
#include "MyTypes.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/eeprom.h>
const uint8_t  EEMEM font5x7[88*5] =
{
	0x00, 0x1C, 0x22, 0x41, 0x00,
	0x00, 0x41, 0x22, 0x1C, 0x00,
	0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
	0x08, 0x08, 0x3E, 0x08, 0x08,
	0x00, 0x80, 0x70, 0x30, 0x00,
	0x08, 0x08, 0x08, 0x08, 0x08,
	0x00, 0x00, 0x60, 0x60, 0x00,
	0x20, 0x10, 0x08, 0x04, 0x02,
	0x3E, 0x51, 0x49, 0x45, 0x3E,
	0x00, 0x42, 0x7F, 0x40, 0x00,
	0x72, 0x49, 0x49, 0x49, 0x46,
	0x21, 0x41, 0x49, 0x4D, 0x33,
	0x18, 0x14, 0x12, 0x7F, 0x10,
	0x27, 0x45, 0x45, 0x45, 0x39,
	0x3C, 0x4A, 0x49, 0x49, 0x31,
	0x41, 0x21, 0x11, 0x09, 0x07,
	0x36, 0x49, 0x49, 0x49, 0x36,
	0x46, 0x49, 0x49, 0x29, 0x1E,
	0x00, 0x00, 0x14, 0x00, 0x00,
	0x00, 0x40, 0x34, 0x00, 0x00,
	0x00, 0x08, 0x14, 0x22, 0x41,
	0x14, 0x14, 0x14, 0x14, 0x14,
	0x00, 0x41, 0x22, 0x14, 0x08,
	0x02, 0x01, 0x59, 0x09, 0x06,
	0x3E, 0x41, 0x5D, 0x59, 0x4E,
	0x7C, 0x12, 0x11, 0x12, 0x7C,
	0x7F, 0x49, 0x49, 0x49, 0x36,
	0x3E, 0x41, 0x41, 0x41, 0x22,
	0x7F, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x49, 0x49, 0x49, 0x41,
	0x7F, 0x09, 0x09, 0x09, 0x01,
	0x3E, 0x41, 0x41, 0x51, 0x73,
	0x7F, 0x08, 0x08, 0x08, 0x7F,
	0x00, 0x41, 0x7F, 0x41, 0x00,
	0x20, 0x40, 0x41, 0x3F, 0x01,
	0x7F, 0x08, 0x14, 0x22, 0x41,
	0x7F, 0x40, 0x40, 0x40, 0x40,
	0x7F, 0x02, 0x1C, 0x02, 0x7F,
	0x7F, 0x04, 0x08, 0x10, 0x7F,
	0x3E, 0x41, 0x41, 0x41, 0x3E,
	0x7F, 0x09, 0x09, 0x09, 0x06,
	0x3E, 0x41, 0x51, 0x21, 0x5E,
	0x7F, 0x09, 0x19, 0x29, 0x46,
	0x26, 0x49, 0x49, 0x49, 0x32,
	0x03, 0x01, 0x7F, 0x01, 0x03,
	0x3F, 0x40, 0x40, 0x40, 0x3F,
	0x1F, 0x20, 0x40, 0x20, 0x1F,
	0x3F, 0x40, 0x38, 0x40, 0x3F,
	0x63, 0x14, 0x08, 0x14, 0x63,
	0x03, 0x04, 0x78, 0x04, 0x03,
	0x61, 0x59, 0x49, 0x4D, 0x43,
	0x00, 0x7F, 0x41, 0x41, 0x41,
	0x02, 0x04, 0x08, 0x10, 0x20,
	0x00, 0x41, 0x41, 0x41, 0x7F,
	0x04, 0x02, 0x01, 0x02, 0x04,
	0x40, 0x40, 0x40, 0x40, 0x40,
	0x00, 0x03, 0x07, 0x08, 0x00,
	0x20, 0x54, 0x54, 0x78, 0x40,
	0x7F, 0x28, 0x44, 0x44, 0x38,
	0x38, 0x44, 0x44, 0x44, 0x28,
	0x38, 0x44, 0x44, 0x28, 0x7F,
	0x38, 0x54, 0x54, 0x54, 0x18,
	0x00, 0x08, 0x7E, 0x09, 0x02,
	0x18, 0xA4, 0xA4, 0x9C, 0x78,
	0x7F, 0x08, 0x04, 0x04, 0x78,
	0x00, 0x44, 0x7D, 0x40, 0x00,
	0x20, 0x40, 0x40, 0x3D, 0x00,
	0x7F, 0x10, 0x28, 0x44, 0x00,
	0x00, 0x41, 0x7F, 0x40, 0x00,
	0x7C, 0x04, 0x78, 0x04, 0x78,
	0x7C, 0x08, 0x04, 0x04, 0x78,
	0x38, 0x44, 0x44, 0x44, 0x38,
	0xFC, 0x18, 0x24, 0x24, 0x18,
	0x18, 0x24, 0x24, 0x18, 0xFC,
	0x7C, 0x08, 0x04, 0x04, 0x08,
	0x48, 0x54, 0x54, 0x54, 0x24,
	0x04, 0x04, 0x3F, 0x44, 0x24,
	0x3C, 0x40, 0x40, 0x20, 0x7C,
	0x1C, 0x20, 0x40, 0x20, 0x1C,
	0x3C, 0x40, 0x30, 0x40, 0x3C,
	0x44, 0x28, 0x10, 0x28, 0x44,
	0x4C, 0x90, 0x90, 0x90, 0x7C,
	0x44, 0x64, 0x54, 0x4C, 0x44,
	0x00, 0x08, 0x36, 0x41, 0x00,
	0x00, 0x00, 0x77, 0x00, 0x00,
	0x00, 0x41, 0x36, 0x08, 0x00,
	0x02, 0x01, 0x02, 0x04, 0x02,
};
const char EEMEM AString[3] ="A:";
const char EEMEM BString[3] ="B:";
const char EEMEM CString[3] ="C:";
const char EEMEM DString[3] ="D:";
const char EEMEM DebugLabel[27] = "  |PORT|| DDR|| PIN|";

using namespace CodeEasyAvr;

class Sh1106{
public:
	void RegisterDataToBinString(unsigned char tabs[], uint8_t registerData)
	{
		for (int i = 0; i < 8; i++)
		{
			if (registerData & (1<<i))
			{
				tabs[i] = '1';
			}
			else
			{
				tabs[i] = '0';
			}
		}
	}

	void RegisterDataToBinString(char tabs[], uint16_t registerData)
	{
		for (int i = 0; i < 16; i++)
		{
			if (registerData & (1<<i))
			{
				tabs[i] = '1';
			}
			else
			{
				tabs[i] = '0';
			}
		}
	}

	void PrintRegisterDataHex(uint8_t registerData)
	{
		PrintChar('|');
		for (uint8_t i = 0; i < 4; i++)
		{	
			PrintChar( '0' + ((registerData>>(i*2)) & 0b00000011));
		}
		PrintChar('|');
	}

		void PrintDataBin(uint16_t data)
		{
			PrintChar('|');
			for (int i = 15; i >=0; i--)
			{
				if (data& (1<<i))
				{
					PrintChar('1');
				}
				else
				{
					PrintChar('0');
				}
			}
			PrintChar('|');
		}

	void PrintDataBin(uint8_t data)
	{
		PrintChar('|');
		for (int i = 7; i >=0; i--)
		{
			if (data& (1<<i))
			{
				PrintChar('1');
			}
			else
			{
				PrintChar('0');
			}
		}
		PrintChar('|');
	}

	void PrintDebugMessage(uint8_t data)
	{	
		char labels[28];
		eeprom_read_block(labels,DebugLabel,26);
		this->ChangeActiveLine(0);
		this->PrintString(labels);

		eeprom_read_block(labels,AString,3);
		this->ChangeActiveLine(1);
		PrintString(labels);
		PrintRegisterDataHex(PORTA);
		PrintRegisterDataHex(DDRA);
		PrintRegisterDataHex(PINA);	

		eeprom_read_block(labels,BString,3);
		this->ChangeActiveLine(2);
		PrintString(labels);
		PrintRegisterDataHex(PORTB);
		PrintRegisterDataHex(DDRB);
		PrintRegisterDataHex(PINB);		

		eeprom_read_block(labels,CString,3);
		this->ChangeActiveLine(3);
		PrintString(labels);
		PrintRegisterDataHex(PORTC);
		PrintRegisterDataHex(DDRC);
		PrintRegisterDataHex(PINC);
		
		eeprom_read_block(labels,DString,3);
		this->ChangeActiveLine(4);
		PrintString(labels);
		PrintRegisterDataHex(PORTD);
		PrintRegisterDataHex(DDRD);
		PrintRegisterDataHex(PIND);
		this->ChangeActiveLine(5);
		
		PrintRegisterDataHex(data);
	}
private:
	PinSBase * _DataCommand;
	PinSBase * _Reset;
	PinSBase * _Mosi;
	PinSBase * _ChipSelect;
	PinSBase * _Clk;
	PinSBase * _Miso;
	PinSBase * _SS;
protected:
#pragma region Const
	const uint8_t REFRESH_MIN = 0x80;
	const uint8_t REFRESH_MID = 0xB0;
	const uint8_t REFRESH_MAX = 0xF0;
	const uint8_t SH1106_SETCONTRAST = 0x81;
	const uint8_t SH1106_DISPLAYALLON_RESUME = 0xA4;
	const uint8_t SH1106_DISPLAYALLON = 0xA5;
	const uint8_t SH1106_NORMALDISPLAY = 0xA6;
	const uint8_t SH1106_INVERTDISPLAY = 0xA7;
	const uint8_t SH1106_DISPLAYOFF = 0xAE;
	const uint8_t SH1106_DISPLAYON = 0xAF;
	const uint8_t SH1106_SETDISPLAYOFFSET = 0xD3;
	const uint8_t SH1106_SETCOMPINS = 0xDA;
	const uint8_t SH1106_SETVCOMDETECT = 0xDB;
	const uint8_t SH1106_SETDISPLAYCLOCKDIV = 0xD5;
	const uint8_t SH1106_SETPRECHARGE = 0xD9;
	const uint8_t SH1106_SETMULTIPLEX = 0xA8;
	const uint8_t SH1106_SETLOWCOLUMN = 0x02    ;//ssd1306 ma = 0x00   //w sh1106 przesuwamy o = 0x02 czyli setlowcolumn | = 0x02
	const uint8_t SH1106_SETHIGHCOLUMN = 0x10;
	const uint8_t SH1106_SETSTARTLINE = 0x40;
	const uint8_t SH1106_MEMORYMODE = 0x20;
	const uint8_t SH1106_COLUMNADDR = 0x21;
	const uint8_t SH1106_PAGEADDR   = 0xB0  ;  //ssd1306 ma = 0x22
	const uint8_t SH1106_COMSCANINC = 0xC0;
	const uint8_t SH1106_COMSCANDEC = 0xC8;
	const uint8_t SH1106_SEGREMAP = 0xA0;
	const uint8_t SH1106_CHARGEPUMP = 0xAD  ;  // ssd1306 ma 8D
	const uint8_t SH1106_EXTERNALVCC = 0x8B  ;  // ssd1306 ma = 0x1
	const uint8_t SH1106_SWITCHCAPVCC = 0x2;
	const uint8_t SH1106_ACTIVATE_SCROLL = 0x2F;
	const uint8_t SH1106_DEACTIVATE_SCROLL = 0x2E;
	const uint8_t SH1106_SET_VERTICAL_SCROLL_AREA = 0xA3;
	const uint8_t SH1106_RIGHT_HORIZONTAL_SCROLL = 0x26;
	const uint8_t SH1106_LEFT_HORIZONTAL_SCROLL = 0x27;
	const uint8_t SH1106_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29;
	const uint8_t SH1106_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A;
#pragma endregion Const
public:
	Sh1106(PinSBase &DataCommand,PinSBase &Reset,PinSBase& Mosi,PinSBase& ChipSelect, PinSBase& Clk, PinSBase &Miso, PinSBase & SS)
	{
		_DataCommand = &DataCommand;
		_Reset = &Reset;
		_Miso = &Miso;
		_Mosi = &Mosi;
		_ChipSelect = &ChipSelect;
		_SS = &SS;
		_Clk = &Clk;
	}

	void Enable()
	{
		_ChipSelect->Clear();
	}

	void Disable()
	{
		_ChipSelect->Set();
	}

	void operator << (char texts[])
	{
		PrintString(texts);
	}

	void operator << (uint8_t data)
	{
		SPDR = data;
		while(!(SPSR & (1<< SPIF)));
	}

	void SPI_Send(uint8_t data)
	{
		SPDR = data;
		while(!(SPSR & (1<< SPIF)));
	}

	void ChangeActiveLine(uint8_t line)
	{
		_DataCommand->Clear();
		this->operator<<(SH1106_SETLOWCOLUMN);
		this->operator<<(0xb0 + line);
		this->operator<<(SH1106_SETHIGHCOLUMN);
		_DataCommand->Set();
		SPI_Send(0);
	}
	
	void PrintChar(char toPrint)
	{
		_DataCommand->Set();
		for (int i = 0;i<5;i++)
		{
			if (toPrint == ' ')
				SPI_Send(0);
			else
				SPI_Send(eeprom_read_byte((unsigned char *)&font5x7[i+(toPrint-'(')*5]));
		}
		SPI_Send(0x00);
		_DataCommand->Clear();
	}

	void PrintString(char string[])
	{
		_DataCommand->Set();
		while (*(string) !=0)
		{	

			PrintChar(*(string));
			string++;
		}
		_DataCommand->Clear();
	}
	
	void ClearScreen()
	{
		for (char i = 0; i<8; i++)
		{
			_DataCommand->Clear();
			SPI_Send(SH1106_SETLOWCOLUMN);
			SPI_Send(0xb0 + i);
			SPI_Send(SH1106_SETHIGHCOLUMN);
			_DataCommand->Set();
			for (char j = 0; j < 128;j++)
			{
				SPI_Send(0x00);
			}
			_DataCommand->Clear();
		}
	}
	
	void Initialize()
	{
		_Reset->AsOutput();
		_DataCommand->AsOutput();
		_ChipSelect->AsOutput();
		_Miso->AsInput();
		_Mosi->AsOutput();
		_Clk->AsOutput();
		_SS->AsOutput();
		Enable();
		ResetWithDelay();

		SPI::InitializeAsMaster();
		SPI::SetDoubleSpeed();
		_DataCommand->Clear();

		SPI_Send (SH1106_DISPLAYOFF);
		SPI_Send (SH1106_SETMULTIPLEX);
		SPI_Send (0x3F);
		SPI_Send (SH1106_SETSTARTLINE);        /*set display start line*/
		SPI_Send (SH1106_PAGEADDR);   		 	/*set page address*/
		SPI_Send (SH1106_SETLOWCOLUMN|0x02);	/*set lower column address*/
		SPI_Send (SH1106_SETHIGHCOLUMN);    	/*set higher column address*/
		SPI_Send (SH1106_SEGREMAP|0x01); 		/*set segment remap*/
		SPI_Send (SH1106_NORMALDISPLAY);    	/*normal / reverse*/
		SPI_Send (SH1106_CHARGEPUMP);			/*set charge pump enable*/
		SPI_Send (SH1106_MEMORYMODE);
		SPI_Send (0x00);
		SPI_Send (SH1106_EXTERNALVCC);			/*external VCC   */
		SPI_Send (0x31);  						/*0X30---0X33  7,4V-8V-8,4V-9V */
		_delay_ms(10);
		SPI_Send (SH1106_COMSCANDEC);    		/*Com scan direction*/
		SPI_Send (SH1106_SETDISPLAYOFFSET);    /*set display offset*/
		SPI_Send (0x00);   						/*   0x20  /   0x00 */
		SPI_Send (SH1106_SETDISPLAYCLOCKDIV);  /*set osc division*/
		SPI_Send (REFRESH_MAX);
		SPI_Send (SH1106_SETPRECHARGE);    	/*set pre-charge period*/
		SPI_Send (0x1f);    					/*0x22  /  0x1f*/
		SPI_Send (SH1106_SETCOMPINS);    		/*set COM pins*/
		SPI_Send (0x12);
		SPI_Send (SH1106_SETVCOMDETECT);    	/*set vcomh*/
		SPI_Send (0x40);
		SPI_Send (SH1106_SETCONTRAST);
		SPI_Send (255);
		SPI_Send (SH1106_DISPLAYALLON_RESUME);
		SPI_Send (SH1106_NORMALDISPLAY);
		SPI_Send (SH1106_DISPLAYON);
		ClearScreen();
	}

	void ResetWithDelay()
	{
		_Reset->Set();
		_delay_ms(50);
		_Reset->Clear();
		_delay_ms(50);
		_Reset->Set();
	}
};
