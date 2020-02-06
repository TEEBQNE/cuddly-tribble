#include "cPointer.h"

void* cDummy::operator new[](size_t sz, void* buffer)
{
	return nullptr;
}

void cDummy::operator delete[](void* buffer, size_t sz)
{
}
