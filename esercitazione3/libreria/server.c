#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "list.h"

#define BUF_SIZE 1000
#define PORT 8000
#define TIPO_LETTORE 0
#define TIPO_CASA_ED 1

typedef struct {
    int socket;
    struct sockaddr_in client_addr;
} ClientData;

LIST libri;
LIST lettori;

void *handle_client(void *arg) {
    ClientData *data = (ClientData *)arg;
    int client_sock = data->socket;
    free(data);

    char buf[BUF_SIZE];
    ItemType msg;
    int answer = 0;

    bzero(buf, BUF_SIZE);

    // Ricezione messaggio
    if (recv(client_sock, buf, BUF_SIZE, 0) == -1) {
        perror("Errore in recv");
        close(client_sock);
        return NULL;
    }

    // Copia il messaggio nella struct msg
    memcpy(&msg, buf, sizeof(ItemType));
    //printf("Ricevuto dalla casa editrice: %d copie di %s\n", msg.num_copie, msg.titolo);
    printf("msg.tipo: %d\n", msg.tipo);

    if (msg.tipo == TIPO_LETTORE) {
        ItemType *libro = Find(libri, msg);

        printf("libro->titolo: %s\n", libro->titolo);
        printf("msg.titolo: %s\n", msg.titolo);
        if (libro->titolo == msg.titolo) {
            printf("Libro trovato\n");
            answer = 1;
            send(client_sock, &answer, sizeof(answer), 0);
            
            libro->num_copie--;
            if (libro->num_copie == 0) {
                printf("Libro %s eliminato\n", libro->titolo);
                libri = Dequeue(libri, *libro);
            }
        } else {
            answer = 0;
            send(client_sock, &answer, sizeof(answer), 0);
        }

    } else if (msg.tipo == TIPO_CASA_ED) {
        //printf("Gestione della casa editrice non implementata.\n");
        printf("Ricevuto dalla casa editrice: %d copie di %s\n", msg.num_copie, msg.titolo);

    } else {
        printf("Tipo %d non riconosciuto.\n", msg.tipo);
    }

    close(client_sock);
    return NULL;
}

int main() {
    struct sockaddr_in serv_addr, cli_addr;
    int sockfd;

    // Creazione socket
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Errore apertura socket");
        exit(1);
    }

    int options = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(options));

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Binding
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Errore in bind");
        exit(1);
    }

    // Ascolto delle connessioni
    if (listen(sockfd, 20) == -1) {
        perror("Errore in listen");
        exit(1);
    }

    printf("Server in attesa di connessioni sulla porta %d...\n", PORT);

    while (1) {
        socklen_t address_size = sizeof(cli_addr);
        int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &address_size);
        if (newsockfd == -1) {
            perror("Errore in accept");
            continue;
        }

        printf("Nuova connessione accettata\n");

        // Creazione di un thread per il client
        pthread_t thread;
        ClientData *client_data = malloc(sizeof(ClientData));
        client_data->socket = newsockfd;
        client_data->client_addr = cli_addr;
        pthread_create(&thread, NULL, handle_client, client_data);
        pthread_detach(thread);
    }

    close(sockfd);
    return 0;
}

