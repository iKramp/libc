# LIBC makefile
# Toolchain
CC      ?= gcc
NASM    ?= nasm
AR      ?= ar
RANLIB  ?= ranlib
LD      ?= ld

# Directories
SRC_DIR    := src
TEST_DIR   := tests
BUILD_DIR  := build
LINKER_DIR := linker

LIB_NAME    := libc.a
CRT0_OBJ    := crt0.o
TEST_BIN    := libc-tests
TEST_LINKER := $(LINKER_DIR)/test.ld

# Freestanding libc build flags
CFLAGS := \
    -std=c23 \
    -ffreestanding \
    -fno-builtin \
    -fno-stack-protector \
    -Wall \
    -Wextra \
    -I$(SRC_DIR)/include \
	-I$(SRC_DIR)

TEST_CFLAGS := \
    $(CFLAGS) \
    -I$(TEST_DIR)

NASMFLAGS := -f elf64

TEST_LDFLAGS := \
    -nostdlib \
    -static \
    -no-pie \
    -Wl,-T,$(TEST_LINKER)



# Recursive source discovery
SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')

# Every .c under tests/, including tests/test.c
TEST_SRCS := $(shell find $(TEST_DIR) -type f -name '*.c')

# Preserve directory structure in build/
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
TEST_OBJS := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/tests/%.o,$(TEST_SRCS))
DEPS := $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

LIBC := $(BUILD_DIR)/$(LIB_NAME)
CRT0 := $(BUILD_DIR)/$(CRT0_OBJ)
TEST := $(BUILD_DIR)/$(TEST_BIN)

# ---- MAKE RULES ----

.PHONY: all test clean compile_commands

all: $(LIBC) $(CRT0)

test: $(TEST)
	@echo "Built $(TEST)"

clean:
	rm -rf $(BUILD_DIR)

compile_commands:
	bear -- make clean all test

# ---- LIBC BUILD ----

# Static library
$(LIBC): $(OBJS)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $^
	$(RANLIB) $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) \
	    -MMD -MP \
	    -c $< \
	    -o $@

# ---- CRT0 BUILD ----

# CRT0 assembly
$(BUILD_DIR)/$(CRT0_OBJ): $(SRC_DIR)/internal/crt0.S
	@mkdir -p $(dir $@)
	$(NASM) $(NASMFLAGS) $< -o $@

# ---- TEST BUILD ----

$(BUILD_DIR)/tests/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(TEST_CFLAGS) \
	    -MMD -MP \
	    -c $< \
	    -o $@

$(TEST): $(TEST_OBJS) $(CRT0) $(LIBC) $(TEST_LINKER)
	@mkdir -p $(dir $@)
	$(CC) \
	    $(TEST_LDFLAGS) \
	    -o $@ \
	    $(CRT0) \
	    $(TEST_OBJS) \
	    -Wl,--whole-archive $(LIBC) -Wl,--no-whole-archive


# ---- DEPS ----

-include $(DEPS)
