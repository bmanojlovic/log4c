static const char version[] = "$Id$";

/*
 * appender_test.c
 *
 * Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <log4c/appender.h>
#include <stdio.h>
#include <string.h>

/*******************************************************************************/
static int test_append(log4c_appender_t* this, 
		       const log4c_logging_event_t* a_event)
{
    FILE* fp = log4c_appender_get_udata(this);

    return fprintf(fp, "[%s] %s", log4c_appender_get_name(this),
		   a_event->evt_rendered_msg);
}

/*******************************************************************************/
#ifdef __GNUC__
const log4c_appender_type_t log4c_appender_type_test = {
    name:	"test",
    open:	NULL,
    append:	test_append,
    close:	NULL,
};
log4c_appender_type_define(log4c_appender_type_test);
#else
#ifdef __sun
const log4c_appender_type_t log4c_appender_type_test = {
  "test",
  NULL,
  test_append,
  NULL,
};
#pragma init (__log4c_init_appender_type_log4c_appender_type_test)
static void __log4c_init_appender_type_log4c_appender_type_test(void)
{ log4c_appender_type_set(&log4c_appender_type_test); }
#endif
#endif
