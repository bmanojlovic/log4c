/* $Id$
 *
 * rc.h
 *
 * Copyright 2001-2002, Meiosys SA (www.meiosys.com). All rights reserved.
 * Copyright 2001-2002, Cedric Le Goater <legoater@meiosys.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef __log4c_rc_h
#define __log4c_rc_h

#include <log4c/defs.h>

__LOG4C_BEGIN_DECLS

typedef struct 
{
    struct 
    {
	int nocleanup;
	int bufsize;
	int debug;
    } config;

} log4c_rc_t;

#define log4c_rc	__log4c_rc()

extern log4c_rc_t*	__log4c_rc(void);
extern log4c_rc_t*	log4c_rc_new(void);
extern void		log4c_rc_delete(log4c_rc_t* a_rc);
extern int		log4c_rc_load(log4c_rc_t* a_rc, const char* a_filename);

__LOG4C_END_DECLS

#endif
