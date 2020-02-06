#include "..\MemoryManagementPlayground\cPointer.h"
#include "..\MemoryManagementPlayground\cPointer.h"
#include "..\MemoryManagementPlayground\cPointer.h"

// default ctr
cDummy::cDummy()
{
	data = 0;
	size = 0;
}

// copy ctr
cDummy::cDummy(cDummy const& rhs)
	:cDummy(rhs.size)
{
	// copy data in actual array
	if (data)
	{

	}
}

// move ctr
cDummy::cDummy(cDummy&& rhs)
{
	size = rhs.size;
	data = rhs.data;
	rhs.data = 0;
}

// dtr
cDummy::~cDummy()
{
	// make sure data exists before attempting to delete it
	//if (data)
	//{
	// check is redundent, delete in c++ has a null check
		delete[] data;
	//}
}

/*
// copy assign
cDummy& cDummy::operator=(cDummy const& rhs)
{
	if (data)
	{
		size = 0;
		delete[] data;
	}

	if (rhs.data)
	{
		size = rhs.size;
		data = new int[size];
	}
		
	return *this;
}

// move assign
cDummy& cDummy::operator=(cDummy&& rhs)
{
	if (data)
	{
		size = 0;
		delete[] data;
	}

	size = rhs.size;
	data = rhs.data;
	rhs.data = 0;

	return *this;
}
*/
// other copy ctr
cDummy::cDummy(size_t newSize)
{
	if (newSize > 0)
	{
		size = newSize;
		data = new blah[size];
	}
	else
	{
		size = 0;
		data = 0;
	}
}

void* cDummy::operator new(size_t sz)
{
	return 0;
}

void cDummy::operator delete(void* ptr)
{

}

void* cDummy::operator new(size_t sz, void* buffer)
{
	return 0;
}

void cDummy::operator delete(void* buffer, void* ptr)
{

}

void* cDummy::operator new[](size_t sz)
{
	return nullptr;
}

void cDummy::operator delete[](void* ptr)
{
}
