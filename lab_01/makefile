CC = gcc
LINTER = CodeChecker
CFLAGS = -std=c99 -Wall -Werror -Wpedantic
INC = ./inc/
SRC = $(wildcard ./src/*.c)
OUT = ./out/
TARGET = main.out


.PHONY: all clean test linter

all: $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -rf $(TARGET) *.gcov *.gcda *.gcno

test:
	$(CC) $(CFLAGS)-O0 -fprofile-arcs -ftest-coverage -o $(TARGET) $(SRCS)
	bash ./func_tests/test.sh
	$(foreach s,$(SRCS), gcov $s;)

linter:
	$(foreach s,$(SRCS), $(LINTER) $s;)
