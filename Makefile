CC=gcc
LIB_SRC=lib
INC=include
CFLAGS=-I$(INC) -Wall -c 
SERVER_OBJS=error.o wrapper.o fifo.o server.o readline.o request_parser.o transaction.o db.o
CLIENT_OBJS=error.o wrapper.o fifo.o client.o readline.o request_parser.o
STARGET=server
CTARGET=client
ifneq ($(DEBUG),)
	CFLAGS += -g -DDEBUG
else
	CFLAGS += -O2
endif

all: clean server client

server: $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o $(STARGET) -L /usr/lib/sqlite3 -lsqlite3

client: $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o $(CTARGET)

server.o: src/server.c
	$(CC) $(CFLAGS) $<

client.o: client_src/client.c
	$(CC) $(CFLAGS) $<

fifo.o: fifo/fifo.c
	$(CC) $(CFLAGS) $<

error.o: lib/error.c
	$(CC) $(CFLAGS) $<

wrapper.o: lib/wrapper.c
	$(CC) $(CFLAGS) $<

readline.o: lib/readline.c
	$(CC) $(CFLAGS) $<

request_parser.o: lib/request_parser.c
	$(CC) $(CFLAGS) $<

transaction.o: src/transaction.c
	$(CC) $(CFLAGS) $<

db.o: src/db.c
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o
	rm -f server
	rm -f client
