.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=srv2 cli2
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
	
cli2:cli2.o sckutil.o
	$(CC) $(CFLAGS) $^ -o $@
	
srv2:srv2.o sckutil.o
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -f *.o $(BIN)




