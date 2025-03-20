CC = gcc
CFLAGS = -Wall -Wextra -g
OBJ = list.o

all: server agentidiborsa investitori

server: server.c $(OBJ)
	$(CC) $(CFLAGS) -o server server.c $(OBJ)

agentidiborsa: agentidiborsa.c $(OBJ)
	$(CC) $(CFLAGS) -o agentidiborsa agentidiborsa.c $(OBJ)

investitori: investitori.c $(OBJ)
	$(CC) $(CFLAGS) -o investitori investitori.c $(OBJ)

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

clean:
	rm -f server agentidiborsa investitori $(OBJ) *~


