/*
Memory Management Playground (MMP)
By Daniel S. Buckstein
Copyright 2019-2020
*/
//-----------------------------------------------------------------------------

#include "mmp/mmp_memory.h"

#include "mmp/mmp_file.h"
#include <stdio.h>


//-----------------------------------------------------------------------------

// structure for a linked list
typedef struct linkedList_tag {
	// pointing to the next block (null if it is pointing to no memory)
	struct linkedList_tag* next;

	// pointing to the previous block (null if it the first block)
	struct linkedList_tag* previous;
} linkedList;

// structure to hold data similar to what malloc holds
// user should be able to access address and size (Not prev/next)
// structued similarly to how we were shown to create a container in class
// i.e union (Important as unions store multiple data types in same memory location
// all of mallocs data appears in the same location right before the allocated block
typedef union malloctest_tag
{
	// actual pointer to start of block in memory
	addr blockAddress;

	// stored linkedList structure of next/previous block of memory
	linkedList node;

	// size of this current block
	size_t sizeOfBlock;

	// determines if this block is in use or not
	__vcrt_bool isInuse;
}malloctest;

//-----------------------------------------------------------------------------
// internal utilities

static inline cstrp mmp_get_cstr_end(cstrp cstr)
{
	uindex i = 0;
	while (*cstr != 0 && i++ < szcstr)
		++cstr;
	return cstr;
}


static inline byte* mmp_set_byte(byte* block, byte const* const end, byte const value)
{
	while (block < end)
		*(block++) = value;
	return block;
}

static inline dword* mmp_set_dword(dword* block, dword const* const end, dword const value)
{
	while (block < end)
		*(block++) = value;
	return block;
}

static inline cstrp mmp_set_cstr(cstr cstr, dword const value)
{
	return (cstrp)mmp_set_dword((dword*)(cstr), (dword*)(cstr + szcstr), value);
}


static inline byte* mmp_copy_byte(byte* block_dst, byte const* block_src, byte const* const end_dst)
{
	while (block_dst < end_dst)
		*(block_dst++) = *(block_src++);
	return block_dst;
}

static inline dword* mmp_copy_dword(dword* block_dst, dword const* block_src, dword const* const end_dst)
{
	while (block_dst < end_dst)
		*(block_dst++) = *(block_src++);
	return block_dst;
}

static inline cstrp mmp_copy_cstr(cstr cstr_dst, cstrp const cstr_src)
{
	return (cstrp)mmp_copy_dword((dword*)(cstr_dst), (dword*)(cstr_src), (dword*)(cstr_dst + szcstr));
}


static inline byte const* mmp_compare_byte(byte const* block_0, byte const* block_1, byte const* const end_0)
{
	while (block_0 < block_0)
		if (*(block_0) == *(block_1++))
			++block_0;
		else
			break;
	return block_0;
}

static inline dword const* mmp_compare_dword(dword const* block_0, dword const* block_1, dword const* const end_0)
{
	while (block_0 < end_0)
		if (*(block_0) == *(block_1++))
			++block_0;
		else
			break;
	return block_0;
}

static inline cstrp mmp_compare_cstr(cstrp const cstr_0, cstrp const cstr_1)
{
	return (cstrp)mmp_compare_dword((dword*)(cstr_0), (dword*)(cstr_1), (dword*)(cstr_0 + szcstr));
}


//-----------------------------------------------------------------------------
// general memory utilities

addr mmp_set(addr const block, size const size_bytes, byte const value)
{
	if (block && size_bytes > 0)
	{
		byte const value2dword[szdword] = { value, value, value, value };
		dword const value_as_dword = *((dword*)value2dword);
		dword* const base = (dword*)block;
		byte const* const basebp = (byte*)block;
		size const dword_ct = size_bytes / szdword;

		// write integers until the last integer can fit
		dword const* itr = mmp_set_dword(base, (dword_ct + base), value_as_dword);

		// write the remaining bytes
		byte const* itrbp = mmp_set_byte((byte*)itr, (size_bytes + basebp), value);

		// done
		return block;
	}
	return 0;
}


