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
#include <stdlib.h>

#if defined(__LOG4C_DEBUG__) && defined(__GLIBC__)
#include <mcheck.h>
#endif

/**
 * @bug (hpux) the constructor needs to be first extern symbol. 
 * Otherwise, it is not called .. why ???
 */

/******************************************************************************/
extern int log4c_init(void)
{    
    /* activate GLIBC allocation debugging */
#if defined(__LOG4C_DEBUG__) && defined(__GLIBC__)
    mtrace();
#endif

    {
	/**
	 * @bug init default appenders/layouts/types ?? conflicts with
	 * constructors.
	 */	
    }

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
	snprintf(rcfiles[1], sizeof(rcfiles[1]) - 1, "%s/.log4crc", getenv("HOME"));
        
	for (i = 0; i < nrcfiles; i++) {
	    if (log4c_rc_load(log4c_rc, rcfiles[i]) == -1)
		sd_error("loading %s failed", rcfiles[i]);
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

    return 0;
}

/******************************************************************************/
extern int log4c_fini(void)
{
    if (log4c_rc->config.nocleanup)
	return -1;

    sd_debug("cleaning up");

    sd_factory_delete(log4c_category_factory);
    sd_factory_delete(log4c_appender_factory);
    sd_factory_delete(log4c_layout_factory);

    log4c_rc_delete(log4c_rc);

#if defined(__LOG4C_DEBUG__) && defined(__GLIBC__)
    muntrace();
#endif
    
    return 0;
}

/******************************************************************************/
#ifdef __GNUC__
extern void __attribute__ ((constructor)) __log4c_init(void)
{    
    log4c_init();
}

extern void __attribute__ ((destructor)) __log4c_fini(void)
{    
    log4c_fini();
}
#endif
