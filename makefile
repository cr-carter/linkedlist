.PHONY: all check clean run tidy asan cppcheck format debug profile release
.DEFAULT_GOAL := all

# ##############################
# Project configurations
#
# These are the main configuration that will be used in the makefile. 
# The program name, the source of the main file, the tools used (compiler, archiver, formatter, etc),
# the directory names, the profile settings (release, debug, asan, profile),
# the source files, and the object paths are all defined here.
# ##############################

PROGRAM_NAME := main
STACK = 256
MAIN := main.c

CC := gcc
AR := ar
ARFLAGS := rcs
MKDIR_P := mkdir -p
STD_CFLAGS := -Wall -Werror -Wextra -Wpedantic -Waggregate-return -Wwrite-strings -Wvla -Wfloat-equal -Winline -Wconversion -Wshadow -std=c99 -Wstack-usage=$(STACK)
LDFLAGS := -L./$(LIB_DIR) 

# Tool variables
CLANG_FORMAT ?= clang-format
CLANG_TIDY   ?= clang-tidy
CPPCHECK     ?= cppcheck
 
# Directory structure
SRC_DIR := src
PUB_INC_DIR := include
PRI_INC_DIR := $(SRC_DIR)/include
TEST_DIR := test
UNITY_DIR := $(TEST_DIR)/unity
OBJ_DIR := obj
LIB_DIR := lib

# Profile-specific flags handling (Prevents stale build caches)
PROFILE ?= release
ifeq ($(PROFILE), release)
	CFLAGS  := $(STD_CFLAGS) -O2 -DNDEBUG
	BUILD_SUBDIR := release
else ifeq ($(PROFILE), debug)
	CFLAGS  := $(STD_CFLAGS) -ggdb -O0 -DDEBUG -D_FORTIFY_SOURCE=2
	BUILD_SUBDIR := debug
else ifeq ($(PROFILE), asan)
	CFLAGS := $(filter-out -Wstack-usage=$(STACK), $(STD_CFLAGS)) -ggdb -O0 -fsanitize=address,undefined,leak \
               -fno-omit-frame-pointer -fsanitize=shift,integer-divide-by-zero,bounds
	LDFLAGS += -fsanitize=address,undefined,leak -fsanitize=shift,integer-divide-by-zero,bounds
	BUILD_SUBDIR := asan
else ifeq ($(PROFILE), profile)
	CFLAGS  := $(STD_CFLAGS) -O2 -pg
	LDFLAGS += -pg
	BUILD_SUBDIR := profile
endif

# Source files
ALL_SRC := $(wildcard $(SRC_DIR)/*.c)
CORE_SRC := $(SRC_DIR)/$(MAIN)
PROJECT_SRC := $(filter-out $(CORE_SRC), $(ALL_SRC))
TEST_SRC := $(UNITY_DIR)/unity.c $(wildcard $(TEST_DIR)/*.c)

# Create object / library paths
CURRENT_OBJ_DIR := $(OBJ_DIR)/$(BUILD_SUBDIR)
CORE_OBJ := $(patsubst %.c, $(CURRENT_OBJ_DIR)/%.o, $(CORE_SRC))
PROJECT_OBJ := $(patsubst %.c, $(CURRENT_OBJ_DIR)/%.o, $(PROJECT_SRC))
TEST_OBJ := $(patsubst %.c, $(CURRENT_OBJ_DIR)/%.o, $(TEST_SRC))

# Combine internal project sources into ONE local static library archive
PROJECT_A := $(LIB_DIR)/lib$(PROGRAM_NAME)_internal.a
TARGET    := $(PROGRAM_NAME)
CPPFLAGS  := -I$(PUB_INC_DIR) -I$(PRI_INC_DIR)


# ##############################
# Project builds
#
# These are the steps taken to build the project, regardless of which profile is used.
# Link internal libraries
# Compile all needed .c files to .o files
# Include all dependencies
# ##############################

all: $(TARGET)

# Link internal library
$(TARGET): $(CORE_OBJ) $(PROJECT_A)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Create internal library
$(PROJECT_A): $(PROJECT_OBJ) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $^

# Compile C source files (App & Lib), also compiles test files
$(CURRENT_OBJ_DIR)/%.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

$(LIB_DIR):
	$(MKDIR_P) $(LIB_DIR)

# Include generated dependencies
-include $(patsubst %.o,%.d,$(CORE_OBJ) $(PROJECT_OBJ) $(TEST_OBJ))

# ##############################
# Quality, test, and profile
#
# Command oo run each make option
# (run, release, debug, asan, profile, check, clean)
# ##############################

run: all
	./$(TARGET)

release:
	@$(MAKE) PROFILE=release all

debug:
	@$(MAKE) PROFILE=debug all

asan:
	@$(MAKE) PROFILE=asan all

profile:
	@$(MAKE) PROFILE=profile all

check: CPPFLAGS += -I$(UNITY_DIR)
check: $(PROJECT_A) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o test_$(PROGRAM_NAME) $(TEST_OBJ) $(PROJECT_A) $(LDFLAGS) $(LDLIBS)
	./test_$(PROGRAM_NAME)

clean:
	$(RM) -r $(OBJ_DIR) $(PROJECT_A)
	$(RM) $(PROGRAM_NAME) test_$(PROGRAM_NAME) gmon.out

# ##############################
# Static analysis and formatting
#
# Additional settings and commands for tidy, cppcheck, and format
# ##############################

TIDY_CHECKS := -*,\
    readability-*,\
	-readability-braces-around-statements,\
    -readability-magic-numbers, \
    performance-*,\
    bugprone-*,\
	-bugprone-easily-swappable-parameters, \
    clang-analyzer-*,\
    cert-*,\
	cppcoreguidelines-init-variables,\
    misc-*,\
    modernize-*,\
    -modernize-use-trailing-return-type


TIDY_FLAGS := --checks='$(TIDY_CHECKS)' \
    --warnings-as-errors='*' \
    -- $(filter-out -Wstack-usage=$(STACK), $(CFLAGS)) $(CPPFLAGS)

tidy:
	$(CLANG_TIDY) $(ALL_SRC) $(TEST_DIR)/*.c $(TIDY_FLAGS)


# cppcheck
cppcheck:
	@echo "==> Running cppcheck..."
	$(CPPCHECK) --enable=all --inconclusive --std=c99 \
		--suppress=missingIncludeSystem $(ALL_SRC) $(wildcard $(TEST_DIR)/*.c) $(CPPFLAGS)

# format
ALL_HEADERS := \
    $(wildcard $(PUB_INC_DIR)/*.h) \
    $(wildcard $(PRI_INC_DIR)/*.h) \
    $(wildcard $(TEST_DIR)/*.h)

format:
	$(CLANG_FORMAT) -i $(ALL_SRC) $(ALL_HEADERS)

# End of makefile #
