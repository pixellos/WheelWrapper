/*AUTO GENERATED CODE BY MATEUSZ POPIELARZ pixdevlife.azurewebsites.net
            Project Easy::CodeGenerator
            */
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

 #define  __FAST__PIN__OPERATIONS__ENABLE
 #ifndef __OPTIMIZE__
 #pragma message"Compiler optimizations disabled; functions from CodeLessAvr -> FastSet, FastClear wont work as designed! At -o3 OOP have only a bit program speed overhead"
#endif

class PinSBase{
public:
	virtual void FastSet(){}
	virtual void FastClear(){}
	virtual void FastToggle(){}
	virtual void FastAsOutput(){}
	virtual void FastAsInput(){}
	virtual void BoolSet(bool value){}
	virtual bool Check(){}
	virtual void Set(){}
	virtual void Clear(){}
	virtual void Toggle(){}
	virtual void AsOutput(){}
	virtual void AsInput(){}
	virtual ~PinSBase(){}
};

 template<const volatile uint8_t PortAddress,const volatile uint8_t PinAddress,const volatile uint8_t DdrAddress,const volatile uint8_t NOPin>
 class PinS : public PinSBase
{
public:
void FastSet()
	{
        (*(uint8_t*)PortAddress) |= (1 << NOPin); //Result is single SBI() in asm when only 1 set, at other case compiler will make it 3x1 cycles asm instrucions
    }

    void FastClear()
    {
        (*(uint8_t*)PortAddress) &= ~(1 << NOPin);
    }

    void FastToggle()
    {
        *(uint8_t*)PortAddress ^= (1 << NOPin);
    }

    void FastAsOutput()
    {
        *(uint8_t*)DdrAddress |= (1 << NOPin);
    }

    void FastAsInput()
    {
        *(uint8_t*)DdrAddress &= ~(1 << NOPin);
    }
	void BoolSet(bool value)
	{	
		if (value)
			Set();
		else
			Clear();
	}

    void Set()
    {
        (*(uint8_t volatile*)PortAddress) |= (1 << NOPin); //Result is single SBI() in asm (2 cycles)
    }

    void Clear()
    {
        (*(uint8_t volatile*)PortAddress) &= ~(1 << NOPin);
    }

    void Toggle()
    {
        (*(uint8_t volatile*)PortAddress) ^= (1 << NOPin);
    }

    bool Check()
    {
        return ((*(uint8_t volatile*)PinAddress >> NOPin) &1);
    }

    void AsOutput()
    {
        (*(uint8_t volatile *)DdrAddress) |= (1 << NOPin);
    }

    void AsInput()
    {
        (*(uint8_t volatile*)DdrAddress) &= ~(1 << NOPin);
    }
};



 