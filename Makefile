CC=gcc
BUILD_DIR=./build
SRC_DIR=./src

OPT= -O0
DEPFLAGS=-MP -MD
CFLAGS= -Wall -Wextra -g -I$(SRC_DIR) $(OPT) $(DEPFLAGS)

CFILES=$(wildcard $(SRC_DIR)/*.c)
OBJECTS=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CFILES))
DEPFILES=$(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.d, $(CFILES))

BINARY=prog

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $^ -o $@

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.c 
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)
