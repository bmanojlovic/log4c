static const char version[] = "$Id$";

/*
 * layout.c
 *
 * Copyright 2001-2002, Meiosys SA (www.meiosys.com). All rights reserved.
 * Copyright 2001-2002, Cedric Le Goater <legoater@meiosys.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <log4c/layout.h>
#include <sd/sprintf.h>
#include <stdio.h>
#include <string.h>

/*******************************************************************************/
static const char* test_format(
    const log4c_layout_t*  	a_layout,
    const log4c_logging_event_t*a_event)
{
    static char buffer[1024];

    snprintf(buffer, sizeof(buffer), "logging %d bytes.\n",  strlen(a_event->evt_msg));
    return buffer;
}

/*******************************************************************************/
const log4c_layout_type_t log4c_layout_type_test = {
    name:	"test",
    format:	test_format,
};

log4c_layout_type_define(log4c_layout_type_test);
