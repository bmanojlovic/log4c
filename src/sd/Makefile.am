INCLUDES = \
        -I$(top_srcdir)/src

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
        domnode-xml.h \
        domnode-xml.c \
        domnode-xml-parser.h \
	domnode-xml-parser.c \
        domnode-xml-scanner.h \
        domnode-xml-scanner.c \
        domnode.c \
        factory.h \
        factory.c \
        hash.h \
        hash.c \
        sprintf.h \
        sprintf.c \
        test.h \
        test.c
liblog4c_sd_la_LDFLAGS =

EXTRA_liblog4c_sd_la_SOURCES = \
	domnode-xml-parser.y \
	domnode-xml-scanner.l \
        sprintf.osf.c

#
# requires bison-1.35 as found on redhat-7.3
#
domnode-xml-parser.c domnode-xml-parser.h:
	bison $(srcdir)/domnode-xml-parser.y
	-mv domnode-xml-parser.[ch] $(srcdir) 2>/dev/null

#
# requires flex-2.5.27 from http://sourceforge.net/projects/lex/
#
domnode-xml-scanner.c domnode-xml-scanner.h:
	/home/legoater/work/sourceforge/flex-2.5.27/flex $(srcdir)/domnode-xml-scanner.l
	-mv domnode-xml-scanner.[ch] $(srcdir) 2>/dev/null

domnode-xml-parser.lo: domnode-xml-scanner.h
domnode-xml-scanner.lo: domnode-xml-parser.h
domnode-xml.lo: domnode-xml-scanner.c domnode-xml-parser.c