addr mmp_copy(addr const block_dst, kaddr const block_src, size const size_bytes)
{
	if (block_dst && block_src && block_dst != block_src && size_bytes > 0)
	{
		dword* const base_dst = (dword*)block_dst;
		dword const* const base_src = (dword*)block_src;
		byte const* const basebp_dst = (byte*)block_dst;
		size const dword_ct = size_bytes / szdword;
		
		// copy and write integers until the last integer can fit
		dword* itr_dst = mmp_copy_dword(base_dst, base_src, (dword_ct + base_dst));

		// copy and write the remaining bytes
		byte* itrbp_dst = mmp_copy_byte((byte*)itr_dst, ((byte const*)(dword_ct + base_src)), (size_bytes + basebp_dst));
	
		// done
		return block_dst;
	}
	return 0;
}


size mmp_compare(kaddr const block_0, kaddr const block_1, size const size_bytes)
{
	if (block_0 && block_1 && block_0 != block_1 && size_bytes > 0)
	{
		dword const* const base_0 = (dword*)block_0;
		dword const* const base_1 = (dword*)block_1;
		byte const* const basebp_0 = (byte*)block_0;
		size dword_ct = size_bytes / szdword;

		// compare integers until the last integer can fit
		dword const* itr_0 = mmp_compare_dword(base_0, base_1, (dword_ct + base_0));

		// compare the remaining bytes
		byte const* itrbp_0 = mmp_compare_byte((byte*)itr_0, ((byte const*)(itr_0 - base_0 + base_1)), (size_bytes + basebp_0));

		// done
		return (itrbp_0 - basebp_0);
	}
	return 0;
}


//-----------------------------------------------------------------------------
// pool utilities

/// creates a list when given a new chunk to use as a pool
void createList(linkedList *listHead)
{
	listHead->previous = NULL;
	listHead->next = NULL;
}

void addToList(struct linkedList* listHead, struct linkedList* prev, struct linkedList* next)
{

}

addr mmp_pool_init(addr const block_base, size const block_base_size, size const pool_size_bytes)
{
	if (block_base && block_base_size && pool_size_bytes)
	{
		// create a linked list to point at the base of the block
		// give it the total size of the given block (pool)
		// then specify the min size (block_base_size)
		malloctest *initPool = (void *)block_base;

		// represent accurate memory in space by substracting all data the structure keeps up
		initPool->sizeOfBlock = pool_size_bytes - sizeof(malloctest);
		initPool->isInuse = 0;

		createList(&initPool->node);

		return initPool;
	}
	return 0;
}


size mmp_pool_term(addr const pool)
{
	if (pool)
	{
		// free contained memory completely
	}
	return 0;
}


//-----------------------------------------------------------------------------
// block utilities

addr mmp_block_reserve(addr const pool, size const block_size_bytes)
{
	if (pool && block_size_bytes)
	{
		// determine if what is being allocated is under the size of one block, if it is and there is memory
		// that is free, allocate it, if not then tell the user

		// cast to head of the block, iterator pointer and new allocation
		malloctest* listHead, *temp, *newMalloc = NULL;
		
		listHead = temp = (malloctest*)pool;

		ui32 actualSize = block_size_bytes + sizeof(malloctest);

		// find next opening position in pool if there is one
		// or until a position is found
		do
		{
			if (!temp->isInuse && temp->sizeOfBlock >= actualSize)
			{
				if(temp->sizeOfBlock != actualSize)
				{
					// split up the block and create a new block
				}
				else
				{
					// is equal, take up the whole block
				}
			}
			else
			{
				temp = (malloctest*)temp->node.next;
			}
		} while (newMalloc == NULL && temp != NULL);

		if (newMalloc != NULL)
			return newMalloc->blockAddress;
		else
			return 0;
	}
	return 0;
}


size mmp_block_release(addr const block, addr const pool)
{
	if (block && pool)
	{
		// add the freed memory back to the pool that can be used
	}
	return 0;
}


//-----------------------------------------------------------------------------
