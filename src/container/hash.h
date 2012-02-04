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
 * \file		hash.h
 *
 */
#ifndef TB_CONTAINER_HASH_H
#define TB_CONTAINER_HASH_H

/* ///////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "item.h"

/* ///////////////////////////////////////////////////////////////////////
 * macros
 */
#define TB_HASH_SIZE_MICRO 					(64)
#define TB_HASH_SIZE_SMALL 					(256)
#define TB_HASH_SIZE_LARGE 					(65536)

#ifdef TB_CONFIG_MEMORY_MODE_SMALL
# 	define TB_HASH_SIZE_DEFAULT 			TB_HASH_SIZE_SMALL
#else
# 	define TB_HASH_SIZE_DEFAULT 			TB_HASH_SIZE_LARGE
#endif

/* ///////////////////////////////////////////////////////////////////////
 * types
 */

// the hash item type
typedef struct __tb_hash_item_t
{
	// the item name
	tb_pointer_t 			name;

	// the item data
	tb_pointer_t 			data;

}tb_hash_item_t;

// the hash item list type
typedef struct __tb_hash_item_list_t
{
	tb_size_t 				size;
	tb_size_t 				maxn;

}tb_hash_item_list_t;

/* the hash type
 *
 *
 *                 0        1        3       ...     ...                n       n + 1
 * hash_list: |--------|--------|--------|--------|--------|--------|--------|--------|
 *                         |
 *                       -----    
 * item_list:           |     |       key:0                                      
 *                       -----   
 *                      |     |       key:1                                              
 *                       -----               <= insert by binary search algorithm
 *                      |     |       key:2                                               
 *                       -----  
 *                      |     |       key:3                                               
 *                       -----   
 *                      |     |       key:4                                               
 *                       -----  
 *                      |     |                                              
 *                       -----  
 *                      |     |                                              
 *                       -----  
 *                      |     |                                              
 *                       -----  
 *
 */
typedef struct __tb_hash_t
{
	// the hash list
	tb_hash_item_list_t** 	hash_list;
	tb_size_t 				hash_size;

	// the item size
	tb_size_t 				item_size;

	// the item maxn
	tb_size_t 				item_maxn;

	// the hash item
	tb_hash_item_t 			hash_item;

	// the hash func
	tb_item_func_t 			name_func;
	tb_item_func_t 			data_func;

}tb_hash_t;

/* ///////////////////////////////////////////////////////////////////////
 * interfaces
 */

// init & exit
tb_hash_t* 				tb_hash_init(tb_size_t size, tb_item_func_t name_func, tb_item_func_t data_func);
tb_void_t 				tb_hash_exit(tb_hash_t* hash);

// accessors & modifiors
tb_void_t 				tb_hash_clear(tb_hash_t* hash);

tb_pointer_t 			tb_hash_at(tb_hash_t* hash, tb_cpointer_t name);
tb_cpointer_t 			tb_hash_const_at(tb_hash_t const* hash, tb_cpointer_t name);

tb_pointer_t 			tb_hash_get(tb_hash_t* hash, tb_cpointer_t name);
tb_void_t 	 			tb_hash_del(tb_hash_t* hash, tb_cpointer_t name);
tb_void_t 	 			tb_hash_set(tb_hash_t* hash, tb_cpointer_t name, tb_cpointer_t data);

// attributes
tb_size_t 				tb_hash_size(tb_hash_t const* hash);
tb_size_t 				tb_hash_maxn(tb_hash_t const* hash);

// debug
tb_void_t 				tb_hash_dump(tb_hash_t const* hash);

/* iterator
 * 
 * tb_size_t itor = tb_hash_itor_head(hash);
 * tb_size_t tail = tb_hash_itor_tail(hash);
 * for (; itor != tail; itor = tb_hash_itor_next(hash, itor))
 * {
 * 		tb_hash_item_t const* item = tb_hash_itor_const_at(hash, itor);
 * 		if (item)
 * 		{
 *
 * 		}
 * }
 *
 * head: => the first item
 * tail: => behind the last item, no item
 *
 * \note the index of the same item is mutable, only for iterator
 */
tb_hash_item_t* 		tb_hash_itor_at(tb_hash_t* hash, tb_size_t itor);
tb_hash_item_t const* 	tb_hash_itor_const_at(tb_hash_t const* hash, tb_size_t itor);
tb_size_t 				tb_hash_itor_head(tb_hash_t const* hash);
tb_size_t 				tb_hash_itor_tail(tb_hash_t const* hash);
tb_size_t 				tb_hash_itor_next(tb_hash_t const* hash, tb_size_t itor);

/* foreach
 *
 * tb_bool_t tb_hash_item(tb_hash_t* hash, tb_hash_item_t* item, tb_bool_t* berase, tb_pointer_t udata)
 * {
 *  	tb_assert_and_check_return_val(hash && berase, TB_FALSE);
 *
 * 		// is tail?
 * 		if (!item) ;
 *
 * 		// erase it
 * 		// *berase = TB_TRUE;
 *
 * 		// ok
 *  	return TB_TRUE;
 *
 *  fail:
 *  	// break
 *   	return TB_FALSE;
 * }
 *
 */
tb_void_t 				tb_hash_foreach(tb_hash_t* hash, tb_bool_t (*item)(tb_hash_t* hash, tb_hash_item_t* item, tb_bool_t* berase, tb_pointer_t udata), tb_pointer_t udata);

#endif

