#include <log4c/appender.h>
#include <log4c/category.h>
#include <sd/malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef long long int usec_t;

static inline usec_t utime(void)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    
    return (usec_t) (tv.tv_sec * 1000000 + tv.tv_usec);
}

#define timed_loop(name, count, expr) \
{ \
    int i; \
    usec_t u = utime(); \
    for (i = 0; i < count; i++) (expr); \
    u -= utime(); \
    fprintf(stderr, name ": elapsed %lld us - average %lld us\n", -u, - u / count); \
} 

log4c_category_define(mmap, "mmap");
log4c_category_define(stream, "stream");

int main(int argc, char* argv[])
{
    int    count  = argc > 1 ? atoi(argv[1]) : 100;
    size_t size   = argc > 2 ? atoi(argv[2]) : 128;    
    char* buffer  = NULL;
    log4c_appender_t* mmap_appender = log4c_appender_get("bench.mmap");
    log4c_appender_t* stream_appender = log4c_appender_get("stdout");
 
    log4c_appender_set_type(mmap_appender, log4c_appender_type_get("mmap"));
    log4c_category_set_appender(mmap, mmap_appender);
    log4c_category_set_appender(stream, stream_appender);
    log4c_category_set_priority(log4c_category_get("root"), LOG4C_PRIORITY_ERROR);

    buffer = (char*) sd_malloc(size * sizeof(char));    
    memset(buffer, 'X', size);
    buffer[size - 1] = '\0';

    fprintf(stderr, "   loop: %d\n", count);
    fprintf(stderr, " buffer: %d bytes\n\n", size);
    
    timed_loop("stream",  count, (log4c_category_error(stream, "%s", buffer)));
    timed_loop("mmap",    count, (log4c_category_error(mmap, "%s", buffer)));
    timed_loop("fprintf", count, (fprintf(stdout, "%s\n", buffer)));

    return 0;
}

