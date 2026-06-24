# Toolchain
CC      ?= gcc
AR      ?= ar
RANLIB  ?= ranlib

# Directories
SRC_DIR   := src
BUILD_DIR := build
LIB_NAME  := libc.a

# Freestanding libc build flags
CFLAGS := \
    -std=c23 \
    -ffreestanding \
    -fno-builtin \
    -fno-stack-protector \
    -Wall \
    -Wextra \
    -I$(SRC_DIR)

# Recursive source discovery
SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')

# Preserve directory structure in build/
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(BUILD_DIR)/$(LIB_NAME)

# Static library
$(BUILD_DIR)/$(LIB_NAME): $(OBJS)
	@mkdir -p $(dir $@)
	$(AR) rcs $@ $^
	$(RANLIB) $@

# Object compilation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) \
	    -MMD -MP \
	    -c $< \
	    -o $@

clean:
	rm -rf $(BUILD_DIR)

-include $(DEPS)
