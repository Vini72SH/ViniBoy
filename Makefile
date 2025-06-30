# Directories
INCLUDE_DIR = include
SRC_DIR = libs
BUILD_DIR = build

# Main source file
MAIN = viniboy.c
TARGET = viniboy

# Compiler and flags
CC = gcc

# pkg-config flags for SDL2 and SDL2_ttf
SDL_CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_ttf)
SDL_LIBS = $(shell pkg-config --libs sdl2 SDL2_ttf)

CFLAGS = -Wall -Wextra -I$(INCLUDE_DIR) $(SDL_CFLAGS)
DEBUGFLAGS = -g -DDEBUG

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default rule
all: $(TARGET)

# Link executable
$(TARGET): $(OBJS) $(MAIN)
	@echo "[LD] Linking executable $(TARGET)..."
	@$(CC) $(CFLAGS) $(MAIN) $(OBJS) -o $(TARGET) $(SDL_LIBS)
	@echo "[OK] Executable $(TARGET) created!"

# Debug build
debug: CFLAGS += $(DEBUGFLAGS)
debug: $(TARGET)

# Compile .c files from libs/ to .o in build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	@echo "[CC] Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Clean generated files
clean:
	@echo "[RM] Cleaning object files and executable..."
	@rm -rf $(BUILD_DIR) $(TARGET)
	@echo "[OK] Clean completed."

.PHONY: all clean debug
