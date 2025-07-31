CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude 
LDFLAGS = 

# Directories
SRC_DIR = src 
BUILD_DIR = bin 
INCLUDE_DIR = include 

# Source Files
MAIN_SRC = src/main.c 
TARGET = bin/bsc

.PHONY: all clean test 

all: $(TARGET)

$(TARGET): $(MAIN_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
