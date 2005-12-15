#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <log4c/appender.h>
#include <log4c/category.h>
#include <log4c/appender_type_stream2.h>
#include <sd/malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <sd/sd_xplatform.h>

/******************************************************************************/

typedef XP_UINT64 usec_t;
static usec_t utime(void)
{
    struct timeval tv;
    
    SD_GETTIMEOFDAY(&tv, NULL);
    return (usec_t) (tv.tv_sec * 1000000 + tv.tv_usec);
}

/******************************************************************************/
#define MSG_SIZE 128
#define NUM_MSGS 16

#define timed_loop(name, confstr, count, expr) \
{ \
    int i; \
    usec_t u = utime(); \
    for (i = 0; i < count; i++) (expr); \
    u -= utime(); \
    fprintf(stderr, name ": %s elapsed %lld us - average %lld us\n", \
confstr, -u, - u / count); \
}

#define USAGE  "Usage: bench [-n] [-h] <num msgs> [<msg size>]\n\n" \
"This program runs a sequence of timed calls to log4c logging calls.\n" \
"This allows, for example, the performance of different appenders\n" \
"or the performance of a given appender with different options to be\n" \
"compared.\n\n" \
"The results are sent to stderr so you can collect them in a file by doing\n" \
"something like this: './bench 128 2> results.txt'\n\n" \
"For covenience, we allow the appenders that write to the screen to be\n" \
"turned off using the -n option.  By default they are run.\n\n" \
"The default msg size is 128.\n\n" \
"-n  do not run the timed tests that write to the screen\n" \
"-h  display this help message\n"

/******************************************************************************/
int g_doall = 0;
int g_noscreen_appenders = 0;
long g_num_msgs = NUM_MSGS;
long g_msgsize = MSG_SIZE;

#ifdef __GNUC__
log4c_category_define(mmap, "mmap");
log4c_category_define(stream, "stream");
log4c_category_define(catstream2, "stream2_file");
log4c_category_define(catstream_file, "stream_file");
#else
static log4c_category_t* mmap = NULL;
static log4c_category_t* stream = NULL;
static log4c_category_t* catstream2 = NULL;
static log4c_category_t* catstream_file = NULL;
#endif

/******************************************************************************/
void getopts(int argc, char **argv){

  char c;

  if ( argc == 1) {
      fprintf(stderr,USAGE);
      exit(1);
  }  

   while ((c = SD_GETOPT(argc, argv, "nh")) != -1) {
    switch(c) {   
    case 'n':
      g_noscreen_appenders = 1;
      break;
    case 'h': 
      fprintf(stderr, USAGE);
      exit(1);
      break;
    }
   }    
   
   /* Pick up the number of msgs and the size */
   if ( SD_OPTIND < argc ){
       g_num_msgs = atol(argv[SD_OPTIND]);
       if ( SD_OPTIND+1 < argc ){
	    g_msgsize =  atol(argv[SD_OPTIND+1]);
       }
   }

   fprintf(stderr, "  Writing %ld message(s) of length %ld\n",
	  g_num_msgs,g_msgsize);
   if ( g_noscreen_appenders){
     fprintf(stderr, "  Not running tests that log to the screen\n\n");
   }

}

int main(int argc, char* argv[]){
    int flags = 0; 
    char* buffer  = NULL;
    log4c_appender_t* mmap_appender = log4c_appender_get("bench.mmap");
    log4c_appender_t* stream_appender = log4c_appender_get("stdout");
    log4c_appender_t* stream2_appender = log4c_appender_get("bench.stream2");
    log4c_appender_t* streamfile_appender =
	log4c_appender_get("bench.stream");
#ifndef __GNUC__
    mmap = log4c_category_get("mmap");
    stream = log4c_category_get("stream");
    catstream2 = log4c_category_get("stream2");
    catstream_file = log4c_category_get("stream_file");
#endif
 
    getopts(argc, argv);

    log4c_appender_set_type(mmap_appender, log4c_appender_type_get("mmap"));
    log4c_category_set_appender(mmap, mmap_appender);
    log4c_category_set_appender(stream, stream_appender);
    /* catstream2 configure */   
    log4c_appender_set_type(stream2_appender,
			    log4c_appender_type_get("stream2"));
    log4c_category_set_appender(catstream2, stream2_appender);
    /**/
     log4c_category_set_appender(catstream_file, streamfile_appender);
    
    log4c_category_set_priority(log4c_category_get("root"),
				LOG4C_PRIORITY_ERROR);

    buffer = (char*) malloc(g_msgsize * sizeof(char));    
    memset(buffer, 'X', g_msgsize);
    buffer[g_msgsize - 1] = '\0';       
   
    /* logs that write to the screen, if required */
    if ( !g_noscreen_appenders){
	timed_loop("fprintf", "", g_num_msgs, 
		   (fprintf(stdout, "%s\n", buffer)));  

	timed_loop("stream",  "", g_num_msgs,
		   (log4c_category_error(stream, "%s", buffer)));
    }
    /* Logs that write to files */
    timed_loop("mmap",    "", g_num_msgs,
		   (log4c_category_error(mmap, "%s", buffer)));    
    timed_loop("stream_file",  "", g_num_msgs, 
	       (log4c_category_error(catstream_file, "%s", buffer)));
    
    /* stream2 appender--once in buffered mode and again unbuffered 
     * to compare 
     */
    log4c_appender_open(stream2_appender);
    flags = log4c_stream2_get_flags(stream2_appender);  
    timed_loop("stream2",  
	      (flags & LOG4C_STREAM2_UNBUFFERED ? "unbuffered -":"buffered -"),
	       g_num_msgs, 
	       (log4c_category_error(catstream2, "%s", buffer)));   
    log4c_appender_close(stream2_appender);    

    log4c_stream2_set_flags(stream2_appender, LOG4C_STREAM2_UNBUFFERED);
    flags = log4c_stream2_get_flags(stream2_appender);    
    timed_loop("stream2", 
	      (flags & LOG4C_STREAM2_UNBUFFERED ? "unbuffered -":"buffered -"),
	       g_num_msgs, 
	       (log4c_category_error(catstream2, "%s", buffer)));    
    return 0;
}

