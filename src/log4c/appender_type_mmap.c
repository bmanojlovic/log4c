static const char version[] = "$Id$";

/*
 * appender_mmap.c
 *
 * Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <log4c/appender.h>
#include <sd/error.h>
#include <sd/malloc.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/* This could be public */
struct mmap_info {
    const char*	name;
    int		fd;
    size_t	length;
    void*	addr;
    void*	ptr;
    struct stat st;
};

static int mmap_info_delete(struct mmap_info* a_minfo);
static struct mmap_info* mmap_info_new(const char* a_name);

/*******************************************************************************/
static struct mmap_info* mmap_info_new(const char* a_name)
{
    struct mmap_info* minfo = NULL;

    minfo = sd_calloc(1, sizeof(*minfo));
    minfo->name = a_name;

    if ( (minfo->fd = open(minfo->name, O_RDWR, 0644)) == -1) {
	sd_oserror("open", minfo->name);
	mmap_info_delete(minfo);
	return NULL;
    }

    if (fstat(minfo->fd, &minfo->st) == -1) {
	sd_oserror("fstat", minfo->name);
	mmap_info_delete(minfo);
	return NULL;
    }

    minfo->length = minfo->st.st_size;

    if (!minfo->length) {
	sd_error("file %s is empty", minfo->name);
	mmap_info_delete(minfo);
	return NULL;
    }

    return minfo;
}
/*******************************************************************************/
static int mmap_info_delete(struct mmap_info* a_minfo)
{
    if (!a_minfo)
	return -1;

    close(a_minfo->fd);
    free(a_minfo);
    return 0;
}

/*******************************************************************************/
static int mmap_open(log4c_appender_t* this)
{
    struct mmap_info* minfo = log4c_appender_get_udata(this);
    
    if (minfo)
	return 0;

    if ( (minfo = mmap_info_new(log4c_appender_get_name(this))) == NULL)
	return -1;

    minfo->addr = mmap(NULL, minfo->length, PROT_READ|PROT_WRITE,
		      MAP_SHARED, minfo->fd, 0);
    if (minfo->addr == NULL) {
	sd_oserror("mmap", minfo->name);
	mmap_info_delete(minfo);
	return -1;
    }

    minfo->ptr = minfo->addr;
    log4c_appender_set_udata(this, minfo);
    return 0;
}

/*******************************************************************************/
static int mmap_append(log4c_appender_t*	this, 
		       const log4c_logging_event_t* a_event)
{
    size_t size, available;
    struct mmap_info* minfo = log4c_appender_get_udata(this);

    if (!minfo && !minfo->ptr)
	return 0;

    size = strlen(a_event->evt_rendered_msg);
    available = (minfo->addr + minfo->length) - minfo->ptr;

    if (size > available) {
	memcpy(minfo->ptr, a_event->evt_rendered_msg, available);
	minfo->ptr = minfo->addr;
	size -= available;
    }

    memcpy(minfo->ptr, a_event->evt_rendered_msg, size);
    minfo->ptr += size;
    return 0;
}

/*******************************************************************************/
static int mmap_close(log4c_appender_t*	this)
{
    struct mmap_info* minfo = log4c_appender_get_udata(this);

    if (!minfo)
	return 0;

    if (munmap(minfo->addr, minfo->length) == -1)
	sd_oserror("munmap", minfo->name);

    mmap_info_delete(minfo);

    log4c_appender_set_udata(this, NULL);
    return 0;
}

/*******************************************************************************/
const log4c_appender_type_t log4c_appender_type_mmap = {
    "mmap",
    mmap_open,
    mmap_append,
    mmap_close,
};

log4c_appender_type_define(log4c_appender_type_mmap);
