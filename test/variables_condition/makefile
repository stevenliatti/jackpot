CC = gcc
FLAGS = -g -Wall -std=c99
SRCS=$(wildcard *.c)
OUT=$(SRCS:%.c=%)

all: $(OUT)

%: %.c
	$(CC) $(FLAGS) $< -o $@

clean:
	rm -rf $(OUT) *~ *.dSYM

rebuild: clean all
