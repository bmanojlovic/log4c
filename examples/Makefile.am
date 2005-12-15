
AM_CPPFLAGS = -I${top_srcdir}/src -I${top_srcdir}/src/log4c

EXTRA_DIST=log4crc readme

lib_LTLIBRARIES = liblog4c_examples.la
bin_PROGRAMS = application_1 application_2 

liblog4c_examples_la_SOURCES = example_formatters.c example_appenders.c

application_1_SOURCES = application_1.c
application_2_SOURCES = application_2.c

application_1_LDADD = ../src/log4c/liblog4c.la
application_2_LDADD = liblog4c_examples.la ../src/log4c/liblog4c.la

