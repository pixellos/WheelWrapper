/* 
* CppHelpers.h
* From AVRFREAKS forum 
* Created: 30/01/2016 20:07:52
* Author: rogoz
*/
#ifndef __CPPHELPERS_H__
#define __CPPHELPERS_H__
void * operator new(size_t size)
{
	return malloc(size);
}

void operator delete(void * ptr)
{
	free(ptr);
}
#endif //__CPPHELPERS_H__
