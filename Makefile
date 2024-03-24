# Compiler Flags of sorts
CC = gcc
CFLAGS = -Isrc/lib -Wall -Wextra -Werror -g
LIBS = -lX11

# Directory Vars
DIR_SRC = src
DIR_DEBUG = debug
DIR_RELEASE = release
DIR_INSTALL ?= $(DIR_RELEASE)

# Source Files: src/*.c
FILES_SRC := $(wildcard $(DIR_SRC)/*.c)
# Library Files: src/lib/**/*.c
FILES_LIB := $(shell find $(DIR_SRC)/lib -type f -name '*.c')
# Object Files: src/<path>/<file>.c -> debug/<path>/<file>.o
FILES_OBJ_LIB := $(patsubst $(DIR_SRC)/%.c,$(DIR_DEBUG)/%.o,$(FILES_LIB))
# Executable Files: src/blah.c -> debug/blah
FILES_BINARIES = $(patsubst $(DIR_SRC)/%.c,$(DIR_DEBUG)/%,$(FILES_SRC))
FILES_INSTALLED = $(patsubst $(DIR_SRC)/%.c,$(DIR_INSTALL)/%,$(FILES_SRC))

## Helper Values
empty :=
space := $(empty) $(empty)

# Default target
all: FILES_BINARIES
	@echo "Available Binaries:\n$(space)$(space)$(subst $(space),\n$(space)$(space),$(FILES_BINARIES))"

# Build a "release" build and install!
install: CFLAGS := $(filter-out -g,$(CFLAGS))
install: clean all
	@echo "Installing to: $(DIR_INSTALL)"
	@mkdir -p $(DIR_INSTALL)
	@cp $(FILES_BINARIES) $(DIR_INSTALL)
	@rm -rf $(DIR_DEBUG)

# Build the Objects for Lib
FILES_OBJ_LIB: $(FILES_OBJ_LIB) # Simple named map to files
$(FILES_OBJ_LIB): $(DIR_DEBUG)/%.o : $(DIR_SRC)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)
	@echo "Built: $@"

# Rule to build the object files
FILES_BINARIES: FILES_OBJ_LIB $(FILES_BINARIES) # Simple named map to files
$(FILES_BINARIES): $(DIR_DEBUG)/%: $(DIR_SRC)/%.c
	@mkdir -p $(DIR_DEBUG)
	@$(CC) $(CFLAGS) $< -o $@ $(FILES_OBJ_LIB) $(LIBS)
	@echo "Built: $@"

# Clean target
clean:
	@rm -rf $(DIR_DEBUG) $(DIR_RELEASE) $(FILES_INSTALLED)
	@echo "Cleared: $(DIR_DEBUG), $(DIR_RELEASE), $(FILES_INSTALLED)"

.PHONY: clean all install