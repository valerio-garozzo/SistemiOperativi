#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <netdb.h>
#include "list.h"


char *host_name = "127.0.0.1"; /* local host */
int port = 8000;


int main(int argc, char *argv[]) 
{
	
	
	if (argc < 2) { 
		printf("Usage: %s player name\n\n", argv[0]);
		return -1;
	}

	struct sockaddr_in serv_addr;
 	struct hostent* server;	
	
	if ( ( server = gethostbyname(host_name) ) == 0 ) 
	{
		perror("Error resolving local host\n");
		exit(1);
	}

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
	serv_addr.sin_port = htons(port);
	
	int sockfd = socket( PF_INET, SOCK_STREAM, 0 );
	if ( sockfd == -1 ) 
	{
		perror("Error opening socket\n");
		exit(1);
	}    

	if ( connect(sockfd, (void*)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error connecting to socket\n");
		exit(1);
	}
    

	// INVIO DEL NOME AL SERVER

	ItemType msg;
	strcpy(msg.name, argv[1]);
	msg.point = 0;
	msg.n_game = 0;
	msg.sockfd = 0;

	/* This sends the string plus the string terminator '\0' */
	if ( send(sockfd, &msg, sizeof(msg), 0) == -1 ) 
	{
		perror("Error on send\n");
		exit(1);
	}

	printf("Message sent. Waiting for start the game...\n");

	// RICEVO IL RISULTATO

	int score;
	if ( send(sockfd, &score, sizeof(score), 0) == -1)
	{
		perror("Error receiving score from server\n");
		exit(1);
	}

	printf("\nCurrent score is: %d\n\n",score);

	close(sockfd);

	return 0;
}



