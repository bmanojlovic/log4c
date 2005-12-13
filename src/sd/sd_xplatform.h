/* $Id$
 *
 * sd_xplatform.h
 * 
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef __sd_xplatform_h
#define __sd_xplatform_h

#ifndef _WIN32
#include <unistd.h>
#include <sys/time.h>
#else
#include <time.h>
#include <windows.h>
#include <winsock.h>
#endif


#ifdef HAVE_STDINT_H
#       include <stdint.h>
#define  XP_UINT64 uint64_t
#else
#ifndef _WIN32
#define  XP_UINT64 long long
#else
#define  XP_UINT64 DWORD64
#endif
#endif

#include <sd/defs.h>


extern int sd_optind;

extern void getopt_reset(void); 

extern int sd_getopt(int argc, char *const *argv, const char *opts);

#ifdef _WIN32
#define SD_GETOPT(a,b,c) sd_getopt(a,b,c)
#define SD_OPTIND sd_optind
#else
#define SD_GETOPT(a,b,c) getopt(a,b,c)
#define SD_OPTIND optind
#endif

extern int sd_gettimeofday(struct timeval* tp, void* tzp);

#ifdef _WIN32
#define SD_GETTIMEOFDAY(a,b) sd_gettimeofday(a,b)
#else
#define SD_GETTIMEOFDAY(a,b) gettimeofday(a,b)
#endif

#ifndef _WIN32
#define DIFF_CMD  "/usr/bin/diff -q"
#else
#define DIFF_CMD  "comp.exe"
#endif

#ifdef _WIN32
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define alloca _alloca
#define strncasecmp strnicmp
#define strcasecmp stricmp
#define YY_NO_UNISTD_H
#endif


#endif
