CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -O2
SRC_DIR = src
BIN_DIR = bin
OBJS = $(BIN_DIR)/hash_tab.o $(BIN_DIR)/main.o

all: $(BIN_DIR)/main

$(BIN_DIR)/main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean