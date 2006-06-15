

LOG4C_MAJOR_VERSION=1
LOG4C_MINOR_VERSION=1
LOG4C_MICRO_VERSION=00
VERSION=$LOG4C_MAJOR_VERSION.$LOG4C_MINOR_VERSION.$LOG4C_MICRO_VERSION${SNAPSHOT:+.$SNAPSHOT}
LINK_VERSION := $LOG4C_MAJOR_VERSION.$LOG4C_MINOR_VERSION

CL=cl.exe
LINK=c:/Program\ Files/Microsoft\ Visual\ Studio/VC98/bin/link.exe
LIBPROG=lib.exe

MSVCLINKFLAGS=/DLL /VERSION:$(LOG4C_MAJOR_VERSION).$(LOG4C_MINOR_VERSION).$(LOG4C_MICRO_VERSION)

# set DEBUG=1 to set compile time debug options
DEBUG=1
ifeq ($(DEBUG),1)
MSVCFLAGS += /MDd /Yd /Zi 
# -D__SD_DEBUG__ -D__LOG4C_DEBUG__
# you also need to set SD_ERROR and SD_DEBUG env variables 
# at runtilme
MSVCLINKFLAGS +=/DEBUG
MSLIBS= MSVCRTD.LIB WINMM.LIB
else
MSVCFLAGS += /MD
MSLIBS= MSVCRT.LIB WINMM.LIB
endif

SRC_LOC=../src
INCLUDES=/I$(SRC_LOC) /I$(SRC_LOC)/sd /I$(SRC_LOC)/log4c /I.
OUT_DIR=log4c

SOURCE_SD_FILES := stack.c malloc.c \
			domnode-xml.c domnode-xml-parser.c domnode-xml-scanner.c \
			domnode.c \
            factory.c hash.c list.c \
            sprintf.c test.c sd_xplatform.c error.c

SD_OBJS :=  $(patsubst %.c,%.obj,$(SOURCE_SD_FILES))
SD_TARGET_OBJS := $(addprefix $(OUT_DIR)/sd/, $(SD_OBJS))

all: $(OUT_DIR)/sd $(OUT_DIR)/sd/liblog4c_sd.lib \
	$(OUT_DIR)/log4c $(OUT_DIR)/log4c/liblog4c.dll \
	$(OUT_DIR)/tests $(OUT_DIR)/tests/test_category.exe \
	$(OUT_DIR)/tests/test_rc.exe \
	$(OUT_DIR)/tests/bench.exe $(OUT_DIR)/tests/test_stream2.exe \
	examples

$(OUT_DIR)/sd/%.obj:$(SRC_LOC)/sd/%.c
	$(CL) $(INCLUDES) /DLOG4C_EXPORTS /c $(MSVCFLAGS) /Fo$@ $<

$(OUT_DIR)/sd/liblog4c_sd.lib: $(SD_TARGET_OBJS)
	$(LIBPROG) /OUT:$@ $^

$(OUT_DIR)/sd:
	mkdir -p $@

$(OUT_DIR)/tests:
	mkdir -p $@

$(OUT_DIR)/log4c:
	mkdir -p  $@

$(OUT_DIR)/examples:
	mkdir -p  $@

SOURCE_LOG4C_FILES = rc.c init.c appender_type_stream.c \
	appender_type_stream2.c layout_type_basic.c layout_type_dated.c \
	version.c logging_event.c priority.c appender.c layout.c \
	category.c

LOG4C_OBJS :=  $(patsubst %.c,%.obj,$(SOURCE_LOG4C_FILES))
LOG4C_TARGET_OBJS := $(addprefix $(OUT_DIR)/log4c/, $(LOG4C_OBJS))

$(OUT_DIR)/log4c/%.obj:$(SRC_LOC)/log4c/%.c
	$(CL) $(INCLUDES) /DLOG4C_EXPORTS /c $(MSVCFLAGS)\
	 /DVERSION="\"LOG4C_MAJOR_VERSION.$LOG4C_MINOR_VERSION\"" \
	/DLOG4C_RCPATH="\".\"" \
	$ /Fo$@ $<

$(OUT_DIR)/log4c/liblog4c.dll: $(LOG4C_TARGET_OBJS) \
								$(OUT_DIR)/sd/liblog4c_sd.lib
	$(LINK) $(MSVCLINKFLAGS) /OUT:$@ /IMPLIB:$(basename $@).lib \
		/EXPORT:sd_optind /EXPORT:sd_getopt /EXPORT:sd_gettimeofday \
		/EXPORT:sd_test_delete /EXPORT:sd_test_run /EXPORT:sd_test_add \
		/EXPORT:sd_test_new /EXPORT:sd_test_out /EXPORT:sd_test_get_name \
		/EXPORT:sd_factory_print /EXPORT:sd_debug_fn /EXPORT:sd_error_fn \
		/LIBPATH:sd $^ $(MSLIBS)

