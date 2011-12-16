/*!The Tiny Box Library
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
 * \file		sstring.c
 *
 */

/* ////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "sstring.h"
#include "../libc/libc.h"
#include "../utils/utils.h"

/* ////////////////////////////////////////////////////////////////////////
 * macros
 */
#ifdef TB_CONFIG_MEMORY_MODE_SMALL
# 	define TB_SSTRING_FMTD_SIZE 		(4096)
#else
# 	define TB_SSTRING_FMTD_SIZE 		(8192)
#endif

/* ////////////////////////////////////////////////////////////////////////
 * init & exit
 */
tb_bool_t tb_sstring_init(tb_sstring_t* string, tb_char_t* data, tb_size_t maxn)
{
	tb_assert_and_check_return_val(string && data && maxn, TB_FALSE);
	string->size = 0;
	string->data = data;
	string->maxn = maxn;
	return TB_TRUE;
}
tb_void_t tb_sstring_exit(tb_sstring_t* string)
{
	if (string) tb_memset(string, 0, sizeof(tb_sstring_t));
}

/* ////////////////////////////////////////////////////////////////////////
 * accessors
 */
tb_char_t const* tb_sstring_cstr(tb_sstring_t const* string)
{
	tb_assert_and_check_return_val(string, TB_NULL);
	return (tb_char_t const*)string->data;
}
tb_size_t tb_sstring_size(tb_sstring_t const* string)
{
	tb_assert_and_check_return_val(string, 0);
	return string->size;
}


/* ////////////////////////////////////////////////////////////////////////
 * modifiors
 */
tb_void_t tb_sstring_clear(tb_sstring_t* string)
{
	tb_assert_and_check_return(string);
	string->size = 0;
	if (string->data) string->data[0] = '\0';
}
tb_char_t const* tb_sstring_strip(tb_sstring_t* string, tb_size_t n)
{
	tb_assert_and_check_return_val(string, TB_NULL);

	// out?
	tb_check_return_val(n < string->maxn, string->data);

	// strip
	string->size = n;
	if (string->data) string->data[n] = '\0';
}
/* ////////////////////////////////////////////////////////////////////////
 * strchr
 */
tb_long_t tb_sstring_strchr(tb_sstring_t const* string, tb_size_t p, tb_char_t c)
{
	tb_char_t const* 	s = tb_sstring_cstr(string);
	tb_size_t 			n = tb_sstring_size(string);
	tb_assert_and_check_return_val(s && p >= 0 && p < n, -1);

	tb_char_t* q = tb_strchr(s + p, c);
	return (q? q - s : -1);
}
tb_long_t tb_sstring_strichr(tb_sstring_t const* string, tb_size_t p, tb_char_t c)
{
	tb_char_t const* 	s = tb_sstring_cstr(string);
	tb_size_t 			n = tb_sstring_size(string);
	tb_assert_and_check_return_val(s && p >= 0 && p < n, -1);

	tb_char_t* q = tb_strichr(s + p, c);
	return (q? q - s : -1);
}

/* ////////////////////////////////////////////////////////////////////////
 * strrchr
 */
tb_long_t tb_sstring_strrchr(tb_sstring_t const* string, tb_size_t p, tb_char_t c)
{
	tb_char_t const* 	s = tb_sstring_cstr(string);
	tb_size_t 			n = tb_sstring_size(string);
	tb_assert_and_check_return_val(s && p >= 0 && p < n, -1);

	tb_char_t* q = tb_strnrchr(s + p, n, c);
	return (q? q - s : -1);
}
tb_long_t tb_sstring_strirchr(tb_sstring_t const* string, tb_size_t p, tb_char_t c)
{
	tb_char_t const* 	s = tb_sstring_cstr(string);
	tb_size_t 			n = tb_sstring_size(string);
	tb_assert_and_check_return_val(s && p >= 0 && p < n, -1);

	tb_char_t* q = tb_strnirchr(s + p, n, c);
	return (q? q - s : -1);
}

/* ////////////////////////////////////////////////////////////////////////
 * strstr
 */
