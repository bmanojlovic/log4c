static const char version[] = "$Id$";

/*
 * Copyright 2002, Cimai Technology SA (www.cimai.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <sd/store.h>
#include <sd/malloc.h>
#include <stdlib.h>

struct __sd_store {
    size_t 		capacity;
    size_t 		size;
    size_t 		npool;
    void**		pool;
};

/******************************************************************************/
extern sd_store_t* sd_store_new(size_t a_capacity, size_t a_size)
{
    int i;
    sd_store_t* this;
    
    this		= sd_calloc(1, sizeof(*this));
    this->capacity	= a_capacity ? a_capacity : 100;
    this->size		= a_size;
    this->npool		= 0;
    /* FIXME: allocation should be done at first use */
    this->pool		= sd_calloc(this->capacity, sizeof(void*));

    for (i = 0; i < this->capacity; i++)
	this->pool[i] = sd_calloc(1, a_size);
    
    return this;
}

/******************************************************************************/
extern void sd_store_reset(sd_store_t* this)
{
    if (!this)
	return;

    this->npool = 0;
}

/******************************************************************************/
extern void* sd_store_get(sd_store_t* this)
{
    int i;

    if (!this)
	return NULL;

    if (this->npool < this->capacity) {
	memset(this->pool[this->npool], 0, this->size);
	return this->pool[this->npool++];
    }
    
    /* increase pool capacity */
    this->capacity *= 2;    
    this->pool      = sd_realloc(this->pool, this->capacity * sizeof(void*));

    for (i = this->npool; i < this->capacity; i++)
	this->pool[i] = sd_calloc(1, this->size);
    
    return this->pool[this->npool++];
}

/******************************************************************************/
extern void sd_store_put(sd_store_t* this, void* a_data)
{
    ;
}

/******************************************************************************/
extern void sd_store_delete(sd_store_t* this)
{
    int i;
    
    if (!this)
	return;

    for (i = 0; i < this->capacity; i++)
	free(this->pool[i]);
    free(this->pool);
    free(this);
}