#	
#############
# tests
#############
TEST_LOC := ../tests/log4c
test_category_SOURCES = test_category.c layout_test.c appender_test.c
$(OUT_DIR)/tests/test_category.exe: $(addprefix $(TEST_LOC)/, $(test_category_SOURCES)) \
	$(OUT_DIR)/log4c/liblog4c.lib
	$(CL) $(INCLUDES) $(MSVCFLAGS) $(filter %.c,$^) /link \
	$(filter %.lib,$^)
	mv *.obj $(OUT_DIR)/tests; mv *.exe $(OUT_DIR)/tests

test_rc_SOURCES = test_rc.c
$(OUT_DIR)/tests/test_rc.exe: $(addprefix $(TEST_LOC)/, $(test_rc_SOURCES)) \
	$(OUT_DIR)/log4c/liblog4c.lib
	$(CL) $(INCLUDES) /DSRCDIR="\"$(TEST_LOC)\"" $(MSVCFLAGS) $(filter %.c,$^) /link \
	$(filter %.lib,$^)
	mv *.obj $(OUT_DIR)/tests; mv *.exe $(OUT_DIR)/tests

test_stream2_SOURCES = test_stream2.c
$(OUT_DIR)/tests/test_stream2.exe: $(addprefix $(TEST_LOC)/, $(test_stream2_SOURCES)) \
	$(OUT_DIR)/log4c/liblog4c.lib
	$(CL) $(INCLUDES) $(MSVCFLAGS) $(filter %.c,$^) /link \
	$(filter %.lib,$^)
	mv *.obj $(OUT_DIR)/tests; mv *.exe $(OUT_DIR)/tests

bench_SOURCES = bench.c
$(OUT_DIR)/tests/bench.exe: $(addprefix $(TEST_LOC)/, $(bench_SOURCES)) \
	$(OUT_DIR)/log4c/liblog4c.lib
	$(CL) $(INCLUDES) $(MSVCFLAGS) $(filter %.c,$^) /link \
	$(filter %.lib,$^)
	mv *.obj $(OUT_DIR)/tests; mv *.exe $(OUT_DIR)/tests

#
#############
# examples
######### ####

examples: $(OUT_DIR)/examples $(OUT_DIR)/examples/application_1.exe \
	$(OUT_DIR)/examples/libexamples.lib $(OUT_DIR)/examples/application_2.exe

EXAMPLES_LOC = ../examples

application_1_SOURCES = application_1.c
$(OUT_DIR)/examples/application_1.exe: $(addprefix $(EXAMPLES_LOC)/, $(application_1_SOURCES)) \
	$(OUT_DIR)/log4c/liblog4c.lib
	$(CL) $(INCLUDES) $(MSVCFLAGS) $(filter %.c,$^) /link \
	$(filter %.lib,$^) WINMM.LIB
	mv *.obj $(OUT_DIR)/examples; mv *.exe $(OUT_DIR)/examples
ifeq ($(DEBUG),1)
	mv *.pdb *.ilk $(OUT_DIR)/examples
endif

EXAMPLE_LIB_SOURCE_FILES := example_formatters.c example_appenders.c
EXAMPLE_LIB_OBJS :=  $(patsubst %.c,%.obj,$(EXAMPLE_LIB_SOURCE_FILES))
EXAMPLE_LIB_TARGET_OBJS := $(addprefix $(OUT_DIR)/examples/, $(EXAMPLE_LIB_OBJS))

$(OUT_DIR)/examples/%.obj: $(EXAMPLES_LOC)/%.c
	$(CL) $(INCLUDES) /c $(MSVCFLAGS) /Fo$@ $<

$(OUT_DIR)/examples/libexamples.lib: $(EXAMPLE_LIB_TARGET_OBJS)
	$(LIBPROG) /OUT:$@ $^ /EXPORT:init_examples_lib

application_2_SOURCES = application_2.c
$(OUT_DIR)/examples/application_2.exe: $(addprefix $(EXAMPLES_LOC)/, $(application_2_SOURCES)) \
    $(OUT_DIR)/examples/libexamples.lib $(OUT_DIR)/log4c/liblog4c.lib
	$(CL) $(INCLUDES) $(MSVCFLAGS) $(filter %.c,$^) /link \
	$(filter %.lib,$^) WINMM.LIB
	mv *.obj $(OUT_DIR)/examples; mv *.exe $(OUT_DIR)/examples
ifeq ($(DEBUG),1)
	mv *.pdb *.ilk $(OUT_DIR)/examples
endif

clean_examples:
	rm -rf $(OUT_DIR)/examples

clean: clean_examples
	rm -rf $(OUT_DIR)/sd $(OUT_DIR)/log4c $(OUT_DIR)/tests
