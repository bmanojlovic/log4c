/* $Id$
 *
 * Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef __log4c_init_h
#define __log4c_init_h

/**
 * @file init.h
 *
 * @brief log4c constructors and destructors
 *
 **/   

/**
 * constructor
 * 
 * @returns 0 for success 
 **/
extern int log4c_init(void);

/**
 * destructor
 *
 * @returns 0 for success 
 **/
extern int log4c_fini(void);

#endif
