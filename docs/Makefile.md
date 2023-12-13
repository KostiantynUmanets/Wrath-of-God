# C Project Makefile Documentation

This document explains the Makefile used for compiling C projects. It details the purpose of each instruction and provides links to relevant documentation for further reading.

## Overview

The Makefile compiles C source files located in the `src/` directory, applying strict compilation flags, and outputs the executable to the `build/` directory. It also includes a check for the GCC compiler version.

### Compiler and Flags

- **Compiler**: `gcc`
  - GCC, the GNU Compiler Collection, is a standard compiler for C.
  - [GCC Documentation](https://gcc.gnu.org/onlinedocs/gcc/)
- **Flags**: `-Wall -Wextra -Werror`
  - These flags enable additional warnings and treat all warnings as errors.
  - [GCC Warning Options](https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html)

### Directories

- **Source Directory**: `src`
  - Contains the .c source files.
- **Build Directory**: `build`
  - The directory where object files and the final executable are placed.

### Files and Targets

- **SOURCES**: List of .c files in the `src` directory.
  - Uses the `wildcard` function.
  - [GNU Make Wildcard Function](https://www.gnu.org/software/make/manual/html_node/Wildcard-Function.html)
- **OBJECTS**: Corresponding .o files for each .c file.
  - Uses `patsubst` for string substitution.
  - [GNU Make Patsubst Function](https://www.gnu.org/software/make/manual/html_node/Text-Functions.html)
- **TARGET**: The final executable file.

### Rules

- **all**: The default target.
  - First checks the GCC version, then builds the target executable.
- **check-gcc-version**: Checks if GCC is installed and its version.
  - Uses shell commands within a Makefile.
  - [Using Shell Commands in Makefiles](https://www.gnu.org/software/make/manual/html_node/Shell-Function.html)
- **$(TARGET)**: Compiles the object files into the final executable.
- **$(BUILD_DIR)/%.o**: Compiles each .c file into an .o file.
  - Includes automatic directory creation for the build folder.
- **clean**: Removes the `build/` directory and its contents.

### Special Targets

- **.PHONY**: Indicates targets that do not correspond to files.
  - Ensures that these targets are always executed when requested.
  - [Phony Targets in Make](https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html)
