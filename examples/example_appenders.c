/******************************************************************************
 * 
 * Part of the log4c examples.
 *
 * Along with example_formatters.c this file is used to create a small
 * library of custom appenders and formatters.
 *
 * This library is excercised using application_2 and a sample log4crc 
 * config file.
 *
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif
#include <time.h>

#include <log4c.h>

/* global var to force linking with the appender when using syslog */
static int log4c_to_syslog_priority(int a_priority);

/*
 * This one is experimental and not in use.  But its a good example to see all the things
 * you have access to during the call to the appender
 */

static int s13_file_append(log4c_appender_t* this, 
			   const log4c_logging_event_t* a_event)
{
    FILE* fp = log4c_appender_get_udata(this);

    /*
      return fprintf(fp, "[%s] [%s] [%d] [%d] [%s] [%s] [%d]\n%s", 
      log4c_appender_get_name(this),
      a_event->evt_category,
      a_event->evt_priority,
      a_event->evt_timestamp.tv_sec*1000000 + a_event->evt_timestamp.tv_usec,
      a_event->evt_msg, 
      a_event->evt_loc->loc_file,
      a_event->evt_loc->loc_line,
      a_event->evt_rendered_msg);
    */

    return fprintf(fp, "%s\n", 
		   a_event->evt_rendered_msg   
	);
}

/*******************************************************************************/
static int etf_open(log4c_appender_t* this)
{
    FILE* fp = log4c_appender_get_udata(this);
    
    if (fp)
	return 0;
    
    if ( (fp = fopen(log4c_appender_get_name(this), "a+")) == NULL)
	fp = stderr;
    
    /* unbuffered mode */
    setbuf(fp, NULL);
    
    log4c_appender_set_udata(this, fp);
    return 0;
}


/*******************************************************************************/
static int etf_close(log4c_appender_t* this)
{
    FILE* fp = log4c_appender_get_udata(this);
    
    return (fp ? fclose(fp) : 0);
}
#ifdef __GNUC__
const log4c_appender_type_t log4c_appender_type_s13_file = {
    name:	"s13_file",
    open:	etf_open,
    append:	s13_file_append,
    close:	etf_close,
};

log4c_appender_type_define(log4c_appender_type_s13_file);
#else
  log4c_appender_type_t log4c_appender_type_s13_file;
#endif


/*******************************************************************************/
/*
 * The log4c stderr adds a "[stderr] " in front of the message
 * it is logging.  this one doesn't, and  leaves the formatting
 * descisions up to the formatter
 */
static int s13_stderr_append(log4c_appender_t* this, 
			     const log4c_logging_event_t* a_event)
{
    return fprintf(stderr, "%s\n", 
		   a_event->evt_rendered_msg   
	);
}
static int s13_stderr_open(log4c_appender_t* this)
{
    /* fprintf (stderr,"running s13_stderr appender open command now\n"); */
    return 0;
}
#ifdef __GNUC__
const log4c_appender_type_t log4c_appender_type_s13_stderr = {
    name:	"s13_stderr",
    open:	s13_stderr_open,
    append:	s13_stderr_append,
    close:	NULL,
};

log4c_appender_type_define(log4c_appender_type_s13_stderr);
#else
    log4c_appender_type_t log4c_appender_type_s13_stderr;
#endif


#ifndef _WIN32
/**************************/
/*    User appender    */
/**************************/
/*******************************************************************************/
static int syslog_user_open(log4c_appender_t* this)
{
    openlog(log4c_appender_get_name(this), LOG_PID, LOG_USER);
    return 0;
}

/*******************************************************************************/
static int syslog_user_append(log4c_appender_t*	this, 
			      const log4c_logging_event_t* a_event)
{

    syslog(log4c_to_syslog_priority(a_event->evt_priority) | LOG_USER,
	   "%s", a_event->evt_rendered_msg); 
    return 0;
}

