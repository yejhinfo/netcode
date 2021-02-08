

.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=echoclient echosrv echoclient-sigpipe selectdm
all:$(BIN)
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f *.o $(BIN)




