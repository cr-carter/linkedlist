.PHONY: all check clean run debug profile
.DEFAULT_GOAL := all

# ##############################
# Project configurations
#
# These are the main configuration that will be used in the makefile. 
# The program name, the source of the main file, the tools used (compiler, archiver, formatter, etc),
# the directory names, the source files, and the object paths are all defined here.
# ##############################

# Modify program_name (binary file name) for project
PROGRAM_NAME ?= $(notdir $(CURDIR))

# Modify if main.c is not entry to program
MAIN ?= main.c

# Modify LIBS with library names ("m", "msg_queue", etc)
LIBS ?= 
# Modify to -L./lib if given library file
LDFLAGS ?= 
LDLIBS := $(addprefix -l,$(LIBS))

CC ?= gcc
CC_STD ?= -std=c99
STACK_USAGE = -Wstack-usage=512
STD_CFLAGS := -Wall -Werror -Wextra -Wpedantic -Waggregate-return -Wwrite-strings -Wvla -Wfloat-equal -Winline -Wconversion -Wshadow $(CC_STD) $(STACK_USAGE)

# Directory structure
SRC_DIR := src
PUB_INC_DIR := include
PRI_INC_DIR := $(SRC_DIR)/include
TEST_DIR := tests
UNITY_DIR := $(TEST_DIR)/unity
OBJ_DIR := obj

# Default settings (release mode)
BUILD_SUBDIR = release
CFLAGS  = $(STD_CFLAGS) -DNDEBUG
TARGET_SUFFIX =

# Source files
ALL_SRC := $(wildcard $(SRC_DIR)/*.c)
CORE_SRC := $(SRC_DIR)/$(MAIN)
LIB_SRC := $(filter-out $(CORE_SRC), $(ALL_SRC))
TEST_SRC := $(UNITY_DIR)/unity.c $(wildcard $(TEST_DIR)/*.c)

# Create object / library paths
CURRENT_OBJ_DIR = $(OBJ_DIR)/$(BUILD_SUBDIR)
CORE_OBJ = $(patsubst %.c, $(CURRENT_OBJ_DIR)/%.o, $(CORE_SRC))
LIB_OBJ = $(patsubst %.c, $(CURRENT_OBJ_DIR)/%.o, $(LIB_SRC))
TEST_OBJ = $(patsubst %.c, $(CURRENT_OBJ_DIR)/%.o, $(TEST_SRC))

ALL_LIB_OBJS = $(CORE_OBJ) $(LIB_OBJ)


TARGET    = $(PROGRAM_NAME)$(TARGET_SUFFIX)
INCLUDES  = -I$(PUB_INC_DIR) -I$(PRI_INC_DIR)


# ##############################
# Project builds
#
# These are the steps taken to build the project, regardless of which mode is used.
# Link libraries
# Compile all needed .c files to .o files
# Include all dependencies
# ##############################

all: $(TARGET)

# Link libraries
$(TARGET): $(ALL_LIB_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Compile C source files (App & Lib), also compiles test files
$(CURRENT_OBJ_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Include generated dependencies
-include $(patsubst %.o,%.d,$(ALL_LIB_OBJS) $(TEST_OBJ))

# ##############################
# Quality, test, and profile
#
# Command to run each make option
# ##############################

debug: CFLAGS  = $(STD_CFLAGS) -ggdb -DDEBUG -D_FORTIFY_SOURCE=2
debug: BUILD_SUBDIR = debug
debug: TARGET_SUFFIX = _debug
debug:
	@$(MAKE) CFLAGS="$(STD_CFLAGS) -ggdb -DDEBUG -D_FORTIFY_SOURCE=2" BUILD_SUBDIR=debug TARGET_SUFFIX=_debug all

profile: CFLAGS  = $(STD_CFLAGS) -pg
profile: LDFLAGS += -pg
profile: BUILD_SUBDIR = profile
profile: TARGET_SUFFIX = _profile
profile:
	@$(MAKE) CFLAGS="$(STD_CFLAGS) -pg" LDFLAGS+="-pg" BUILD_SUBDIR=profile TARGET_SUFFIX=_profile all

check: INCLUDES += -I$(UNITY_DIR)
check: $(LIB_OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) -g -O0 $(INCLUDES) -o test_$(PROGRAM_NAME) $(TEST_OBJ) $(LIB_OBJ) $(LDFLAGS) $(LDLIBS)
	./test_$(PROGRAM_NAME)

clean:
	$(RM) -r $(OBJ_DIR)
	$(RM) $(PROGRAM_NAME) $(PROGRAM_NAME)_debug $(PROGRAM_NAME)_profile $(PROGRAM_NAME)_asan test_$(PROGRAM_NAME) gmon.out

# ##############################
# Local tools
# ##############################
-include makefile.dev

# End of makefile #
