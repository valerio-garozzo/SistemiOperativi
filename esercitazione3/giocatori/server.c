#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <netdb.h>
#include "list.h"


#define BUF_SIZE 1000
#define PLAYER_NUMBER 3

int port = 8000;


int main() 
{
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;

	// Socket opening
	int sockfd = socket( PF_INET, SOCK_STREAM, 0 );  
	if ( sockfd == -1 ) 
	{
		perror("Error opening socket");
		exit(1);
	}
	
	int options = 1;
	if(setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof (options)) < 0) {
		perror("Error on setsockopt");
		exit(1);
	}

	bzero( &serv_addr, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	// Address bindind to socket
	if ( bind( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr) ) == -1 ) 
	{
		perror("Error on binding");
		exit(1);
	}
	
	// Maximum number of connection kept in the socket queue
	if ( listen( sockfd, 20 ) == -1 ) 
	{
		perror("Error on listen");
		exit(1);
	}


	socklen_t address_size = sizeof( cli_addr );	
	
	// CONNESSIONI IN INGRESSO

	ItemType msg;
	LIST ranking = NewList(); // storico
	LIST players = NewList();  // registra risultati

	srand(time(NULL));

	while(1) 
	{
		printf("\nWaiting for a new connection...\n");
		
		// New connection acceptance		
		int newsockfd = accept( sockfd, (struct sockaddr *)&cli_addr, &address_size );      
		if (newsockfd == -1) 
		{
			perror("Error on accept");
			exit(1);
		}
		
		
		
		// Message reception
		if ( recv( newsockfd, &msg, sizeof(ItemType), 0 ) == -1) 
		{
			perror("Error on receive");
			exit(1);
		}

		printf("Received from player: \"%s\"\n", msg.name);

			// controllare se il giocatore è nuovo o no

		ItemType* player = Find(ranking, msg);

		if(player == NULL){
			// giocatore nuovo
			printf("New player connect for the first time: \"%s\"\n", msg.name);
			msg.point = 0;
			msg.n_game = 0;
			msg.sockfd = 0;
			ranking = EnqueueLast(ranking,msg);
		} else {
			// giocatore gia registrato
			printf("Player %s connected and has already played %d game \n", player->name, player->n_game);
		}

		msg.sockfd = newsockfd;
		players = EnqueueLast(players, msg);

		printf("Players waiting for the game:\n");
		
		// TODO stampa di tutti i giocatori connessi

		LIST it; //puntatore temporaneo
		for(it = players; it != NULL; it = it->next) {				
			player = Find(ranking, it->item);
			PrintItem(*player);
		} 

		if ( getLength(players) >= PLAYER_NUMBER){
			// Inizia il gioco simulato
			// fine del gioco
			// aggiornare lista ranking - num pertite e punteggio
			printf("\nPlaying the game...\n");
			
						
			// Increasing number of matches
			for(it = players; it != NULL; it = it->next) {				
				player = Find(ranking, it->item);
				player->n_game++;
			}
			
			
			// Classifica causale
			int points;		
			for (points = 3; points > 0; --points) 
			{
			
				/* Deciding the winner */
				int remainingPlayers = getLength(players);				
				int winningIndex = rand() % remainingPlayers;
				ItemType winningPlayer = getItemAtPos(players, winningIndex);
				
				
				/* Attributing points */
				player = Find(ranking, winningPlayer);
				player->point += points;				
				
				printf("Player %s wins %d points. Total score: %d\n", player->name, points, player->punteggio);
				
				
				/* Sending the result of the match */
				if (send(winningPlayer.sockfd, &points, sizeof(int), 0) == -1) 
				{
					perror("Error sending the result to a winner");
					exit(1);
				}
				// IMPORTANTE: chiusura socket e rimozione dalla lista
				close(winningPlayer.sockfd);
				players = Dequeue(players,*player);
			}
			
			
			/* Remaining players (losers) */
			while (!isEmpty(players)) 
			{
				ItemType loser = getHead(players);
				printf("Player %s lose\n",loser.name);
				
				int points = 0;
				if ( send(loser.sockfd,&points,sizeof(int), 0) == -1) 
				{
					perror("Error sending the result to a loser");
					exit(1);
				}
				close(loser.sockfd);
				players = DequeueFirst(players);
			}
			printf("\nRanking:\n");
			PrintList(ranking);


		}

	}

	close(sockfd);
	return 0;
}



