
# Sample Makefile for compiling log4c dll, test programs and examples
# See the README in this directory for more details.

###############################################################
# TWEAKABLES: tweak the parameters in this section as required,
# you should not need to change the rest of the file in normal
# usage
# 
LOG4C_MAJOR_VERSION:=1
LOG4C_MINOR_VERSION:=1
LOG4C_MICRO_VERSION:=0

# We have tested log4c with msvc6 and with VC Express
VC_LOCATION=c:/Program\ Files/Microsoft\ Visual\ Studio/VC98
#VC_LOCATION=c:/Program\ Files/Microsoft\ Visual\ Studio\ 8/VC
SDK_LOCATION=c:/Program\ Files/Microsoft\ Platform\ SDK

# set DEBUG=1 to set compile time debug options and to link
# with msvcrtd.dll.  Note that msvcrtd.dll is not typically
# installed on Production systems so if you distribute a debug
# version your users will need to install msvcrtd.dll
DEBUG=1

# Set USE_EXPAT to 1 to use expat to parse the xml conf file.
# Otherwise a hard coded lex//yacc is used--it works but, uggh!
USE_EXPAT=0
EXPAT_LOCATION=C:/Expat-2.0.0

# END TWEAKABLES 
#############################################################


###########################################
# Construct version number to be built into
# the source
###########################################
LINK_VERSION := $(LOG4C_MAJOR_VERSION).$(LOG4C_MINOR_VERSION).$(LOG4C_MICRO_VERSION)

########################
# Define the Build Tools
########################
CL=$(VC_LOCATION)/bin/cl.exe
LINK=$(VC_LOCATION)/bin/link.exe
LIBPROG=$(VC_LOCATION)/bin/lib.exe

#############################
# Set the Compiler/Link flags
#############################
# /NOLGO -- suppress banner display when cl.exe starts (quicker)
# /W3 -- good level of warning (default is 2)
MSVCFLAGS = /nologo /DHAVE_STDARG /DWITH_ROLLINGFILE /W3
MSVCLINKFLAGS=/NOLOGO /DLL /VERSION:$(LINK_VERSION)

# MS Visual Studio 8 Express: need to pick up the SDK seperately
MSINCLUDES=$(SDK_LOCATION)/Include
MS_SDKLIB_PATH=$(SDK_LOCATION)/Lib

ifeq ($(DEBUG),1)
# options are as follows:
# /MDd -- mutlithread debug DLL
# /Od -- disable optimization, easier debugging
# /Zi -- put debug info into a pdb file
# /GZ -- makes it more likely to catch memory corruption bugs
MSVCFLAGS += /MDd /Od /Zi /GZ /D__SD_DEBUG__ /D__LOG4C_DEBUG__
# To see internal log4c traces remember you also need to set
# SD_ERROR and SD_DEBUG env variables # at runtime
MSVCLINKFLAGS +=/DEBUG
MSLIBS= MSVCRTD.LIB
else
MSVCFLAGS += /MD
MSLIBS= MSVCRT.LIB
endif

SRC_LOC=../src
INCLUDES=/I$(SRC_LOC) /I$(SRC_LOC)/sd /I$(SRC_LOC)/log4c /I. /I$(MSINCLUDES) 
OUT_DIR=log4c

SOURCE_SD_FILES := stack.c malloc.c \
            factory.c hash.c list.c \
            sprintf.c test.c sd_xplatform.c error.c


MYLIBPATHS=/LIBPATH:$(MS_SDKLIB_PATH) /LIBPATH:sd 
MYLIBLIST= $^ $(MSLIBS)

####################################################
# Add options to define expat dependency if required
####################################################

ifeq ($(USE_EXPAT),1)
MSVCFLAGS += /DUSE_EXPAT
INCLUDES+=/I"$(EXPAT_LOCATION)/Source/Lib"
SOURCE_SD_FILES += domnode-expat.c
MYLIBPATHS+=/LIBPATH:$(EXPAT_LOCATION)/Libs
MYLIBLIST+=libexpat.lib
else
SOURCE_SD_FILES += domnode.c \
		domnode-xml.c domnode-xml-parser.c domnode-xml-scanner.c \

endif

###############################
# Define SD Sources and Objects
###############################
SD_OBJS :=  $(patsubst %.c,%.obj,$(SOURCE_SD_FILES))
SD_TARGET_OBJS := $(addprefix $(OUT_DIR)/sd/, $(SD_OBJS))


