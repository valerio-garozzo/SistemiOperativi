CC = gcc
CFLAGS = -Wall -g
OBJ = list.o

all: server casaeditrice lettore

server: server.c $(OBJ)
	$(CC) $(CFLAGS) -o server server.c $(OBJ)

casaeditrice: casaeditrice.c $(OBJ)
	$(CC) $(CFLAGS) -o casaeditrice casaeditrice.c $(OBJ)

lettore: lettore.c $(OBJ)
	$(CC) $(CFLAGS) -o lettore lettore.c $(OBJ)

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

clean:
	rm -f server casaeditrice lettore $(OBJ) *~

