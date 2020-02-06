/*
Memory Management Playground (MMP)
By Daniel S. Buckstein
Copyright 2019-2020
*/
//-----------------------------------------------------------------------------

#ifndef _MMP_MEMORY_H_
#define _MMP_MEMORY_H_


#include "mmp_file.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus

#endif	// __cplusplus


//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// general memory utilities

// mmp_set
//	Set/format memory utility.
//		param 'block': pointer to block
//		param 'size_bytes': number of bytes to set
//		param 'value': value used to set all bytes
//		return 'SUCCESS': block
//		return 'FAILURE': null pointer (0)
addr mmp_set(addr const block, size const size_bytes, byte const value);

// mmp_set_zero
//	Set/format memory so that all bytes are zero.
//		param 'block': pointer to block
//		param 'size_bytes': number of bytes to set to zero
//		return 'SUCCESS': block
//		return 'FAILURE': null pointer (0)
addr mmp_set_zero(addr const block, size const size_bytes);

// mmp_copy
//	Copy memory utility.
//		param 'block_dst': pointer to destination block
//		param 'block_src': pointer to source block
//		param 'size_bytes': number of bytes to copy
//		return 'SUCCESS': block_dst
//		return 'FAILURE': null pointer (0)
addr mmp_copy(addr const block_dst, kaddr const block_src, size const size_bytes);

// mmp_compare
//	Compare memory utility.
//		param 'block_0': pointer to first block to compare
//		param 'block_1': pointer to second block to compare
//		param 'size_bytes': number of bytes to compare
//		return 'SUCCESS': number of bytes before a difference occurs
//		return 'FAILURE': zero
size mmp_compare(kaddr const block_0, kaddr const block_1, size const size_bytes);


//-----------------------------------------------------------------------------
// pool utilities

// mmp_pool_init
//	Initialize managed pool given pre-allocated (stack or heap) block of 
//	memory. Effectively turns any memory block into a managed block.
//		param 'block_base': base pointer of pre-allocated block
//		param 'block_base_size': size of pre-allocated block
//		param 'pool_size_bytes': size of new pool
//		return 'SUCCESS': pool block base pointer
//		return 'FAILURE': null pointer (0)
addr mmp_pool_init(addr const block_base, size const block_base_size, size const pool_size_bytes);

// mmp_pool_term
//	Terminate managed pool, leaving the contained memory unaffected.
//		param 'pool': pool pointer (assuming everything in it has been released)
//		return 'SUCCESS': size of pool released
//		return 'FAILURE': zero
size mmp_pool_term(addr const pool);


//-----------------------------------------------------------------------------
// block utilities

// mmp_block_reserve
//	Reserve a block within a pool.
//		param 'pool': pointer to pool from which to reserve memory
//		param 'block_size_bytes': number of bytes to reserve
//		return 'SUCCESS': reservation address to be used to store data
//		return 'FAILURE': null pointer (0)
addr mmp_block_reserve(addr const pool, size const block_size_bytes);

// mmp_block_release
//	Release a block back into a pool; does not reset data.
//		param 'block': pointer to reserved block to be released
//		param 'pool': pointer to pool in which block is reserved
//		return 'SUCCESS': size of block released
//		return 'FAILURE': zero
size mmp_block_release(addr const block, addr const pool);


//-----------------------------------------------------------------------------


#include "_inl/mmp_memory.inl"


#ifdef __cplusplus
}
#else	// !__cplusplus

#endif	// __cplusplus


#endif	// !_MMP_MEMORY_H_