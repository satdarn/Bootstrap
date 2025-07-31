CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LDFLAGS = 

# Directories
SRC_DIR = src
BUILD_DIR = bin
INCLUDE_DIR = include

# Source Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
TARGET = $(BUILD_DIR)/bsc

.PHONY: all clean test run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) test/test.bsx

clean:
	rm -rf $(BUILD_DIR)

test: $(TARGET)
	./$(TARGET) test/test.bsx
