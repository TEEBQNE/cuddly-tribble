/*
Memory Management Playground (MMP)
By Daniel S. Buckstein
Copyright 2019-2020
*/
//-----------------------------------------------------------------------------

#include "mmp/mmp_memory.h"
//#include <stdio.h>


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


	return testMMP(argc, argv);
	//return testMalloc(argc, argv);
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

	// create a structure given the chunk starting position, defined malloc block size
	// and size of entire pool itself

	// as for now I have no reason to break up the chunk into multiple pools, just giving
	// one pool all of the memory
	addr malloc_Pool = mmp_pool_init(chunk_base, 4, count);

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


#pragma region  mallocDebuggTesting

	//for (int x = 4; x < 65536; x += 4)
	//{
	//	malloctest* test1 = malloc(x);
	//
	//	size_t test = (size_t)test1;
	//
	//	free(test1);
	//
	//	malloctest* test2 = malloc(x);
	//	size_t test0 = (size_t)test2;
	//
	//	if (test0 != test)
	//	{
	//		printf("This number does not have same address: %d", x); printf("\n");
	//	}
	//	//if
	//	//	printf("This number does not have same address: %d", x);
	//}

	//malloctest* test1024 = malloc(32);
	//free(test1024);
	//
	//malloctest* test1025 = malloc(32);

	//free(test1025);

	//void* test = malloc(1024);
	//
	//int* list[256];
	//
	//for (int i = 50; i < 306; i++)
	//{
	//	list[i] = (int*)(test)+i;
	//}
	//
	//free(test);
#pragma endregion
#pragma endregion
	// done
	return 0;
}
