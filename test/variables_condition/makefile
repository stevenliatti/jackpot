CC = gcc
FLAGS = -g -Wall -std=gnu99
LDFLAGS = -pthread
SRCS=$(wildcard *.c)
OUT=$(SRCS:%.c=%)

all: $(OUT)

%: %.c
	$(CC) $(FLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -rf $(OUT) *~ *.dSYM

rebuild: clean all
