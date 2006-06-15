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
#ifdef __GNUC__
const log4c_layout_type_t log4c_layout_type_cat  = {
    name:       "s13_cat",
    format:     cat_format,
};

log4c_layout_type_define(log4c_layout_type_cat);

#else
log4c_layout_type_t log4c_layout_type_cat;
#endif

static const char* none_format(
    const log4c_layout_t*       a_layout,
    const log4c_logging_event_t*a_event)
{
    static char buffer[4096];
    return buffer;
}
#ifdef __GNUC__
const log4c_layout_type_t log4c_layout_type_none  = {
    name:       "s13_none",
    format:     none_format,
};
#else
log4c_layout_type_t log4c_layout_type_none;
#endif



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
#ifdef __GNUC__
const log4c_layout_type_t log4c_layout_type_xml = {
    name:       "s13_xml",
    format:     xml_format,
};
log4c_layout_type_define(log4c_layout_type_xml);
#else
log4c_layout_type_t log4c_layout_type_xml;
#endif


/*****************************/
/*
 * Here provide an init routine for this lib 
 *
******************************/


int init_example_formatters(){

	int rc = 0;


	log4c_layout_type_set(&log4c_layout_type_xml);
	log4c_layout_type_xml.name = "s13_xml";
	log4c_layout_type_xml.format = xml_format;

	log4c_layout_type_set(&log4c_layout_type_none);
	log4c_layout_type_xml.name = "s13_none";
	log4c_layout_type_xml.format = none_format;


	log4c_layout_type_set(&log4c_layout_type_cat);
	log4c_layout_type_xml.name = "s13_cat";
	log4c_layout_type_xml.format = cat_format;

	return(rc);

}


int init_examples_lib() {

	init_example_formatters();
	init_example_appenders();

}