tb_long_t tb_sstring_strstr(tb_sstring_t const* string, tb_size_t p, tb_sstring_t const* s)
{
	return tb_sstring_cstrstr(string, tb_sstring_cstr(s), p);
}
tb_long_t tb_sstring_stristr(tb_sstring_t const* string, tb_size_t p, tb_sstring_t const* s)
{
	return tb_sstring_cstristr(string, tb_sstring_cstr(s), p);
}
tb_long_t tb_sstring_cstrstr(tb_sstring_t const* string, tb_size_t p, tb_char_t const* s2)
{
	tb_char_t const* 	s = tb_sstring_cstr(string);
	tb_size_t 			n = tb_sstring_size(string);
	tb_assert_and_check_return_val(s && p >= 0 && p < n, -1);

	tb_char_t* q = tb_strstr(s + p, s2);
	return (q? q - s : -1);
}
tb_long_t tb_sstring_cstristr(tb_sstring_t const* string, tb_size_t p, tb_char_t const* s2)
{	
	tb_char_t const* 	s = tb_sstring_cstr(string);
	tb_size_t 			n = tb_sstring_size(string);
	tb_assert_and_check_return_val(s && p >= 0 && p < n, -1);

	tb_char_t* q = tb_stristr(s + p, s2);
	return (q? q - s : -1);
}

/* ////////////////////////////////////////////////////////////////////////
 * strrstr
 */
tb_long_t tb_sstring_strrstr(tb_sstring_t const* string, tb_size_t p, tb_sstring_t const* s)
{
	return tb_sstring_cstrrstr(string, tb_sstring_cstr(s), p);
}
tb_long_t tb_sstring_strirstr(tb_sstring_t const* string, tb_size_t p, tb_sstring_t const* s)
{
	return tb_sstring_cstrirstr(string, tb_sstring_cstr(s), p);
}

tb_long_t tb_sstring_cstrrstr(tb_sstring_t const* string, tb_size_t p, tb_char_t const* s2)
{	
	tb_char_t const* 	s = tb_sstring_cstr(string);
	tb_size_t 			n = tb_sstring_size(string);
	tb_assert_and_check_return_val(s && p >= 0 && p < n, -1);

	tb_char_t* q = tb_strnrstr(s + p, n, s2);
	return (q? q - s : -1);
}
tb_long_t tb_sstring_cstrirstr(tb_sstring_t const* string, tb_size_t p, tb_char_t const* s2)
{
	tb_char_t const* 	s = tb_sstring_cstr(string);
	tb_size_t 			n = tb_sstring_size(string);
	tb_assert_and_check_return_val(s && p >= 0 && p < n, -1);

	tb_char_t* q = tb_strnirstr(s + p, n, s2);
	return (q? q - s : -1);
}

/* ////////////////////////////////////////////////////////////////////////
 * strcpy
 */
tb_char_t const* tb_sstring_strcpy(tb_sstring_t* string, tb_sstring_t const* s)
{
	tb_assert_and_check_return_val(s, TB_NULL);
	return tb_sstring_cstrncpy(string, tb_sstring_cstr(s), tb_sstring_size(s));
}
tb_char_t const* tb_sstring_cstrcpy(tb_sstring_t* string, tb_char_t const* s)
{
	tb_assert_and_check_return_val(s, TB_NULL);
	return tb_sstring_cstrncpy(string, s, tb_strlen(s));
}
tb_char_t const* tb_sstring_cstrncpy(tb_sstring_t* string, tb_char_t const* s, tb_size_t n)
{
	tb_assert_and_check_return_val(string && string->data && string->maxn, TB_NULL);
	tb_assert_and_check_return_val(s && n && n < string->maxn, TB_NULL);

	// copy data
	tb_memcpy(string->data, s, n);
	string->data[n] = '\0';
	string->size = n;

	// ok
	return string->data;
}
tb_char_t const* tb_sstring_cstrfcpy(tb_sstring_t* string, tb_char_t const* fmt, ...)
{
	tb_assert_and_check_return_val(string && fmt, TB_NULL);

	// format data
	tb_char_t p[TB_SSTRING_FMTD_SIZE] = {0};
	tb_size_t n = 0;
	tb_va_format(p, TB_SSTRING_FMTD_SIZE, fmt, &n);
	tb_assert_and_check_return_val(n, TB_NULL);
	
	return tb_sstring_cstrncpy(string, p, n);
}
/* ////////////////////////////////////////////////////////////////////////
 * chrcat
 */
