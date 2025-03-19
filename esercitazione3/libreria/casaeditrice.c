#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "list.h"

#define BUF_SIZE 1000
#define HOST "127.0.0.1"
#define PORT 8000

int main(int argc, char *argv[]) 
{
    if (argc < 3) { 
        printf("Usage: %s <titolo_libro> <num_copie>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Creazione e popolazione della struttura itemType
    ItemType msg;
    strncpy(msg.titolo, argv[1], TITOLO_LENGTH);
    msg.titolo[TITOLO_LENGTH] = '\0';  // Garantire la terminazione
    msg.num_copie = atoi(argv[2]);
    msg.tipo = 1;

    struct sockaddr_in serv_addr;
    struct hostent* server;    

    if ((server = gethostbyname(HOST)) == NULL) {
        perror("Errore risolvendo l'host");
        exit(EXIT_FAILURE);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    //serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    serv_addr.sin_port = htons(PORT);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Errore apertura socket");
        exit(EXIT_FAILURE);
    }    

    if (connect(sockfd, (void*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Errore connessione socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Invio della struttura itemType
    printf("Invio al server il libro: %s con %d copie\n", msg.titolo, msg.num_copie);

    if (send(sockfd, &msg, sizeof(ItemType), 0) == -1) {
        perror("Errore nell'invio del messaggio");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    int answer;
    if (recv(sockfd, &answer, sizeof(answer), 0) == -1) {
        perror("Errore nella ricezione della risposta dal server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Risposta del server: %d\n", answer);

    close(sockfd);
    return 0;
}
