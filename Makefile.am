if DOC
DOC_SUBDIR = doc
endif
if TEST
TEST_SUBDIR = tests
endif

SUBDIRS = config src ${DOC_SUBDIR} ${TEST_SUBDIR}

DIST_SUBDIRS = config src doc tests 

EXTRA_DIST = \
	log4c.m4 \
	log4c.spec.in \
	log4c.spec

bin_SCRIPTS  = log4c-config
BUILT_SOURCES=log4c-config

m4datadir = $(datadir)/aclocal
m4data_DATA = log4c.m4

sysconf_DATA = log4crc

rpm: dist
	rpmbuild -ta ${distdir}.tar.gz
