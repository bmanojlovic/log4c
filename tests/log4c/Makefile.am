INCLUDES = \
	-I$(top_srcdir)/src \
	-DSRCDIR="\"$(srcdir)\""

noinst_PROGRAMS = test_category test_rc bench

test_category_SOURCES = \
	layout_test.c \
	appender_test.c \
	test_category.c
test_category_LDADD   = $(top_builddir)/src/log4c/liblog4c.la

test_rc_SOURCES = test_rc.c
test_rc_LDADD   = $(top_builddir)/src/log4c/liblog4c.la

bench_SOURCES = bench.c
bench_LDADD   = $(top_builddir)/src/log4c/liblog4c.la

all-local: bench.mmap

bench.mmap:
	dd if=/dev/zero of=$@ bs=1k count=64

clean-local:
	$(RM) *.out bench.mmap

