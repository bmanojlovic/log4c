
AM_CPPFLAGS = -I${top_srcdir}/src

EXTRA_DIST=log4crc README

lib_LTLIBRARIES = liblog4c_examples.la
bin_PROGRAMS = helloworld helloworld1 application_1 application_2 

liblog4c_examples_la_SOURCES = example_formatters.c example_appenders.c

application_1_SOURCES = application_1.c
application_2_SOURCES = application_2.c
helloworld_SOURCES = helloworld/helloworld.c
helloworld1_SOURCES = helloworld1/helloworld1.c

helloworld_LDADD = \
	$(top_builddir)/src/log4c/liblog4c.la
helloworld1_LDADD = \
	$(top_builddir)/src/log4c/liblog4c.la
application_1_LDADD = \
	$(top_builddir)/src/log4c/liblog4c.la
application_2_LDADD = \
	$(top_builddir)/examples/liblog4c_examples.la \
	$(top_builddir)/src/log4c/liblog4c.la

