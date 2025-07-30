CC = gcc 
CFLAGS = -Wall -Wextra -G -Iinclude 
LDFLAGS = 

# Directories
SRC_DIR = src 
TEST_DIR = test 
BUILD_DIR = bin 
INCLUDE_DIR = include 

# Source Files
MAIN_SRC = $(SRC_DIR)/main.c 

# Test Files 
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)

# Object Files 
MAIN_OBJ = $(BUID_DIR)/main.o 
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/%.o, $(TEST_SRCS))

# Executables 

TARGET = bsc 
TEST_EXEC = bootstrap_tests

.PHONY: all clean test 

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf bin
