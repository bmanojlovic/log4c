/*
 * This is one of the log4c example programs
 *
 * In this example, everything is the same as application 1 EXCEPT THE
 * LINKING so here we link against a shared library that has
 * additional formatters and appenders.  This shows how easy it is to
 * add in more appenders and formatters to the log4c framnework
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "log4c.h"

int main(int argc, char** argv)
{
    struct timeval start_time;
    struct timeval now_time;
    int looptime = 0;
    log4c_category_t* mycat = NULL;
  
    if (argc < 2)
    {
	printf("usage: %s loop_time_in_seconds\n",argv[0]);
	exit (1);
    }
    if (sscanf(argv[1],"%d",&looptime) != 1)
    {
	printf("could not convert %s to number of seconds to loop\n",argv[1]);
	exit(1);
    }
 
    /*
     * You could choose to wrap hte log4c_category_log with some macro
     * that then calls an accessor to get your pre-created category
     * mycat and then logs to it.  But for now we just focus on the fact
     * that we are using log4crc to have this application use the new
     * formatters and appenders we wrote as examples
     */
    log4c_init();
    mycat = log4c_category_get("six13log.log.app.application2");

    gettimeofday(&start_time, NULL);
    gettimeofday(&now_time, NULL);
  
    while ( (now_time.tv_sec - start_time.tv_sec) < looptime)
    {
	log4c_category_log(mycat, LOG4C_PRIORITY_DEBUG, "Debugging app 2");	      
	log4c_category_log(mycat, LOG4C_PRIORITY_ERROR,
			   "some error from app2 at line %d in file %s",
			   __LINE__, __FILE__);
	sleep(3);

	gettimeofday(&now_time, NULL);
    }
    return 0;
}
