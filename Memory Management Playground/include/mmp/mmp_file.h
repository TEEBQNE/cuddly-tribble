/*
Memory Management Playground (MMP)
By Daniel S. Buckstein
Copyright 2019-2020
*/
//-----------------------------------------------------------------------------

#ifndef _MMP_FILE_H_
#define _MMP_FILE_H_


#include "mmp_types.h"


#ifdef __cplusplus
extern "C"
{
#else	// !__cplusplus
typedef		struct mmp_file			mmp_file;
#endif	// __cplusplus


//-----------------------------------------------------------------------------

struct mmp_file
{
	// stream pointer
	ptr stream;

	// total bytes in the file
	size bytes_len;

	// total bytes read/written
	size bytes_rw;
	
	// number of bytes read/written in the last r/w operation
	size bytes_rw_prev;

	// read/write mode
	byte mode_rw[szdword];
};


// format of custom data read function
typedef size(*mmp_file_read_func)(ptr const data, size const size_bytes, mmp_file const* const file);

// format of custom data write function
typedef size(*mmp_file_write_func)(kptr const data, size const size_bytes, mmp_file const* const file);


//-----------------------------------------------------------------------------

// mmp_file_open_read
//	Open a file for reading.
//		param 'file': pointer to file descriptor
//		param 'filepath': path (relative or absolute) to file to open
//		return 'SUCCESS': file length in bytes
//		return 'FAILURE': zero
size mmp_file_open_read(mmp_file* const file, cstrp const filepath);

// mmp_file_open_write
//	Open a file for writing.
//		param 'file': pointer to file descriptor
//		param 'filepath': path (relative or absolute) to file to open
//		return 'SUCCESS': file length in bytes
//		return 'FAILURE': zero
size mmp_file_open_write(mmp_file* const file, cstrp const filepath);

// mmp_file_read
//	Read data from a file using a custom read function.
//		param 'file': pointer to file descriptor
//		param 'data_dst': pointer to data to read from file
//		param 'size_bytes_max': maximum number of bytes to read
//		param 'func_read': pointer to custom read function
//			(passing null pointer (0) defaults to 'mmp_file_util_read_quick')
//		return 'SUCCESS': length in bytes of data copied from file
//		return 'FAILURE': zero
size mmp_file_read(mmp_file* const file, ptr const data_dst, size size_bytes_max, mmp_file_read_func const func_read);

// mmp_file_write
//	Write data to a file using a custom write function.
//		param 'file': pointer to file descriptor
//		param 'data_src': pointer to data to write to file
//		param 'size_bytes_max': maximum number of bytes to write
//		param 'func_write': pointer to custom write function
//			(passing null pointer (0) defaults to 'mmp_file_util_write_quick')
//		return 'SUCCESS': length in bytes of data copied from file
//		return 'FAILURE': zero
size mmp_file_write(mmp_file* const file, kptr const data_src, size size_bytes_max, mmp_file_write_func const func_write);

// mmp_file_close
//	Close a file.
//		param 'file': pointer to file descriptor
//		return 'SUCCESS': length of file closed in bytes
//		return 'FAILURE': zero
size mmp_file_close(mmp_file* const file);

// mmp_file_is_open
//	Check if a file is open for streaming.
//		param 'file': pointer to file descriptor
//		return 'SUCCESS': positive if open, zero if closed
//		return 'FAILURE': negative
flag mmp_file_is_open(mmp_file const* const file);

// mmp_file_is_reading
//	Check if a file is open for reading.
//		param 'file': pointer to file descriptor
//		return 'SUCCESS': positive if reading, zero if closed or writing
//		return 'FAILURE': negative
flag mmp_file_is_reading(mmp_file const* const file);

// mmp_file_is_writing
//	Check if a file is open for writing.
//		param 'file': pointer to file descriptor
//		return 'SUCCESS': positive if writing, zero if closed or reading
//		return 'FAILURE': negative
flag mmp_file_is_writing(mmp_file const* const file);


//-----------------------------------------------------------------------------

// mmp_file_util_read_quick
//	Wrapper for binary read function.
//		param 'data_dst': pointer to data to read from file
//		param 'size_bytes': number of bytes to read
//		param 'file': pointer to file descriptor
//		return 'SUCCESS': size_bytes
//		return 'FAILURE': zero
size mmp_file_util_read_quick(ptr const data_dst, size const size_bytes, mmp_file const* const file);

// mmp_file_util_write_quick
//	Wrapper for binary write function.
//		param 'data_dst': pointer to data to write to file
//		param 'size_bytes': number of bytes to write
//		param 'file': pointer to file descriptor
//		return 'SUCCESS': number of bytes written
//		return 'FAILURE': zero
size mmp_file_util_write_quick(kptr const data_src, size const size_bytes, mmp_file const* const file);

// mmp_file_util_mkdir
//	Create a directory.
//		param 'dirpath': path (relative or absolute) to directory to create
//		return 'SUCCESS': one
//		return 'FAILURE': zero
size mmp_file_util_mkdir(cstrp const dirpath);


//-----------------------------------------------------------------------------


#include "_inl/mmp_file.inl"


#ifdef __cplusplus
}
#else	// !__cplusplus

#endif	// __cplusplus


#endif	// !_MMP_FILE_H_