##################
# Default target
##################
all: $(OUT_DIR)/sd $(OUT_DIR)/sd/liblog4c_sd.lib \
	$(OUT_DIR)/log4c $(OUT_DIR)/log4c/liblog4c.dll \
	$(OUT_DIR)/tests tests \
	examples

########################################
# Compile and Link the sd static library
########################################
$(OUT_DIR)/sd/%.obj:$(SRC_LOC)/sd/%.c
	$(CL) $(INCLUDES) /DLOG4C_EXPORTS /c $(MSVCFLAGS) /Fo$@ $<

$(OUT_DIR)/sd/liblog4c_sd.lib: $(SD_TARGET_OBJS)
	$(LIBPROG) /OUT:$@ $^

##########################
# Build output directories
##########################


$(OUT_DIR)/sd:
	mkdir -p $@

$(OUT_DIR)/tests:
	mkdir -p $@

$(OUT_DIR)/log4c:
	mkdir -p  $@

$(OUT_DIR)/examples:
	mkdir -p  $@
	cp ../examples/log4crc $(OUT_DIR)/examples

###########################################
# Define the LOG4C sources and object files
###########################################
SOURCE_LOG4C_FILES = rc.c init.c appender_type_stream.c \
	appender_type_stream2.c layout_type_basic.c layout_type_dated.c \
	version.c logging_event.c priority.c appender.c layout.c \
	category.c \
  appender_type_rollingfile.c rollingpolicy.c rollingpolicy_type_sizewin.c

LOG4C_OBJS :=  $(patsubst %.c,%.obj,$(SOURCE_LOG4C_FILES))
LOG4C_TARGET_OBJS := $(addprefix $(OUT_DIR)/log4c/, $(LOG4C_OBJS))

################################
# Compile and Link the log4c dll
################################

$(OUT_DIR)/log4c/%.obj:$(SRC_LOC)/log4c/%.c
	$(CL) $(INCLUDES) /DLOG4C_EXPORTS /c $(MSVCFLAGS)\
	 /DVERSION="\"$(LINK_VERSION)\"" \
	/DLOG4C_RCPATH="\".\"" \
	$ /Fo$@ $<

$(OUT_DIR)/log4c/liblog4c.dll: $(LOG4C_TARGET_OBJS) \
					$(OUT_DIR)/sd/liblog4c_sd.lib
	$(LINK) $(MSVCLINKFLAGS) /OUT:$@ /IMPLIB:$(basename $@).lib \
		/EXPORT:sd_optind /EXPORT:sd_getopt /EXPORT:sd_gettimeofday \
		/EXPORT:sd_test_delete /EXPORT:sd_test_run /EXPORT:sd_test_add \
		/EXPORT:sd_test_new /EXPORT:sd_test_out /EXPORT:sd_test_get_name \
		/EXPORT:sd_factory_print /EXPORT:sd_debug /EXPORT:sd_error \
		$(MYLIBPATHS) $(MYLIBLIST)

	
#######
# tests
#######
TEST_LOC := ../tests/log4c

tests: $(OUT_DIR)/tests/test_category.exe $(OUT_DIR)/tests/test_rc.exe \
  $(OUT_DIR)/tests/test_stream2.exe $(OUT_DIR)/tests/bench.exe \
    $(OUT_DIR)/tests/test_rollingfile_appender.exe \
      $(OUT_DIR)/tests/test_rollingfile_appender_mt.exe

test_category_SOURCES = test_category.c
$(OUT_DIR)/tests/test_category.exe: $(addprefix $(TEST_LOC)/, $(test_category_SOURCES)) \
	$(OUT_DIR)/log4c/liblog4c.lib
	$(CL) $(INCLUDES) $(MSVCFLAGS) $(filter %.c,$^) /link \
	/LIBPATH:$(MS_SDKLIB_PATH) $(filter %.lib,$^)
	mv *.obj $(OUT_DIR)/tests; mv *.exe $(OUT_DIR)/tests \
	mv *.manifest $(OUT_DIR)/tests; mv *.pdb $(OUT_DIR)/tests


test_rc_SOURCES = test_rc.c
$(OUT_DIR)/tests/test_rc.exe: $(addprefix $(TEST_LOC)/, $(test_rc_SOURCES)) \
	$(OUT_DIR)/log4c/liblog4c.lib
	$(CL) $(INCLUDES) /DSRCDIR="\"$(TEST_LOC)\"" $(MSVCFLAGS) $(filter %.c,$^) /link \
	/LIBPATH:$(MS_SDKLIB_PATH) $(filter %.lib,$^)
	mv *.obj $(OUT_DIR)/tests; mv *.exe $(OUT_DIR)/tests \
	mv *.manifest $(OUT_DIR)/tests; mv *.pdb $(OUT_DIR)/tests

