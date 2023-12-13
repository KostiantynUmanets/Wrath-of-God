# Makefile for C projects with best practices
# Sources are taken from 'src/' folder and binaries are placed in 'build/' folder.
# Additional flags: -Wall -Wextra -Werror for more strict error checking.

# Compiler to use
CC := gcc

# Flags for compiler
CFLAGS := -Wall -Wextra -Werror

# Source and build directories
SRC_DIR := src
BUILD_DIR := build

# Find all .c files in the source directory
SOURCES := $(wildcard $(SRC_DIR)/*.c)

# Replace .c from SOURCES with .o and prefix with build directory
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# Target executable
TARGET := $(BUILD_DIR)/my_program

# Default target
all: $(TARGET)

# Rule to create the target executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to create object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean
