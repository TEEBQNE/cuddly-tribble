/*
Memory Management Playground (MMP)
By Daniel S. Buckstein
Copyright 2019-2020
*/
//-----------------------------------------------------------------------------

#ifdef	_MMP_MEMORY_H_
#ifndef _MMP_MEMORY_INL_
#define _MMP_MEMORY_INL_


//-----------------------------------------------------------------------------

static inline addr mmp_set_zero(addr const block, size const size_bytes)
{
	return mmp_set(block, size_bytes, 0);
}


//-----------------------------------------------------------------------------


#endif	// !_MMP_MEMORY_INL_
#endif	// _MMP_MEMORY_H_