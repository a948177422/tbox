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
 * \sock		sock.c
 *
 */

/* /////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../../string/string.h"
#include "../../platform/platform.h"

/* /////////////////////////////////////////////////////////
 * macros
 */
#define TB_SSTREAM_HOST_MAX 		(1024)

/* /////////////////////////////////////////////////////////
 * types
 */

// the sock stream type
typedef struct __tb_sstream_t
{
	// the base
	tb_gstream_t 		base;

	// the sock handle
	tb_handle_t 		sock;

	// the sock type
	tb_size_t 			type;

	// the port
	tb_size_t 			port;

	// the host
	tb_char_t 			host[TB_SSTREAM_HOST_MAX];

}tb_sstream_t;


/* /////////////////////////////////////////////////////////
 * details
 */
static __tb_inline__ tb_sstream_t* tb_sstream_cast(tb_gstream_t* gst)
{
	tb_assert_and_check_return_val(gst && gst->type == TB_GSTREAM_TYPE_SOCK, TB_NULL);
	return (tb_sstream_t*)gst;
}
static tb_bool_t tb_sstream_open(tb_gstream_t* gst)
{
	tb_sstream_t* sst = tb_sstream_cast(gst);
	tb_assert_and_check_return_val(sst && !sst->sock && gst->url, TB_FALSE);

	// skip prefix
	tb_size_t 			n = tb_strlen(gst->url);
	tb_char_t const* 	p = gst->url;
	tb_char_t const* 	e = p + n;
	tb_assert_and_check_return_val(n > 7 && !tb_strncmp(p, "sock://", 7), TB_FALSE);
	p += 7;

	// get host
	tb_char_t* pb = sst->host;
	tb_char_t* pe = sst->host + TB_SSTREAM_HOST_MAX - 1;
	while (p < e && pb < pe && *p && *p != '/' && *p != ':') *pb++ = *p++;
	*pb = '\0';
	tb_trace("[sst]: host: %s", sst->host);

	// get port
	if (*p && *p == ':')
	{
		tb_char_t port[12];
		pb = port;
		pe = port + 12 - 1;
		for (p++; p < e && pb < pe && *p && *p != '/'; ) *pb++ = *p++;
		*pb = '\0';
		sst->port = tb_stou32(port);
	}
	tb_assert_and_check_return_val(sst->port, TB_FALSE);
	tb_trace("[sst]: port: %u", sst->port);

	// open socket
	sst->sock = tb_socket_client_open(sst->host, sst->port, sst->type, TB_FALSE);
	tb_assert_and_check_return_val(sst->sock, TB_FALSE);

	// ok
	return TB_TRUE;
}
static tb_void_t tb_sstream_close(tb_gstream_t* gst)
{
	tb_sstream_t* sst = tb_sstream_cast(gst);
	if (sst && sst->sock)
	{
		tb_socket_close(sst->sock);
		sst->sock = TB_NULL;
	}
}
static tb_long_t tb_sstream_read(tb_gstream_t* gst, tb_byte_t* data, tb_size_t size)
{
	tb_sstream_t* sst = tb_sstream_cast(gst);
	tb_assert_and_check_return_val(sst && sst->sock && data, -1);
	tb_check_return_val(size, 0);

	// read data
	return tb_socket_recv(sst->sock, data, size);
}
static tb_long_t tb_sstream_writ(tb_gstream_t* gst, tb_byte_t* data, tb_size_t size)
{
	tb_sstream_t* sst = tb_sstream_cast(gst);
	tb_assert_and_check_return_val(sst && sst->sock && data, -1);
	tb_check_return_val(size, 0);

	// writ data
	return tb_socket_send(sst->sock, data, size);
}
static tb_bool_t tb_sstream_ioctl1(tb_gstream_t* gst, tb_size_t cmd, tb_pointer_t arg1)
{
	tb_sstream_t* sst = tb_sstream_cast(gst);
	tb_assert_and_check_return_val(sst, TB_FALSE);

	switch (cmd)
	{
	default:
		break;
	}
	return TB_FALSE;
}
/* /////////////////////////////////////////////////////////
 * interfaces
 */

tb_gstream_t* tb_gstream_init_sock()
{
	tb_gstream_t* gst = (tb_gstream_t*)tb_calloc(1, sizeof(tb_sstream_t));
	tb_assert_and_check_return_val(gst, TB_NULL);

	// init stream
	tb_sstream_t* sst = (tb_sstream_t*)gst;
	gst->type 	= TB_GSTREAM_TYPE_SOCK;
	gst->open 	= tb_sstream_open;
	gst->close 	= tb_sstream_close;
	gst->read 	= tb_sstream_read;
	gst->writ 	= tb_sstream_writ;
	gst->ioctl1 = tb_sstream_ioctl1;
	sst->sock 	= TB_NULL;
	sst->type 	= TB_SOCKET_TYPE_TCP;

	return gst;
}
