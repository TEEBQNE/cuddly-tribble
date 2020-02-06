/*
Memory Management Playground (MMP)
By Daniel S. Buckstein
Copyright 2019-2020
*/
//-----------------------------------------------------------------------------

#include "mmp/mmp_memory.h"

#pragma comment(lib, "MemoryManagementPlayground.lib")


//-----------------------------------------------------------------------------

typedef		byte				chunk_kb[1024];


//-----------------------------------------------------------------------------

#define		decl_argc			ui32 const argc
#define		decl_argv			cstrp const argv[]
typedef		i32(*entry_func)(decl_argc, decl_argv);


//-----------------------------------------------------------------------------

int testMMP(decl_argc, decl_argv);
int testMalloc(decl_argc, decl_argv);


//-----------------------------------------------------------------------------

int main(decl_argc, decl_argv)
{
	//return testMMP(argc, argv);
	return testMalloc(argc, argv);
}


//-----------------------------------------------------------------------------

int testMMP(decl_argc, decl_argv)
{
	// stack-allocate a bunch of data

	// chunk is max memory
	// split chunk into pools
	// split pools into blocks
	// manage the pool by max memory and manage the blocks for malloc implementation
	chunk_kb chunk[12];
	size count = sizeof(chunk);
	ptr chunk_base = mmp_set_zero(chunk, count);

	

	// done, stack-allocated data popped
	return 0;
}


//-----------------------------------------------------------------------------

#include <stdlib.h>

// add nothing to header files - only to source (Malloc implementation is only for you, not for anyone else)
int testMalloc(decl_argc, decl_argv)
{
#pragma region PointerExamplesReading

	// some memory address - don't know the type
	/*void* p;

	// some memory address - know the type, so know the size of memory allocated
	int* pi;

	// if written like this reading right to left, it describes exactly what the variable is

	// kpi is a pointer to a constant integer

	// is a pointer to a constant integer
	int const* kpi;								// same as int const kpi[];

	// pki is a constant pointer to an integer

	// is a constant pointer to an integer
	int* const pki;								// int pki[] (this is not right Dan forgot how to write it)

	// kpki is a constant pointer to a constant integer
	int const* const kpki;

	// kpkpkpki const pointer to a const pointer to a const pointer to a const integer
	// each pointer points to another memory address as a value until it points to the final value of an integer as a constant
	int const* const* const* const kpkpkpi;			// can be rewritten as 3D array (int const kpkpkpi[][][])

	// constant pointer to a constant pointer to a pointer to a constant integer
	int const** const* const kpkpkpi;


	int**** ppppi;									// can be rewritten as a 4D array

	*/

#pragma endregion

	
#pragma region MallocFun

	// implement the data structure as if it was the real malloc
	// in mmp_memory.c create your own data structrures the ones that are created here
	// just use what is pertinent and use your own
	// pointers are relevant as they will tell you which data structures malloc uses to allocate memory
	// figure out what memory allocation malloc uses
	typedef union malloctest_tag
	{
		i32 data[32];

		ptr pdata[32];
		struct 
		{
			i32 dummy;
		};
	}malloctest;

	typedef union other_test
	{
		int test[5];
	}otherTest;

	malloctest* test1024 = malloc(4);
	malloctest* test2048 = malloc(4);
	malloctest* test4096 = malloc(4);
	malloctest* test8192 = malloc(4);
	malloctest* test16 = malloc(4);


	

	int a[5] = { 1, 2, 3, 4, 5 };

	int* p = a;

	//testTest.test[0] = 5;


	free(test1024);

	test1024 = malloc(8192);



	free(test4096);
	free(test1024);
	free(test2048);
	free(test8192);
#pragma endregion
	// done
	return 0;
}
