#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "list.h"

#define BUF_SIZE 1000
int port = 8000;
LIST borsa; // Lista dei titoli quotati

/* Invia la lista dei titoli all'investitore */
void inviaQuotazioni(int client_sockfd) {
    char buf[BUF_SIZE] = "Titoli quotati:\n";
    LIST tmp = borsa;

    while (tmp != NULL) {
        char titolo_info[100];
        sprintf(titolo_info, "Agente: %s, Quantità: %d, Prezzo: %d\n",
                tmp->item.nome_agente, tmp->item.quantità, tmp->item.prezzo_corrente);
        strcat(buf, titolo_info);
        tmp = tmp->next;
    }
    send(client_sockfd, buf, strlen(buf) + 1, 0);
}

/* Aggiorna il mercato dopo un acquisto */
void aggiornaMercato(char *nome_agente) {
    Titolo *titolo = Find(borsa, nome_agente);
    if (titolo) {
        titolo->quantità--;
        titolo->prezzo_corrente++;

        // Riduce i prezzi degli altri titoli
        LIST tmp = borsa;
        while (tmp != NULL) {
            if (strcmp(tmp->item.nome_agente, nome_agente) != 0)
                tmp->item.prezzo_corrente--;
            tmp = tmp->next;
        }

        // Rimuove il titolo se esaurito o sotto prezzo minimo
        if (titolo->quantità == 0 || titolo->prezzo_corrente < titolo->prezzo_minimo) {
            int ricavo = (titolo->prezzo_corrente - 1) * (titolo->quantità + 1);
            printf("Titolo %s ritirato, ricavo: %d\n", nome_agente, ricavo);

            borsa = Dequeue(borsa, nome_agente);
        }
    }
}

int main() {
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t address_size = sizeof(cli_addr);
    borsa = NewList();

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Errore apertura socket");
        exit(1);
    }

    int options = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &options, sizeof(options));

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Errore binding");
        exit(1);
    }

    if (listen(sockfd, 20) == -1) {
        perror("Errore listen");
        exit(1);
    }

    char buf[BUF_SIZE];

    while (1) {
        printf("\nIn attesa di connessioni...\n");
        int newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &address_size);
        if (newsockfd == -1) {
            perror("Errore accept");
            exit(1);
        }

        bzero(buf, BUF_SIZE);
        if (recv(newsockfd, buf, BUF_SIZE, 0) == -1) {
            perror("Errore ricezione");
            exit(1);
        }

        printf("Ricevuto: \"%s\"\n", buf);

        if (strncmp(buf, "A", 1) == 0) {
            Titolo nuovo_titolo;
            sscanf(buf + 1, "%s %d %d %d", nuovo_titolo.nome_agente,
                   &nuovo_titolo.quantità, &nuovo_titolo.prezzo_corrente, &nuovo_titolo.prezzo_minimo);
            nuovo_titolo.ricavo_totale = 0;

            borsa = EnqueueOrdered(borsa, nuovo_titolo);
            sprintf(buf, "Titolo %s registrato con successo.", nuovo_titolo.nome_agente);
            send(newsockfd, buf, strlen(buf) + 1, 0);
        } else if (strncmp(buf, "I", 1) == 0) {
            inviaQuotazioni(newsockfd);
            if (recv(newsockfd, buf, BUF_SIZE, 0) == -1) {
                perror("Errore ricezione acquisto");
                exit(1);
            }
            printf("Investitore acquista da: %s\n", buf);
            aggiornaMercato(buf);
        }

        close(newsockfd);
    }

    close(sockfd);
    return 0;
}





