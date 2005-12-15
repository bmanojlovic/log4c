static const char version[] = "$Id$";

/*
 * rc.c
 *
 * Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <log4c/rc.h>
#include <log4c/category.h>
#include <log4c/appender.h>
#include <log4c/layout.h>
#include <sd/error.h>
#include <sd/domnode.h>
#include <sd/malloc.h>
#include <sd/sd_xplatform.h>
#include <stdlib.h>
#include <string.h>

    
static log4c_rc_t __log4c_rc = { { 0, 0, 0 } };

log4c_rc_t* const log4c_rc = &__log4c_rc;

/******************************************************************************/
static int config_load(log4c_rc_t* this, sd_domnode_t* anode)
{
    sd_list_iter_t* i = NULL;

    for (i = sd_list_begin(anode->children); i != sd_list_end(anode->children); 
	 i = sd_list_iter_next(i)) 
    {
	sd_domnode_t* node = i->data;

	if (!strcmp(node->name, "nocleanup")) {
	    this->config.nocleanup = atoi(node->value);
	    if (this->config.nocleanup)
		sd_debug("deactivating log4c cleanup");		
	}

	if (!strcmp(node->name, "bufsize")) {	    
	    this->config.bufsize = atoi(node->value);

	    if (this->config.bufsize)
		sd_debug("using fixed buffer size of %d bytes", 
			 this->config.bufsize);
	    else
		sd_debug("using dynamic allocated buffer");
	}

	if (!strcmp(node->name, "debug")) {
	    sd_domnode_t* level = sd_domnode_attrs_get(node, "level");

	    if (level) {
		this->config.debug = atoi(level->value);
		sd_debug("activating log4c debugging. level = %d", this->config.debug);
	    }
	}
    }
    
    return 0;
}

/******************************************************************************/
static int category_load(log4c_rc_t* this, sd_domnode_t* anode)
{
    sd_domnode_t*     name     = sd_domnode_attrs_get(anode, "name");
    sd_domnode_t*     priority = sd_domnode_attrs_get(anode, "priority");
    sd_domnode_t*     additivity = sd_domnode_attrs_get(anode, "additivity");
    sd_domnode_t*     appender = sd_domnode_attrs_get(anode, "appender");
    log4c_category_t* cat      = NULL;
    
    if (!name) {
	sd_error("attribute \"name\" is missing");
	return -1;
    }

    cat = log4c_category_get(name->value);
    
    if (priority)
	log4c_category_set_priority(
	    cat, log4c_priority_to_int(priority->value));
    
    if (additivity) {
	if (!strcasecmp(additivity->value, "false")) {
	    log4c_category_set_additivity(cat, 0);
	} else if (!strcasecmp(additivity->value, "true")) {
	    log4c_category_set_additivity(cat, 1);
	} else {
	    sd_error("additivity value is invalid : %s", additivity->value);
	}
    }

    if (appender)
	log4c_category_set_appender(
	    cat, log4c_appender_get(appender->value));

    return 0;
}

/******************************************************************************/
static int appender_load(log4c_rc_t* this, sd_domnode_t* anode)
{
    sd_domnode_t*     name   = sd_domnode_attrs_get(anode, "name");
    sd_domnode_t*     type   = sd_domnode_attrs_get(anode, "type");
    sd_domnode_t*     layout = sd_domnode_attrs_get(anode, "layout");
    log4c_appender_t* app    = NULL;
    
    if (!name) {
	sd_error("attribute \"name\" is missing");
	return -1;
    }
    
    app = log4c_appender_get(name->value);
    
    if (type)
	log4c_appender_set_type(app, log4c_appender_type_get(type->value));

    if (layout)
	log4c_appender_set_layout(app, log4c_layout_get(layout->value));

    return 0;
}

/******************************************************************************/
static int layout_load(log4c_rc_t* this, sd_domnode_t* anode)
{
    sd_domnode_t*   name   = sd_domnode_attrs_get(anode, "name");
    sd_domnode_t*   type   = sd_domnode_attrs_get(anode, "type");
    log4c_layout_t* layout = NULL;
    
    if (!name) {
	sd_error("attribute \"name\" is missing");
	return -1;
    }

    layout = log4c_layout_get(name->value);

    if (type)
	log4c_layout_set_type(layout, log4c_layout_type_get(type->value));

    return 0;
}

/******************************************************************************/
extern int log4c_rc_load(log4c_rc_t* this, const char* a_filename)
{    
    sd_list_iter_t* i = NULL;
    sd_domnode_t*   node = NULL;        
    sd_domnode_t*   root_node = NULL;

    if (!this)
	return -1;

    root_node = sd_domnode_new(NULL, NULL);
    
    if (sd_domnode_load(root_node, a_filename) == -1) {
	sd_domnode_delete(root_node);
	return -1;
    }

    /* Check configuration file root node */
    if (strcmp(root_node->name, "log4c")) {
	sd_error("invalid root name %s", root_node->name);
	sd_domnode_delete(root_node);
	return -1;
    }
    
    /* Check configuration file revision */
    if ( (node = sd_domnode_attrs_get(root_node, "version")) != NULL)
	if (strcmp(VERSION, node->value)) {
	    sd_error("version mismatch: %s != %s", VERSION, node->value);
	    sd_domnode_delete(root_node);
	    return -1;
	}
	    
    /* backward compatibility. */
    if ( (node = sd_domnode_attrs_get(root_node, "cleanup")) != NULL) {
	sd_debug("attribute \"cleanup\" is deprecated");
	this->config.nocleanup = !atoi(node->value);
    }
			
    /* load configuration elements */
    for (i = sd_list_begin(root_node->children);
	 i != sd_list_end(root_node->children); 
	 i = sd_list_iter_next(i)) 
    {
	sd_domnode_t* node = i->data;

	if (!strcmp(node->name, "category")) category_load(this, node);
	if (!strcmp(node->name, "appender")) appender_load(this, node);
	if (!strcmp(node->name, "layout"))   layout_load(this, node);
	if (!strcmp(node->name, "config"))   config_load(this, node);
    }
	
    sd_domnode_delete(root_node);
    return 0;
}

/******************************************************************************/
extern int log4c_load(const char* a_filename)
{
    return log4c_rc_load(&__log4c_rc, a_filename);
}
