/******************************************************************************
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <log4c.h>


/**********************************************************************
 *
 * Formatted to put [category] out at the front of the message
 *
 **********************************************************************/
static const char* cat_format(
    const log4c_layout_t*       a_layout,
    const log4c_logging_event_t*a_event)
{
    static char buffer[4096];

    /*
     * For this formatter we put the category up front in the log message
     */
    sprintf(buffer, "[%s] %s", a_event->evt_category,  a_event->evt_msg);

    return buffer;
}
const log4c_layout_type_t log4c_layout_type_cat  = {
    name:       "s13_cat",
    format:     cat_format,
};

log4c_layout_type_define(log4c_layout_type_cat);

static const char* none_format(
    const log4c_layout_t*       a_layout,
    const log4c_logging_event_t*a_event)
{
    static char buffer[4096];
    return buffer;
}
const log4c_layout_type_t log4c_layout_type_none  = {
    name:       "s13_none",
    format:     none_format,
};



/**********************************************************************/
/*
 * Formatted to mock up an xml format.
 *
 **********************************************************************/
static const char* xml_format(
    const log4c_layout_t*       a_layout,
    const log4c_logging_event_t*a_event)
{
    static char buffer[4096];

    /*
     * For this formatter we put the category up front in the log message
     */
    sprintf(buffer, "<logmessage><category>%s</category><message>%s</message></logmessage>", a_event->evt_category,  a_event->evt_msg);

    return buffer;
}
const log4c_layout_type_t log4c_layout_type_xml  = {
    name:       "s13_xml",
    format:     xml_format,
};

log4c_layout_type_define(log4c_layout_type_xml);

