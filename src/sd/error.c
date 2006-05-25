#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>


#include <string.h>
#include <stdlib.h>
#include <log4c/defs.h>
#include <sd/error.h>
#ifdef HAVE_VARARGS_H
#include <varargs.h>
#endif

/*
  These function provide replacements for the analogous functions
  on platforms/compilers where a variable number of arguments are not supported
  such as windows/msvc6 for example.

  See error.h for the preprocessor logic as to whether these get used or not.

*/

int sd_debug_fn(const char *a_format,...){

    va_list va_args;
    int rc = 0;   
#ifdef __SD_DEBUG__ 
    char format_buf[2048];

    snprintf(format_buf, sizeof(format_buf),
	     "[DEBUG] %s \n", a_format);

    if ( getenv("SD_DEBUG") ){
	rc = vfprintf(stderr, format_buf, va_args );
    }
#endif
    return rc;
}

int sd_error_fn(const char *a_format,... ){

    va_list va_args;
    int rc = 0;
#ifdef __SD_DEBUG__
    char format_buf[2048];

    snprintf(format_buf, sizeof(format_buf),
	     "[ERROR] %s \n", a_format);
    
    if ( getenv("SD_ERROR") ){
	 vfprintf(stderr, format_buf, va_args );
    }
#endif

    return rc;
}

void sd_oserror_fn(const char *a_func, const char *a_param){

#ifdef __SD_DEBUG__
    if ( getenv("SD_ERROR") ){	
	sd_error("%s(%s): #%d %s", a_func, a_param, errno, strerror(errno));
    }
#endif

}
