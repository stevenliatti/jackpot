CC=gcc
CFLAGS=-g -Wall -Wextra -std=gnu99
LDFLAGS=-pthread -lpthread
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
EXEC=jackpot

all: $(EXEC) mostlyclean

$(EXEC): $(OBJ)
	$(CC) $^ -o $(EXEC) $(LDFLAGS) -lrt

%o: %c
	$(CC) $(CFLAGS) -c $^

run: $(EXEC)
	./$<

mostlyclean:
	rm -rf *.o *~ *.dSYM

clean:
	rm -rf $(EXEC) *.o *~ *.dSYM

rebuild: clean all
