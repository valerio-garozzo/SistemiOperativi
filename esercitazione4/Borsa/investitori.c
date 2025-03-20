#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define BUF_SIZE 1000

char *host_name = "127.0.0.1"; /* Localhost */
int port = 8000;

int main() {
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buf[BUF_SIZE];

    if ((server = gethostbyname(host_name)) == NULL) {
        perror("Errore risoluzione host");
        exit(1);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    serv_addr.sin_port = htons(port);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Errore apertura socket");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Errore connessione socket");
        exit(1);
    }

    // Richiede la lista dei titoli al server
    if (send(sockfd, "I", 2, 0) == -1) {
        perror("Errore invio richiesta titoli");
        exit(1);
    }

    // Riceve e mostra la lista dei titoli
    if (recv(sockfd, buf, BUF_SIZE, 0) == -1) {
        perror("Errore ricezione lista titoli");
        exit(1);
    }

    printf("Lista titoli disponibili:\n%s", buf);

    // Chiede all'utente quale agente scegliere
    printf("Inserisci il nome dell'agente da cui acquistare: ");
    char agente[30];
    fgets(agente, sizeof(agente), stdin);
    agente[strcspn(agente, "\n")] = '\0'; // Rimuove il newline

    // Invia il nome dell'agente al server
    if (send(sockfd, agente, strlen(agente) + 1, 0) == -1) {
        perror("Errore invio scelta acquisto");
        exit(1);
    }

    printf("Acquisto inviato al server. Chiusura connessione.\n");

    close(sockfd);
    return 0;
}




