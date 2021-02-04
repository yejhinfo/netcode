

.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=dm02_server dm03_clinet dm04_svr2
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)




