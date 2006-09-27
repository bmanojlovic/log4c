static const char version[] = "$Id$";

/*
 * init.c
 *
 * Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <log4c/init.h>
#include <log4c/category.h>
#include <log4c/appender.h>
#include <log4c/layout.h>
#include <log4c/rc.h>
#include <sd/error.h>
#include <sd/sprintf.h>
#include <sd/factory.h>
#include <sd/sd_xplatform.h>
#include <stdlib.h>

#include <appender_type_stream.h>
#include <appender_type_stream2.h>
#include <appender_type_syslog.h>
#include <appender_type_mmap.h>
#include <layout_type_basic.h>
#include <layout_type_dated.h>
#include <layout_type_basic_r.h>
#include <layout_type_dated_r.h>

#if defined(__LOG4C_DEBUG__) && defined(__GLIBC__)
#include <mcheck.h>
#endif

static const log4c_layout_type_t * const layout_types[] = {
    &log4c_layout_type_basic,
    &log4c_layout_type_dated,
    &log4c_layout_type_basic_r,
    &log4c_layout_type_dated_r    
};
static size_t nlayout_types = sizeof(layout_types) / sizeof(layout_types[0]);

static const log4c_appender_type_t * const appender_types[] = {
    &log4c_appender_type_stream,
    &log4c_appender_type_stream2,
    &log4c_appender_type_mmap,
#ifdef HAVE_SYSLOG_H
    &log4c_appender_type_syslog    
#endif
};
static size_t nappender_types = sizeof(appender_types) / sizeof(appender_types[0]);

static int log4c_is_init = 0;

/******************************************************************************/
extern int log4c_init(void)
{    
    int i;
    int ret = 0;

    /* activate GLIBC allocation debugging */
#if defined(__LOG4C_DEBUG__) && defined(__GLIBC__)
    mtrace();
#endif

    if (log4c_is_init)
	return 0;

    log4c_is_init++;

    /* Initialize default layouts and appenders */
    for (i = 0; i < nlayout_types; i++) 
	log4c_layout_type_set(layout_types[i]);

    for (i = 0; i < nappender_types; i++) 
	log4c_appender_type_set(appender_types[i]);

    /* load configuration files */
    {
	static char rcfiles[][256] = {
	    "$LOG4C_RCPATH/log4crc",
	    "$HOME/.log4crc",
	    "./log4crc" 
	};
	static const int nrcfiles = sizeof(rcfiles) / sizeof(rcfiles[0]);
	int i;
	
	snprintf(rcfiles[0], sizeof(rcfiles[0]) - 1, "%s/log4crc", 
		 getenv("LOG4C_RCPATH") ? getenv("LOG4C_RCPATH") : LOG4C_RCPATH);
	snprintf(rcfiles[1], sizeof(rcfiles[1]) - 1, "%s/.log4crc",
		 getenv("HOME") ? getenv("HOME") : "");
        
	for (i = 0; i < nrcfiles; i++) {
	    if (SD_ACCESS_READ(rcfiles[i])) continue;

	    if (log4c_load(rcfiles[i]) == -1) {
		sd_error("loading %s failed", rcfiles[i]);
		ret = -1;
	    }
	    else
		sd_debug("loading %s succeeded", rcfiles[i]);		
	}
    }

    /* override configuration with environment variables */
    {
	const char* priority;
	const char* appender;

	if ( (priority = getenv("LOG4C_PRIORITY")) != NULL)
	    log4c_category_set_priority(log4c_category_get("root"), 
					log4c_priority_to_int(priority)); 
	
	if ( (appender = getenv("LOG4C_APPENDER")) != NULL)
	    log4c_category_set_appender(log4c_category_get("root"), 
					log4c_appender_get(appender));
    }

    return ret;
}

/******************************************************************************/
extern int log4c_fini(void)
{
    if (log4c_rc->config.nocleanup)
	return -1;

    if (!log4c_is_init)
	return 0;

    log4c_is_init--;

    sd_debug("cleaning up");

    if (log4c_category_factory) {
	sd_factory_delete(log4c_category_factory);
	log4c_category_factory = NULL;
    }

    if (log4c_appender_factory) {
	sd_factory_delete(log4c_appender_factory);
	log4c_appender_factory = NULL;
    }

    if (log4c_layout_factory) {
	sd_factory_delete(log4c_layout_factory);
	log4c_layout_factory = NULL;
    }

#if defined(__LOG4C_DEBUG__) && defined(__GLIBC__)
    muntrace();
#endif
    
    return 0;
}

/******************************************************************************/
#ifdef __GNUC__
extern void __attribute__ ((constructor)) __log4c_init(void)
{    
    printf("please make sure log4c_init() is called to initialize log4c");
}

extern void __attribute__ ((destructor)) __log4c_fini(void)
{    
    printf("please make sure log4c_fini() is called to clean up log4c internals");
}
#endif
