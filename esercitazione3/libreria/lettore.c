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
    if (argc < 2) { 
        printf("Usage: %s <titolo_libro>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Creazione e popolazione della struttura itemType
    ItemType msg;
    strncpy(msg.titolo, argv[1], TITOLO_LENGTH);
    msg.titolo[TITOLO_LENGTH] = '\0';  // Garantire la terminazione
    msg.tipo = 0;

    struct sockaddr_in serv_addr;
    struct hostent* server;    
    int answer;

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
    printf("Invio richiesta per il libro: %s\n", msg.titolo);

    if (send(sockfd, &msg, sizeof(ItemType), 0) == -1) {
        perror("Errore nell'invio del messaggio");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Attesa della risposta del server
    printf("In attesa della risposta dal server...\n");
    
    if (recv(sockfd, &answer, sizeof(answer), 0) == -1) {
        perror("Errore nella ricezione della risposta dal server");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("answer: %d\n", answer);
    if (answer == 1)
        printf("Il libro è disponibile! Acquisto completato.\n");
    else
        printf("Il libro non è disponibile al momento. Attendere rifornimenti.\n");

    close(sockfd);
    return 0;
}
