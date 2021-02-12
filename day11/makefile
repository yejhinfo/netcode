

.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=dm01_pipe writeshm readshm sem
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)




