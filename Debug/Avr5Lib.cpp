/*
 * Avr5Lib.cpp
 *Sandbox for testing 
 * Created: 28/01/2016 12:19:21
 *  Author: rogoz
 c++11
 */ 
 
 //Tons of static code beacuse avr5 are slow, and it must be full compilator optimalizable
//#ifndef _AVR_IO_H_
	//#error "Include <avr/io.h> before this file."
//#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>

  #define CLEAR(toClear, val) toClear &= ~(uint8_t)(val)
  #define SET(toSet,val) toSet |= (uint8_t)val

namespace CodeEasyAvr
{
	//8bit Timer/Counter0 with PWM
	class SPI
	{
	public:
		void static AsMaster()
		{
			SET(SPCR,MSTR);
		}

		void static AsSlave()
		{
			CLEAR(SPCR,MSTR);
		}

		void static EnableInterrupt()
		{
			SET(SPCR,SPIE);
		}

		void static DisableInterrupt()
		{
			CLEAR(SPCR,(1<<SPIE));
		}

		void static SPIEnable()
		{
			SET(SPCR,(1<<SPE));
		}

		void static SPIDisable()
		{
			CLEAR(SPCR,(1<<SPE));
		}

		enum class SPIDataOrder
		{
			LSB_First = (1<<DORD),
			MSB_First = 0,
		};

		void static SetDataOrder(SPIDataOrder dataOrder)
		{
			CLEAR(SPCR,(uint8_t)dataOrder);
					SET(SPCR,(uint8_t)dataOrder);
		}

		enum class ClockPolarity
		{
			LeadingEdgeRising_TrailingEdgeFalling = 0,
			LeadingEdgeFalling_TrailingEdgeRising = (1<<CPOL),
		};
		
		void static SetClockPolarity(ClockPolarity polarity)
		{
			CLEAR(SPCR,(1<<CPOL));
			SET(SPCR,(uint8_t) polarity);
		}

		enum class ClockPhase
		{
			LeadingEdgeSample_TrailingEdgeSetup = 0,
			LeadingEdgeSetup_TrailingEdgeSample= (1<<CPHA),
			BitSet = LeadingEdgeSetup_TrailingEdgeSample
		};

		void static SetClockPhase(ClockPhase phase)
		{
			CLEAR(SPCR,ClockPhase::BitSet);
			SET(SPCR,(uint8_t)phase);
		}

		void static SpiMode3()
		{
			SetClockPhase(ClockPhase::LeadingEdgeSetup_TrailingEdgeSample);
			SetClockPolarity(ClockPolarity::LeadingEdgeFalling_TrailingEdgeRising);	
		};

		enum class Prescalers
		{
			Prescaler_4=0,
			Prescaler_16 = (1<<SPR0),
			Prescaler_64 = (1<<SPR1),
			Prescaler_128 = (1<<SPR1) || (1<<SPR0),
			AllBitsSet = Prescaler_128
		};

		void static SetPrescaler(Prescalers prescaler)
		{
			CLEAR(SPCR,(uint8_t)Prescalers::AllBitsSet);
			SET(SPCR,(uint8_t)prescaler);
		}

		void static SetDoubleSpeed()
		{
			SET(SPSR,SPI2X);
		}

		void static ClearDoubleSpeed()
		{
			CLEAR(SPSR,SPI2X);
		}

		void static SetData(uint8_t dataFrame)
		{
			SPDR = dataFrame;
		}


		uint8_t static CheckData()
		{
			return SPDR;
		}
			
		bool static CheckOperationCompleteFlag()
		{
			return ((SPSR >> SPIF) & 1); 
		}
	};

	class Timer0
	{
	public:
		void static SetCompareValue(uint8_t compareRegisterValue)
		{
			OCR0 = compareRegisterValue;
		}
			
		static uint8_t GetCounterValue()
		{
			return TCNT0;
		}

		enum InterruptMask
		{
			Disabled = 0,
			CompareMatchInterrupt = (1<<OCIE0),
			OverFlowInterrupt = (1<<TOIE0),
			CompareAndOverFlowInterrupts = (OverFlowInterrupt||OverFlowInterrupt),
			AllBitsSet = CompareAndOverFlowInterrupts
		};

		volatile void static SetInterrupts(InterruptMask interrupt)
		{
			CLEAR(TIMSK,(TOIE0,OCIE0));
			TIMSK |= interrupt;
		}

		enum class Prescalers
		{
			Stop = 0,
			Prescaler_1 = (1<<CS00),
			Prescaler_8 = (1<<CS01),
			Prescaler_64 = (1<<CS01) || (1<<CS00),
			Prescaler_256 = (1<<CS02),
			Prescaler_1024 = (1<<CS02)||(1<<CS00),
			External_Falling = (1<<CS02) || (1<<CS01),
			External_Rising = (1<<CS02) || (1<<CS01) || (1<<CS00),
			AllBitsSet = External_Rising
		};

		enum class WaveForms
		{
			Normal = 0,
			PWM_PhaseCorrect = (1<<WGM00),
			CTC = (1<<WGM01),
			PWM_Fast = ((1<<WGM00)||(1<<WGM01))
		};

		enum class OutputCounterMode
		{
			Disconnected = 0,
			Toggle_Or_Reserved = (1<<COM00),
			Clear = (1<<COM01),
			Set = (1<<COM00)||(1<<COM01),
		};

		/////Reset prescaller timer 1 and 0
		static void ResetPrescaler()
		{
			SET(SFIOR,(1<<PSR10));
		}

		static void SetPrescaler(Prescalers prescaler)
		{
			CLEAR(TCCR0,(uint8_t)Prescalers::AllBitsSet);
			SET(TCCR0,(uint8_t)prescaler);
		}

		static void WaveFormMode(WaveForms waveForms)
		{
			CLEAR(TCCR0,(1<<WGM01)||(1<<WGM00));
			SET(TCCR0,(uint8_t)waveForms);
		}

		static void CompareMatchOutput(OutputCounterMode outputCounterMode)
		{
			CLEAR(TCCR0,(1<<COM01)||(1<<COM00));
			SET(TCCR0,(uint8_t)outputCounterMode);
		};
	}; 
 }