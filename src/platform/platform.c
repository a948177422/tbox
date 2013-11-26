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
 * Copyright (C) 2009 - 2012, ruki All rights reserved.
 *
 * @author		ruki
 * @file		platform.c
 * @ingroup 	platform
 *
 */

/* ///////////////////////////////////////////////////////////////////////
 * includes
 */
#include "platform.h"
#include "../network/network.h"

/* ///////////////////////////////////////////////////////////////////////
 * implementation
 */

tb_bool_t tb_platform_init()
{
	// init printf
	if (!tb_printf_init(TB_PRINTF_MODE_STDOUT, tb_null)) return tb_false;

	// init socket
	if (!tb_socket_init()) return tb_false;

	// init tstore
	if (!tb_tstore_init()) return tb_false;

	// spak ctime
	tb_ctime_spak();

	// ok
	return tb_true;
}
tb_void_t tb_platform_exit()
{
	// exit dns
	tb_dns_list_exit();

	// exit tstore
	tb_tstore_exit();

	// exit socket
	tb_socket_exit();

	// exit printf
	tb_printf_exit();
}

