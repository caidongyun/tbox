/*!The Treasure Box Library
 * 
 * TBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * TBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with TBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2009 - 2011, ruki All rights reserved.
 *
 * \author		ruki
 * \file		file.c
 *
 */

/* /////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../file.h"
#include "../event.h"
#include "../../math/math.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <stdio.h>
#ifndef TB_CONFIG_OS_ANDROID
# 	include <sys/unistd.h>
#endif

/* /////////////////////////////////////////////////////////
 * implemention
 */

// file
tb_handle_t tb_file_init(tb_char_t const* path, tb_size_t flags)
{
	tb_assert_and_check_return_val(path, TB_NULL);

	// flag
	tb_size_t flag = 0;
	if (flags & TB_FILE_RO) flag |= O_RDONLY;
	else if (flags & TB_FILE_WO) flag |= O_WRONLY;
	else if (flags & TB_FILE_RW) flag |= O_RDWR;

	if (flags & TB_FILE_CREAT) flag |= O_CREAT;
	if (flags & TB_FILE_APPEND) flag |= O_APPEND;
	if (flags & TB_FILE_TRUNC) flag |= O_TRUNC;

	// mode
	tb_size_t mode = 0;
	if (flags & TB_FILE_CREAT) 
	{
		//if ((flags & TB_FILE_RO) | (flags & TB_FILE_RW)) mode |= S_IREAD;
		//if ((flags & TB_FILE_WO) | (flags & TB_FILE_RW)) mode |= S_IWRITE;
		mode = 0777;
	}

	// open it
	tb_long_t fd = open(path, flag, mode);

	// ok?
	return (fd < 0)? TB_NULL : ((tb_handle_t)(fd + 1));
}
tb_void_t tb_file_exit(tb_handle_t hfile)
{
	if (hfile) close((tb_long_t)hfile - 1);
}
tb_long_t tb_file_read(tb_handle_t hfile, tb_byte_t* data, tb_size_t size)
{
	tb_assert_and_check_return_val(hfile, -1);
	return read((tb_long_t)hfile - 1, data, size);
}
tb_long_t tb_file_writ(tb_handle_t hfile, tb_byte_t const* data, tb_size_t size)
{
	tb_assert_and_check_return_val(hfile, -1);
	return write((tb_long_t)hfile - 1, data, size);
}
tb_void_t tb_file_sync(tb_handle_t hfile)
{
#ifdef TB_CONFIG_OS_LINUX
	if (hfile) fdatasync((tb_long_t)hfile - 1);
#else
	if (hfile) fsync((tb_long_t)hfile - 1);
#endif
}
tb_int64_t tb_file_seek(tb_handle_t hfile, tb_int64_t offset, tb_size_t flags)
{
	tb_assert_and_check_return_val(hfile, -1);

	switch (flags)
	{
	case TB_FILE_SEEK_BEG:
		offset = lseek((tb_long_t)hfile - 1, offset, SEEK_SET);
	case TB_FILE_SEEK_CUR:
		offset = lseek((tb_long_t)hfile - 1, offset, SEEK_CUR);
	case TB_FILE_SEEK_END:
		offset = lseek((tb_long_t)hfile - 1, offset, SEEK_END);
	default:
		tb_trace("unknown file seek flag: %d", flags);
		break;
	}

	return offset;
}
tb_long_t tb_file_wait(tb_handle_t hfile, tb_size_t etype, tb_long_t timeout)
{
	tb_assert_and_check_return_val(hfile, 0);

	// init
	struct pollfd pfd = {0};
	pfd.fd = (tb_long_t)hfile - 1;
	if (etype & TB_ETYPE_READ) pfd.events |= POLLIN;
	if (etype & TB_ETYPE_WRIT) pfd.events |= POLLOUT;
	etype = 0;

	// poll
	tb_long_t r = poll(&pfd, 1, timeout);
	tb_assert_and_check_return_val(r >= 0, -1);

	// timeout?
	tb_check_return_val(r, 0);

	// ok
	if (pfd.revents & POLLIN) etype |= TB_ETYPE_READ;
	if (pfd.revents & POLLOUT) etype |= TB_ETYPE_WRIT;
	return etype;
}
tb_void_t tb_file_kill(tb_handle_t hfile)
{
}
tb_uint64_t tb_file_size(tb_handle_t hfile)
{
	tb_assert_and_check_return_val(hfile, 0);

	struct stat st = {0};
	return !fstat((tb_long_t)hfile - 1, &st) && st.st_size >= 0? (tb_uint64_t)st.st_size : 0;
}
tb_bool_t tb_file_info(tb_char_t const* path, tb_file_info_t* info)
{
	tb_assert_and_check_return_val(path, TB_FALSE);

	// exists?
	tb_check_return_val(!access(path, F_OK), TB_FALSE);

	// get info
	if (info)
	{
		// init info
		tb_memset(info, 0, sizeof(tb_file_info_t));

		// get stat
		struct stat st = {0};
		if (!stat(path, &st))
		{
			// file type
			if (S_ISDIR(st.st_mode)) info->type = TB_FILE_TYPE_DIR;
			else info->type = TB_FILE_TYPE_FILE;

			// file size
			info->size = st.st_size >= 0? (tb_uint64_t)st.st_size : 0;

			// ok
			return TB_TRUE;
		}
	}
	return TB_FALSE;
}
tb_bool_t tb_file_create(tb_char_t const* path, tb_size_t type)
{
	tb_assert_and_check_return_val(path, TB_FALSE);
	switch (type)
	{
	case TB_FILE_TYPE_DIR:
		return !mkdir(path, S_IRWXU)? TB_TRUE : TB_FALSE;
	case TB_FILE_TYPE_FILE:
		{
			tb_long_t fd = open(path, O_CREAT | O_TRUNC, 0777);
			if (fd >= 0) 
			{
				close(fd); 
				return TB_TRUE;
			}
		}
		break;
	default:
		break;
	}
	return TB_FALSE;
}
tb_void_t tb_file_delete(tb_char_t const* path, tb_size_t type)
{
	tb_assert_and_check_return(path);
	remove(path);
}