/* $Id$
 *
 * layout.h
 * 
 * Copyright 2001-2002, Meiosys SA (www.meiosys.com). All rights reserved.
 * Copyright 2001-2002, Cedric Le Goater <legoater@meiosys.com>. All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef log4c_layout_h
#define log4c_layout_h

/**
 * @file layout.h
 *
 * @brief Interface for user specific layout format of log4c_logging_event
 * events. 
 *
 * @todo the layout interface needs a better configuration system
 * depending on the layout type. The udata field is a just a trick.
 *
 * @todo a pattern layout would be welcomed !!
 **/

#include <log4c/defs.h>
#include <log4c/logging_event.h>
#include <stdio.h>

__LOG4C_BEGIN_DECLS

struct __log4c_layout;

/**
 * log4c layout class 
 **/
typedef struct __log4c_layout log4c_layout_t;

/**
 * @brief log4c layout type class
 *
 * Attributes description:
 * 
 * @li @c name layout type name 
 * @li @c format 
 **/
typedef struct log4c_layout_type {
    const char* name;
    const char* (*format) (const log4c_layout_t*, const log4c_logging_event_t*);
} log4c_layout_type_t;

/**
 * Get a pointer to an existing layout type.
 *
 * @param a_name the name of the layout type to return.  
 * @returns a pointer to an existing layout type, or NULL if no layout
 * type with the specified name exists.
 **/
extern const log4c_layout_type_t* log4c_layout_type_get(const char* a_name);

/**
 * Set an layout type.
 *
 * @param a_type a pointer to the new layout type to set.
 * @returns a pointer to the previous layout type of same name.
 **/
extern const log4c_layout_type_t* log4c_layout_type_set(
    const log4c_layout_type_t* a_type);

/**
 * Get a pointer to an existing layout.
 *
 * @param a_name the name of the layout to return.
 * @returns a pointer to an existing layout, or NULL if no layout
 * with the specfied name exists.
 **/
extern log4c_layout_t* log4c_layout_get(const char* a_name);

/**
 * Constructor for layout. 
 **/
extern log4c_layout_t* log4c_layout_new(const char* a_name);

/**
 * Destructor for layout.
 **/
extern void log4c_layout_delete(log4c_layout_t* a_layout);

/**
 * @param a_layout the log4c_layout_t object
 * @return the layout name
 **/
extern const char* log4c_layout_get_name(const log4c_layout_t* a_layout);

/**
 * @param a_layout the log4c_layout_t object
 * @return a log4c_layout_type_t object
 **/
extern const log4c_layout_type_t* log4c_layout_get_type(
    const log4c_layout_t* a_layout);

/**
 * sets the layout type
 *
 * @param a_layout the log4c_layout_t object
 * @param a_type the new layout type
 * @return the previous layout type
 **/
extern const log4c_layout_type_t* log4c_layout_set_type(
    log4c_layout_t* a_layout,
    const log4c_layout_type_t* a_type);

/**
 * @param a_layout the log4c_layout_t object
 * @return the layout user data
 **/
extern void* log4c_layout_get_udata(const log4c_layout_t* a_layout);

/**
 * sets the layout user data
 *
 * @param a_layout the log4c_layout_t object
 * @param a_udata the new layout user data
 * @return the previous layout user data
 **/
extern void* log4c_layout_set_udata(log4c_layout_t*	a_layout, 
				    void*		a_udata);
/**
 * format a log4c_logging_event events to a string.
 *
 * @param a_layout the log4c_layout_t object
 * @param a_event a logging_event_t object
 * @returns an appendable string.
 **/
extern const char* log4c_layout_format(
    const log4c_layout_t*		a_layout,
    const log4c_logging_event_t*	a_event);

/**
 * prints the layout on a stream
 * @param a_layout the log4c_layout_t object
 * @param a_stream the stream
 **/
extern void log4c_layout_print(
    const log4c_layout_t* a_layout, FILE* a_stream);

/**
 * Helper macro to define static layout types.
 *
 * @param a_type the log4c_layout_type_t object to define
 * @warning needs GCC support
 **/
#ifdef __GNUC__
#   define log4c_layout_type_define(a_type) \
static void __attribute__ ((constructor)) __log4c_init_layout_type_##a_type(void) \
{ log4c_layout_type_set(&a_type); }
#else
#   define log4c_layout_type_define(a_type)
#endif

/**
 * @internal
 **/
struct __sd_factory;
extern struct __sd_factory* log4c_layout_factory;

__LOG4C_END_DECLS

#endif
