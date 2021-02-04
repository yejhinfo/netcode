

.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=p2pclt p2psvr client server
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)




