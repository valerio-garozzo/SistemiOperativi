CC=gcc
CFLAGS=-Wall -g

OBJS=list.o

all: server guide visitatori test_list

server: server.c $(OBJS)
	$(CC) $(CFLAGS) -o server server.c $(OBJS)

guide: guide.c
	$(CC) $(CFLAGS) -o guide guide.c

visitatori: visitatori.c
	$(CC) $(CFLAGS) -o visitatori visitatori.c

test_list: test_list.c list.c
	$(CC) $(CFLAGS) -o test_list test_list.c list.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

clean:
	rm -f *.o server guide visitatori test_list