/*******************************************************************************/
static int syslog_user_close(log4c_appender_t*	this)
{
    closelog();
    return 0;
}

/*******************************************************************************/

#ifdef __GNUC__
const log4c_appender_type_t log4c_appender_type_syslog_user = {
    name:	"syslog_user",
    open:	syslog_user_open,
    append:	syslog_user_append,
    close:	syslog_user_close,
};
log4c_appender_type_define(log4c_appender_type_syslog_user);
#else
   log4c_appender_type_t log4c_appender_type_syslog_user;
#endif


/**************************/
/*    Local 0 appender    */
/**************************/
/*******************************************************************************/
static int syslog_local0_open(log4c_appender_t* this)
{
    openlog(log4c_appender_get_name(this), LOG_PID, LOG_LOCAL0);
    return 0;
}

/*******************************************************************************/
static int syslog_local0_append(log4c_appender_t*	this, 
				const log4c_logging_event_t* a_event)
{

    syslog(log4c_to_syslog_priority(a_event->evt_priority) | LOG_LOCAL0,
	   "%s", a_event->evt_rendered_msg);
    return 0;
}

/*******************************************************************************/
static int syslog_local0_close(log4c_appender_t*	this)
{
    closelog();
    return 0;
}

/*******************************************************************************/
#ifdef __GNUC__
const log4c_appender_type_t log4c_appender_type_syslog_local0 = {
    name:	"syslog_local0",
    open:	syslog_local0_open,
    append:	syslog_local0_append,
    close:	syslog_local0_close,
};
log4c_appender_type_define(log4c_appender_type_syslog_local0);
#else
    log4c_appender_type_t log4c_appender_type_syslog_local0; 
#endif


static int log4c_to_syslog_priority(int a_priority)
{
    static int priorities[] = {
        LOG_EMERG,
        LOG_ALERT,
        LOG_CRIT,
        LOG_ERR,
        LOG_WARNING,
        LOG_NOTICE,
        LOG_INFO,
        LOG_DEBUG
    };
    int result;

    a_priority++;
    a_priority /= 100;

    if (a_priority < 0) {
        result = LOG_EMERG;
    } else if (a_priority > 7) {
        result = LOG_DEBUG;
    } else {
        result = priorities[a_priority];
    }

    return result;
}


#endif /* _WIN32 */

/*****************************************************/

#ifndef __GNUC__
int init_example_appenders(){

	int rc = 0;

#ifndef _WIN32	
	log4c_appender_type_syslog_local0.name = "syslog_local0";
	log4c_appender_type_syslog_local0.open = syslog_local0_open;
	log4c_appender_type_syslog_local0.append = syslog_local0_append;
	log4c_appender_type_syslog_local0.close = syslog_local0_close;
	log4c_appender_type_set(&log4c_appender_type_syslog_local0);
	 
	
	log4c_appender_type_syslog_user.name = "syslog_user";
	log4c_appender_type_syslog_user.open = syslog_user_open;
	log4c_appender_type_syslog_user.append = syslog_user_append;
	log4c_appender_type_syslog_user.close = syslog_user_close;
	log4c_appender_type_set(&log4c_appender_type_syslog_user);
#else
	log4c_appender_type_s13_file.name = "s13_file";
	log4c_appender_type_s13_file.open = etf_open;
	log4c_appender_type_s13_file.append = s13_file_append;
	log4c_appender_type_s13_file.close = etf_close;
	log4c_appender_type_set(&log4c_appender_type_s13_file);
			
	log4c_appender_type_s13_stderr.name = "s13_stderr";
	log4c_appender_type_s13_stderr.open = s13_stderr_open;
	log4c_appender_type_s13_stderr.append = s13_stderr_append;
	log4c_appender_type_s13_stderr.close = NULL;
	log4c_appender_type_set(&log4c_appender_type_s13_stderr);
#endif
	return(rc);			
}
#endif
