/* $Id$
 *
 * Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef __sd_error_h
#define __sd_error_h

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#ifdef __SD_DEBUG__ 

#   define __sd_str(n) #n
#   define __sd_location(n) __FUNCTION__"() at " __FILE__":" __sd_str(n)
#   define sd_location  __sd_location(__LINE__)

#   define sd_debug(a_format, ...) \
	(getenv("SD_DEBUG") ? fprintf(stderr, "[DEBUG] "sd_location" "a_format"\n", ##__VA_ARGS__ ) : 0)
#   define sd_error(a_format, ...) \
	(getenv("SD_ERROR") ? fprintf(stderr, "[ERROR] "sd_location" "a_format"\n", ##__VA_ARGS__ ) : 0)
#   define sd_oserror(afunc, aparam) \
        (sd_error("%s(%s): #%d %s", afunc, aparam, errno, strerror(errno)), -1)

#else

#   define sd_debug(...)
#   define sd_error(...)
#   define sd_oserror(...) -1

#endif

/* backward compatibility */
#define os_error sd_oserror

#endif