tb_char_t const* tb_sstring_chrcat(tb_sstring_t* string, tb_char_t c)
{
	// check
	tb_assert_and_check_return_val(string && string->data && string->maxn, TB_NULL);
	tb_assert_and_check_return_val(string->size + 1 < string->maxn, TB_NULL);

	// append char
	string->data[string->size] = c;
	string->size++;
	string->data[string->size] = '\0';

	// ok
	return string->data;
}

tb_char_t const* tb_sstring_chrncat(tb_sstring_t* string, tb_char_t c, tb_size_t n)
{
	// check
	tb_assert_and_check_return_val(string && string->data && string->maxn, TB_NULL);
	tb_assert_and_check_return_val(string->size + n < string->maxn, TB_NULL);

	// append chars
	tb_memset(string->data + string->size, c, n);
	string->size += n;
	string->data[string->size] = '\0';

	// ok
	return string->data;
}
/* ////////////////////////////////////////////////////////////////////////
 * strcat
 */
tb_char_t const* tb_sstring_strcat(tb_sstring_t* string, tb_sstring_t const* s)
{
	tb_assert_and_check_return_val(s, TB_NULL);
	return tb_sstring_cstrncat(string, tb_sstring_cstr(s), tb_sstring_size(s));
}
tb_char_t const* tb_sstring_cstrcat(tb_sstring_t* string, tb_char_t const* s)
{
	tb_assert_and_check_return_val(s, TB_NULL);
	return tb_sstring_cstrncat(string, s, tb_strlen(s));
}
tb_char_t const* tb_sstring_cstrncat(tb_sstring_t* string, tb_char_t const* s, tb_size_t n)
{
	tb_assert_and_check_return_val(string && string->data && string->maxn, TB_NULL);
	tb_assert_and_check_return_val(s && n && string->size + n < string->maxn, TB_NULL);

	// copy it if null
	if (!string->size) return tb_sstring_cstrncpy(string, s, n);

	// copy data
	tb_memcpy(string->data + string->size, s, n);
	string->data[string->size + n] = '\0';
	string->size += n;

	// ok
	return string->data;
}
tb_char_t const* tb_sstring_cstrfcat(tb_sstring_t* string, tb_char_t const* fmt, ...)
{
	tb_assert_and_check_return_val(string && fmt, TB_NULL);

	// format data
	tb_char_t p[TB_SSTRING_FMTD_SIZE] = {0};
	tb_size_t n = 0;
	tb_va_format(p, TB_SSTRING_FMTD_SIZE, fmt, &n);
	tb_assert_and_check_return_val(n, TB_NULL);
	
	return tb_sstring_cstrncat(string, p, n);
}

/* ////////////////////////////////////////////////////////////////////////
 * strcmp
 */
tb_long_t tb_sstring_strcmp(tb_sstring_t* string, tb_sstring_t const* s)
{
	tb_assert_and_check_return_val(string && s, 0);
	return tb_sstring_cstrncmp(string, s, tb_sstring_size(s));
}
tb_long_t tb_sstring_strimp(tb_sstring_t* string, tb_sstring_t const* s)
{
	tb_assert_and_check_return_val(string && s, 0);
	return tb_sstring_cstrnicmp(string, s, tb_sstring_size(s));
}
tb_long_t tb_sstring_cstrcmp(tb_sstring_t* string, tb_char_t const* s)
{
	tb_assert_and_check_return_val(string && s, 0);
	return tb_sstring_cstrncmp(string, s, tb_strlen(s));
}
tb_long_t tb_sstring_cstricmp(tb_sstring_t* string, tb_char_t const* s)
{
	tb_assert_and_check_return_val(string && s, 0);
	return tb_sstring_cstrnicmp(string, s, tb_strlen(s));
}
tb_long_t tb_sstring_cstrncmp(tb_sstring_t* string, tb_char_t const* s, tb_size_t n)
{
	tb_assert_and_check_return_val(string && string->data && s, 0);
	return tb_strncmp(string->data, s, n);
}
tb_long_t tb_sstring_cstrnicmp(tb_sstring_t* string, tb_char_t const* s, tb_size_t n)
{
	tb_assert_and_check_return_val(string && string->data && s, 0);
	return tb_strnicmp(string->data, s, n);
}
