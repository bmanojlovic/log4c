/* $Id$
 *
 * Copyright 2002, Cimai Technology SA (www.cimai.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifndef __sd_store_h
#define __sd_store_h

#include <stddef.h>
#include <sd/defs.h>

/**
 * @file store.h
 *
 * @brief memory storage manager
 */

__SD_BEGIN_DECLS

typedef struct __sd_store sd_store_t;

extern sd_store_t*	sd_store_new(size_t a_capacity, size_t a_size);
extern void		sd_store_delete(sd_store_t* a_this);
extern void		sd_store_reset(sd_store_t* a_this);
extern void*		sd_store_get(sd_store_t* a_this);
extern void		sd_store_put(sd_store_t* a_this, void* a_data);

__SD_END_DECLS

#endif
