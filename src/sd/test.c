static const char version[] = "$Id$";

/*
 * Simple test framework
 * 
 * Author: Cedric Le Goater <legoater@meiosys.com>, (c) Cimai 2002
 */
#ifdef HAVE_CONFIG_H
#       include "config.h"
#endif

#include <sd/test.h>
#include <sd/malloc.h>
#include <sd/sprintf.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#ifdef HAVE_STDINT_H
#       include <stdint.h>
        typedef uint64_t usec_t;
#else
        typedef unsigned long long usec_t;
#endif

#define MAX_NFUNC 100

struct __sd_test
{
    const char*         name;
    char                out_filename[128];
    FILE*               out;
    FILE*               err;
    int                 verbose;
    int                 timed;
    sd_test_func_t**    funcs;
    int                 size;
    int			argc;
    char**		argv;
};

/******************************************************************************/
static usec_t now(void)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    
    return (usec_t) (tv.tv_sec * 1000000 + tv.tv_usec);
}

/******************************************************************************/
extern sd_test_t* sd_test_new(int a_argc, char* a_argv[])
{
    sd_test_t* this;
    char c;

    this        = sd_calloc(1, sizeof(sd_test_t));
    this->funcs = sd_calloc(MAX_NFUNC, sizeof(sd_test_func_t));
    this->name  = strstr(a_argv[0], "lt-") ? a_argv[0] + 3 : a_argv[0];

    snprintf(this->out_filename, sizeof(this->out_filename), "%s.out",
	     this->name);
    
    this->out   = fopen(this->out_filename, "w");
    this->err   = 0;
    this->verbose= 0;
    this->size  = 0;

    while ((c = getopt(a_argc, a_argv, "vt")) != EOF) {
        switch(c) {
        case 'v': this->verbose = 1; break;
        case 't': this->timed   = 1; break;
        default:                     break; 
        }
    }

    this->argc = a_argc - (optind - 1);
    this->argv = a_argv + (optind - 1);
    return this;
}

/******************************************************************************/
extern void sd_test_delete(sd_test_t* this)
{
    if (!this)
        return;

    if (this->out) fclose(this->out);
    free(this->funcs);
    free(this);
}
        
/******************************************************************************/
extern const char* sd_test_get_name(const sd_test_t* this)
{
    return this ? this->name : NULL;
}

/******************************************************************************/
extern int sd_test_get_verbose(const sd_test_t* this)
{
    return this ? this->verbose: 0;
}

/******************************************************************************/
extern int sd_test_set_verbose(sd_test_t* this, int a_verbose)
{
    if (!this)
        return 0;

    return this->verbose = a_verbose;
}

/******************************************************************************/
extern FILE* sd_test_out(sd_test_t* this)
{
    if (!this)
        return NULL;

    if (this->verbose)
        return stdout;

    return this->out ? this->out : stdout;
}

/******************************************************************************/
extern FILE* sd_test_err(sd_test_t* this)
{
    if (!this)
        return NULL;

    return this->err ? this->err : stderr;
}

/******************************************************************************/
extern int sd_test_run(sd_test_t* this, int argc, char* argv[])
{
    int i, passed = 0;

    if (!this)
        return -1;

    fprintf(sd_test_err(this), "%s: ", this->name);
    
    for (i = 0; i < this->size; i++) {
        int t;
        usec_t elapsed;

        fprintf(sd_test_out(this), "=> test #%d :\n", i);

        elapsed = now();
        t = (*this->funcs[i])(this, this->argc, this->argv);
        elapsed = now() - elapsed;

        fprintf(sd_test_out(this), "=> test #%d : %s\n", i, t ? " passed" : " failed");
        fflush(sd_test_out(this));

        passed += (t != 0);
        
        fprintf(sd_test_err(this), "%c", t ? '+' : '-');
        if (this->timed)
            fprintf(sd_test_err(this), "%lld ", elapsed);
    }
    
    fprintf(sd_test_err(this), " %d/%d %s.\n", passed, this->size,
            passed == this->size ? " passed" : " failed");
    
    return passed == this->size;
}

/******************************************************************************/
extern int sd_test_add(sd_test_t* this, sd_test_func_t a_func)
{
    if (!this)
        return -1;

    if (this->size == MAX_NFUNC)
        return this->size;

    this->funcs[this->size] = a_func;

    return this->size++;
}
