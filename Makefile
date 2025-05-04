# see LICENSE file for copyright and license information

.PHONY: test clean-test clean-example clean example

CC := cc
CFLAGS := -std=c99 -Os -g -fsanitize=address
TEST_CFLAGS := $(CLAGS) -Wall -Werror -Wpedantic -fsanitize=address -g -Dmkhas_DEBUG
TEST_DIR := test
BUILD_DIR := build

TEST_DIR_BUILD := $(BUILD_DIR)/$(basename $(TEST_DIR))
TEST_SRC := $(shell find $(TEST_DIR) -type f -name '*.c')
TEST_BIN := $(patsubst $(TEST_DIR)/%.c,$(TEST_DIR_BUILD)/%,$(TEST_SRC))

all: example test

test: $(TEST_BIN)
	./test.sh $(TEST_DIR) $(BUILD_DIR)

$(TEST_DIR_BUILD)/%: $(TEST_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $< -o $@ $(TEST_CFLAGS) -I./

clean-test:
	rm -r $(TEST_DIR_BUILD)/*

example: example.c mkhas.h
	$(CC) example.c -o $(BUILD_DIR)/example $(CFLAGS)

clean-example:
	rm $(BUILD_DIR)/example

clean: clean-test clean-example
