/*
Memory Management Playground (MMP)
By Daniel S. Buckstein
Copyright 2019-2020
*/
//-----------------------------------------------------------------------------

#ifdef	_MMP_FILE_H_
#ifndef _MMP_FILE_INL_
#define _MMP_FILE_INL_


//-----------------------------------------------------------------------------

static inline flag mmp_file_is_open(mmp_file const* const file)
{
	if (file)
	{
		flag const open = (file->stream != 0);
		return open;
	}
	return -1;
}


static inline flag mmp_file_is_reading(mmp_file const* const file)
{
	if (file)
	{
		flag const reading = (file->stream != 0 && *file->mode_rw == 'r');
		return reading;
	}
	return -1;
}


static inline flag mmp_file_is_writing(mmp_file const* const file)
{
	if (file)
	{
		flag const writing = (file->stream != 0 && *file->mode_rw == 'w');
		return writing;
	}
	return -1;
}


//-----------------------------------------------------------------------------


#endif	// !_MMP_FILE_INL_
#endif	// _MMP_FILE_H_