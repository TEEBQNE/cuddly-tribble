/*
Memory Management Playground (MMP)
By Daniel S. Buckstein
Copyright 2019-2020
*/
//-----------------------------------------------------------------------------

#include "mmp/mmp_file.h"

#include <stdio.h>


//-----------------------------------------------------------------------------

static inline size mmp_file_open(mmp_file* const file, cstrp const filepath, byte const mode[szdword])
{
	if (file && !file->stream && filepath && *filepath)
	{
		size result = 0;
		FILE* fp = fopen(filepath, mode);
		if (fp)
		{
			fseek(fp, 0, SEEK_END);
			file->bytes_len = (size)ftell(fp);
			if (file->bytes_len)
			{
				*((dword*)file->mode_rw) = *((dword*)mode);
				file->stream = fp;
				file->bytes_rw = 0;
				file->bytes_rw_prev = 0;
				return file->bytes_len;
			}
			fclose(fp);
		}
	}
	return 0;
}


//-----------------------------------------------------------------------------

size mmp_file_open_read(mmp_file* const file, cstrp const filepath)
{
	return mmp_file_open(file, filepath, "rb");
}


size mmp_file_open_write(mmp_file* const file, cstrp const filepath)
{
	return mmp_file_open(file, filepath, "wb");
}


size mmp_file_read(mmp_file* const file, ptr const data_dst, size size_bytes_max, mmp_file_read_func const func_read)
{
	if (mmp_file_is_reading(file) > 0 && data_dst)
	{
		mmp_file_read_func const read = func_read ? func_read : mmp_file_util_read_quick;
		size const result = read(data_dst, size_bytes_max, file);
		file->bytes_rw_prev = result;
		file->bytes_rw += result;
		return result;
	}
	return 0;
}


size mmp_file_write(mmp_file* const file, kptr const data_src, size size_bytes_max, mmp_file_write_func const func_write)
{
	if (mmp_file_is_writing(file) > 0 && data_src)
	{
		mmp_file_write_func const write = func_write ? func_write : mmp_file_util_write_quick;
		size const result = write(data_src, size_bytes_max, file);
		file->bytes_rw_prev = result;
		file->bytes_rw += result;
		return result;
	}
	return 0;
}


size mmp_file_close(mmp_file* const file)
{
	if (mmp_file_is_open(file) > 0)
	{
		const size result = file->bytes_len;
		*((dword*)file->mode_rw) = 0;
		fclose((FILE*)file->stream);
		file->stream = 0;
		return result;
	}
	return 0;
}


//-----------------------------------------------------------------------------

size mmp_file_util_read_quick(ptr const data_dst, size const size_bytes, mmp_file const* const file)
{
	return fread(data_dst, size_bytes, 1, (FILE*)file->stream);
}

size mmp_file_util_write_quick(kptr const data_src, size const size_bytes, mmp_file const* const file)
{
	return fwrite(data_src, size_bytes, 1, (FILE*)file->stream);
}


#ifdef _WIN32
#include <direct.h>
#else	// !_WIN32
#include <sys/stat.h>
#endif	// _WIN32

size mmp_file_util_mkdir(cstrp const dirpath)
{
	if (dirpath && *dirpath)
	{
		size const ret =
#ifdef _WIN32
			_mkdir(dirpath);
#else	// !_WIN32
			mkdir(dirpath, 0700);
#endif	// _WIN32
		return (ret == 0);
	}
	return 0;
}


//-----------------------------------------------------------------------------
