static const char version[] = "$Id$";

/*
 * test_category.c
 *
 * Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */

#include <log4c/appender.h>
#include <log4c/layout.h>
#include <log4c/category.h>
#include <sd/test.h>
#include <sd/factory.h>
#include <stdio.h>

log4c_category_define(root, "root");
log4c_category_define(sub1, "sub1");
log4c_category_define(sun1sub2, "sub1.sub2");

/******************************************************************************/
static void log4c_print(FILE* a_fp)
{   
    extern sd_factory_t* log4c_category_factory;
    extern sd_factory_t* log4c_appender_factory;
    extern sd_factory_t* log4c_layout_factory;

    sd_factory_print(log4c_category_factory, a_fp);	fprintf(a_fp, "\n");
    sd_factory_print(log4c_appender_factory, a_fp);	fprintf(a_fp, "\n");
    sd_factory_print(log4c_layout_factory, a_fp);	fprintf(a_fp, "\n");
}

/******************************************************************************/
static int test0(sd_test_t* a_test, int argc, char* argv[])
{   
    log4c_print(sd_test_out(a_test));
    return 1;
}

/******************************************************************************/
static int test00(sd_test_t* a_test, int argc, char* argv[])
{      
    int p;
    log4c_category_t*	cat = log4c_category_get("a category");

    for (p = 0; p < LOG4C_PRIORITY_UNKNOWN; p++)
	log4c_category_log(cat, p * 100, "this is a %s event", 
			   log4c_priority_to_string(p * 100));
    return 1;
}

/******************************************************************************/
static int test1(sd_test_t* a_test, int argc, char* argv[])
{   
    log4c_layout_t*   layout1   = log4c_layout_get("layout1");
    log4c_layout_t*   layout2   = log4c_layout_get("layout2");
    log4c_appender_t* appender  = log4c_appender_get(sd_test_get_name(a_test));
    log4c_appender_t* appender1 = log4c_appender_get("appender1");
    log4c_appender_t* appender2 = log4c_appender_get("appender2");

    log4c_layout_set_type(layout1, log4c_layout_type_get("test"));
    log4c_layout_set_type(layout2, log4c_layout_type_get("test"));

    log4c_appender_set_udata(appender, sd_test_out(a_test));

    log4c_appender_set_type(appender1,   log4c_appender_type_get("test"));
    log4c_appender_set_layout(appender1, layout1);
    log4c_appender_set_udata(appender1,  sd_test_out(a_test));

    log4c_appender_set_type(appender2,   log4c_appender_type_get("test"));
    log4c_appender_set_layout(appender2, layout2);
    log4c_appender_set_udata(appender2,  sd_test_out(a_test));

    log4c_category_set_appender(root, appender);
    log4c_category_set_appender(sub1, appender1);
    log4c_category_set_appender(sun1sub2, appender2);

    log4c_category_set_priority(root, LOG4C_PRIORITY_ERROR);

    log4c_print(sd_test_out(a_test));
    return 1;
}

#define foo(cat, level) \
{ \
    fprintf(sd_test_out(a_test), "\n# "#cat" "#level" (priority = %s)\n", \
	    log4c_priority_to_string(log4c_category_get_priority(cat))); \
    log4c_category_##level(cat, #cat" "#level); \
}

/******************************************************************************/
static int test2(sd_test_t* a_test, int argc, char* argv[])
{   
    foo(root, error);
    foo(root, warn);
    foo(sub1, error);
    foo(sub1, warn);
    foo(sun1sub2, error);
    foo(sun1sub2, warn);

    return 1;
}

/******************************************************************************/
static int test3(sd_test_t* a_test, int argc, char* argv[])
{   
    log4c_category_set_priority(sub1, LOG4C_PRIORITY_INFO);

    foo(root, error);
    foo(root, warn);
    foo(sub1, error);
    foo(sub1, warn);
    foo(sun1sub2, error);
    foo(sun1sub2, warn);

    return 1;
}

/******************************************************************************/
static int test4(sd_test_t* a_test, int argc, char* argv[])
{   
    log4c_category_set_priority(root, LOG4C_PRIORITY_TRACE);

    foo(root, info);
    foo(root, warn);
    foo(sub1, info);
    foo(sub1, warn);
    foo(sun1sub2, info);
    foo(sun1sub2, warn);
    return 1;
}

/******************************************************************************/
int main(int argc, char* argv[])
{    
    int ret;
    sd_test_t* t = sd_test_new(argc, argv);

    sd_test_add(t, test0);
    sd_test_add(t, test00);
    sd_test_add(t, test1);
    sd_test_add(t, test2);
    sd_test_add(t, test3);
    sd_test_add(t, test4);

    ret = sd_test_run(t, argc, argv);

    sd_test_delete(t);

    return ! ret;
}
