CC=gcc
BUILD_DIR=./build
SRC_DIR=./src

CFLAGS= -Wall -Wextra -Wpedantic -g -flto -O0 -MP -MD -c

CFILES=$(wildcard $(SRC_DIR)/*.c)
OBJECTS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CFILES))
DEPFILES=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.d, $(CFILES))

BINARY=clox

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.c 
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)
