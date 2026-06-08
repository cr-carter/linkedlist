.PHONY: all check clean run tidy asan  cppcheck format debug profile
.DEFAULT_GOAL ?= all

# Program name
PROGRAM := linkedlist

# Set stack size
STACK = 512

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Werror -Wextra -Wpedantic -Waggregate-return -Wwrite-strings -Wvla -Wfloat-equal -Winline -Wconversion -Wshadow -std=c99 -Wstack-usage=$(STACK)
LDFLAGS := 

# Tool variables
CLANG_FORMAT ?= clang-format
CLANG_TIDY   ?= clang-tidy
CPPCHECK     ?= cppcheck

# Directories
SRC_DIR := src
INC_DIR := include
TEST_DIR := test
UNITY_DIR := test/unity
OBJ_DIR := obj

# Target and additional defaults
TARGET := $(PROGRAM)
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
TEST_SRC := $(UNITY_DIR)/unity.c $(wildcard $(TEST_DIR)/*.c)
TEST_FILES := $(filter-out $(SRC_DIR)/$(PROGRAM).c,$(SRC_FILES)) $(TEST_SRC)

# Create object file paths
OBJ_FILES := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
TEST_OBJ := $(patsubst %.c,$(OBJ_DIR)/%.o,$(TEST_FILES))

# Default target
all: CFLAGS += -O2 -DNDEBUG
all: $(TARGET)

# Include flag
INCLUDES := -I$(INC_DIR) -I$(UNITY_DIR)
-include $(OBJ_FILES:.o=.d)

# Create directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Create executable
$(TARGET): $(OBJ_FILES) 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test_$(PROGRAM): $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .c to .o
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

# Additional make options

profile: CFLAGS += -pg
profile: LDFLAGS += -pg
profile: TARGET := profile_$(TARGET)
profile: $(TARGET)

debug: CFLAGS := $(filter-out -DNDEBUG, $(CFLAGS))
debug: CFLAGS := $(filter-out -O2, $(CFLAGS))
debug: CFLAGS += -ggdb -O0 -DDEBUG -D_FORTIFY_SOURCE=2
debug: $(TARGET)

run: $(TARGET)
	./$(TARGET)


check: test_$(PROGRAM)
	./test_$(PROGRAM)

clean:
	$(RM) -r $(OBJ_DIR)
	$(RM) $(PROGRAM) test/test_$(PROGRAM)
	$(RM) test_$(PROGRAM)

# Tidy
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
    -- $(filter-out -Wstack-usage=$(STACK), $(CFLAGS)) -I$(INC_DIR)

tidy:
	$(CLANG_TIDY) $(SRC_FILES) $(TIDY_FLAGS)

# asan
asan: clean
asan: CFLAGS := $(filter-out -O2, $(CFLAGS))
asan: CFLAGS := $(filter-out -Wstack-usage=$(STACK), $(CFLAGS))
asan: CFLAGS += -ggdb -O0 -fsanitize=address,undefined,leak -fno-omit-frame-pointer -fsanitize=shift,integer-divide-by-zero,bounds
asan: LDFLAGS += -fsanitize=address,undefined,leak -fsanitize=shift,integer-divide-by-zero,bounds
asan: $(TARGET)

# WILL NOT WORK UNLESS CLANG-17 IS INSTALLED... CANNOT INSTALL ON UBUNTU 22.04.1?
# # tsan 
# TSAN_OPTIONS := \
# 	verbosity=3 \
# 	halt_on_error=0 \
# 	history_size=7 \
# 	report_signal_unsafe=1 \
# 	report_odr_violation=1 \
# 	second_deadlock_stack=1 \
# 	print_stacktrace=1

# tsan: CFLAGS += -g3 -O1 -fsanitize=thread -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wframe-larger-than=512
# tsan: CFLAGS := $(filter-out -Wstack-usage=512,$(CFLAGS))
# tsan: LDFLAGS += -fsanitize=thread
# tsan: $(TARGET)
# 	TSAN_OPTIONS=$(TSAN_OPTIONS) ./$(TARGET)


# cppcheck
cppcheck:
	@echo "==> Running cppcheck..."
	$(CPPCHECK) --enable=all --inconclusive --std=c99 \
		--suppress=missingIncludeSystem $(SRC_FILES) $(INCLUDES)

# format
format:
	$(CLANG_FORMAT) -i $(SRC_FILES) $(wildcard $(INC_DIR)/*.h)

# End of makefile #