test_stream2_SOURCES = test_stream2.c
$(OUT_DIR)/tests/test_stream2.exe: $(addprefix $(TEST_LOC)/, $(test_stream2_SOURCES)) \
	$(OUT_DIR)/log4c/liblog4c.lib
	$(CL) $(INCLUDES) $(MSVCFLAGS) $(filter %.c,$^) /link \
	/LIBPATH:$(MS_SDKLIB_PATH) $(filter %.lib,$^)
	mv *.obj $(OUT_DIR)/tests; mv *.exe $(OUT_DIR)/tests \
	mv *.manifest $(OUT_DIR)/tests; mv *.pdb $(OUT_DIR)/tests

bench_SOURCES = bench.c
$(OUT_DIR)/tests/bench.exe: $(addprefix $(TEST_LOC)/, $(bench_SOURCES)) \
	$(OUT_DIR)/log4c/liblog4c.lib
	$(CL) $(INCLUDES) $(MSVCFLAGS) $(filter %.c,$^) /link \
	/LIBPATH:$(MS_SDKLIB_PATH) $(filter %.lib,$^)
	mv *.obj $(OUT_DIR)/tests; mv *.exe $(OUT_DIR)/tests \
	mv *.manifest $(OUT_DIR)/tests; mv *.pdb $(OUT_DIR)/tests


test_rollingfile_appender_SOURCES = test_rollingfile_appender.c
$(OUT_DIR)/tests/test_rollingfile_appender.exe: \
  $(addprefix $(TEST_LOC)/, $(test_rollingfile_appender_SOURCES)) \
	$(OUT_DIR)/log4c/liblog4c.lib
	$(CL) $(INCLUDES) $(MSVCFLAGS) $(filter %.c,$^) /link \
	/LIBPATH:$(MS_SDKLIB_PATH) $(filter %.lib,$^)
	mv *.obj $(OUT_DIR)/tests; mv *.exe $(OUT_DIR)/tests \
	mv *.manifest $(OUT_DIR)/tests; mv *.pdb $(OUT_DIR)/tests
  
test_rollingfile_appender_mt_SOURCES = test_rollingfile_appender_mt.c
$(OUT_DIR)/tests/test_rollingfile_appender_mt.exe: \
  $(addprefix $(TEST_LOC)/, $(test_rollingfile_appender_mt_SOURCES)) \
	$(OUT_DIR)/log4c/liblog4c.lib
	$(CL) $(INCLUDES) $(MSVCFLAGS) $(filter %.c,$^) /link \
	/LIBPATH:$(MS_SDKLIB_PATH) $(filter %.lib,$^)
	mv *.obj $(OUT_DIR)/tests; mv *.exe $(OUT_DIR)/tests \
	mv *.manifest $(OUT_DIR)/tests; mv *.pdb $(OUT_DIR)/tests

  
##########
# examples
##########

examples: $(OUT_DIR)/examples $(OUT_DIR)/examples/application_1.exe \
	$(OUT_DIR)/examples/libexamples.lib $(OUT_DIR)/examples/application_2.exe

EXAMPLES_LOC = ../examples

application_1_SOURCES = application_1.c
$(OUT_DIR)/examples/application_1.exe: $(addprefix $(EXAMPLES_LOC)/, $(application_1_SOURCES)) \
	$(OUT_DIR)/log4c/liblog4c.lib
	$(CL) $(INCLUDES) $(MSVCFLAGS) $(filter %.c,$^) /link \
	/LIBPATH:$(MS_SDKLIB_PATH) $(filter %.lib,$^) WINMM.LIB
	mv *.obj $(OUT_DIR)/examples; mv *.exe $(OUT_DIR)/examples
ifeq ($(DEBUG),1)
	mv *.manifest $(OUT_DIR)/examples
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
	/LIBPATH:$(MS_SDKLIB_PATH) $(filter %.lib,$^) WINMM.LIB
	mv *.obj $(OUT_DIR)/examples; mv *.exe $(OUT_DIR)/examples
ifeq ($(DEBUG),1)
	mv *.manifest $(OUT_DIR)/examples
	mv *.pdb *.ilk $(OUT_DIR)/examples
endif

###############
# Clean targets
###############

clean_examples:
	rm -rf $(OUT_DIR)/examples

clean: clean_examples
	rm -rf $(OUT_DIR)/sd $(OUT_DIR)/log4c $(OUT_DIR)/tests
