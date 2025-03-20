CC = gcc
CFLAGS = -Wall -pthread

all: centrivaccinali fornitori server

centrivaccinali: centrivaccinali.c
	$(CC) $(CFLAGS) -o centrivaccinali centrivaccinali.c

fornitori: fornitori.c
	$(CC) $(CFLAGS) -o fornitori fornitori.c

server: server.c
	$(CC) $(CFLAGS) -o server server.c

clean:
	rm -f centrivaccinali fornitori server *~

