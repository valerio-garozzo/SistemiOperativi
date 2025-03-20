#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define BUF_SIZE 1000

char *host_name = "127.0.0.1"; /* Localhost */
int port = 8000;

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: agentidiborsa <nome> <quantità> <prezzo_corrente> <prezzo_minimo>\n");
        exit(1);
    }

    char nome_agente[30];
    int quantità = atoi(argv[2]);
    int prezzo_corrente = atoi(argv[3]);
    int prezzo_minimo = atoi(argv[4]);

    strcpy(nome_agente, argv[1]);

    struct sockaddr_in serv_addr;
    struct hostent *server;
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

    char buf[BUF_SIZE];
    snprintf(buf, BUF_SIZE, "A %s %d %d %d", nome_agente, quantità, prezzo_corrente, prezzo_minimo);

    if (send(sockfd, buf, strlen(buf) + 1, 0) == -1) {
        perror("Errore invio dati");
        exit(1);
    }

    // Attesa del ricavo finale dal server
    if (recv(sockfd, buf, BUF_SIZE, 0) == -1) {
        perror("Errore ricezione ricavo");
        exit(1);
    }

    printf("Il server ha risposto: \"%s\"\n", buf);
    printf("Agente %s terminato.\n", nome_agente);

    close(sockfd);
    return 0;
}




