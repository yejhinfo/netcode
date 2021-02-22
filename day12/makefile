.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=dm02_multhread
all:$(BIN)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ 
	
dm02_multhread:dm02_multhread.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

clean:
	rm -f *.o $(BIN)
