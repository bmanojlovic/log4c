static const char version[] = "$Id$";

/*
 * appender_syslog.c
 *
 * Copyright 2001-2002, Meiosys SA (www.meiosys.com). All rights reserved.
 * Copyright 2001-2002, Cedric Le Goater <legoater@meiosys.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <log4c/appender.h>
#include <log4c/priority.h>
#include <syslog.h>

/*******************************************************************************/
static int log4c_to_syslog_priority(int a_priority)
{
    static int priorities[] = {
	LOG_EMERG,	
	LOG_ALERT,	
	LOG_CRIT, 
	LOG_ERR,	
	LOG_WARNING,	
	LOG_NOTICE, 
	LOG_INFO, 	
	LOG_DEBUG 
    };
    int result;
    
    a_priority++;
    a_priority /= 100;
    
    if (a_priority < 0) {
	result = LOG_EMERG;
    } else if (a_priority > 7) {
	result = LOG_DEBUG;
    } else {
	result = priorities[a_priority];
    }
    
    return result;
}

/*******************************************************************************/
static int syslog_open(log4c_appender_t* this)
{
    openlog(log4c_appender_get_name(this), LOG_PID, LOG_USER); 
    return 0;
}

/*******************************************************************************/
static int syslog_append(log4c_appender_t*	this, 
			 const log4c_logging_event_t* a_event)
{

    syslog(log4c_to_syslog_priority(a_event->evt_priority) | LOG_USER, 
	   a_event->evt_rendered_msg); 
    return 0;
}

/*******************************************************************************/
static int syslog_close(log4c_appender_t*	this)
{
    closelog();
    return 0;
}

/*******************************************************************************/
const log4c_appender_type_t log4c_appender_type_syslog = {
    name:	"syslog",
    open:	syslog_open,
    append:	syslog_append,
    close:	syslog_close,
};

log4c_appender_type_define(log4c_appender_type_syslog);
