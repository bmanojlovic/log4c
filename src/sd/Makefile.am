INCLUDES = \
        -I$(top_srcdir)/src \
	@EXPAT_CFLAGS@

noinst_LTLIBRARIES  = liblog4c_sd.la

liblog4c_sd_la_SOURCES = \
        error.h \
        defs.h \
        stack.h \
        stack.c \
        list.h \
        list.c \
        malloc.h \
        malloc.c \
        domnode.h \
        domnode.c \
        factory.h \
        factory.c \
        hash.h \
        hash.c \
        sprintf.h \
        sprintf.c \
        test.h \
        test.c
liblog4c_sd_la_LDFLAGS = @EXPAT_LIBS@

EXTRA_liblog4c_sd_la_SOURCES = \
        sprintf.osf.c

