/* server.c - Server per la gestione della distribuzione dei vaccini */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>  // manipolazione degli indirizzi IP
#include <sys/socket.h> // creazione e gestione dei socket di rete
#include <sys/types.h>
#include <pthread.h>    // programmazione multithreading

#define BUF_SIZE 1000
#define MAX_CENTERS 20
#define MAX_SUPPLIERS 20

int port = 8000; 

// Struttura per i centri vaccinali
typedef struct {
    char name[50];
    int request;
} VaccinationCenter;

// Struttura per i fornitori
typedef struct {
    char name[50];
    int available_vaccines;
    int min_required;
} Supplier;

VaccinationCenter centers[MAX_CENTERS];
Supplier suppliers[MAX_SUPPLIERS];
int center_count = 0;
int supplier_count = 0;

pthread_mutex_t lock;

void *handle_client(void *arg) {
    int client_sock = *(int *)arg;
    free(arg);

    char buffer[BUF_SIZE];
    recv(client_sock, buffer, BUF_SIZE, 0);

    // Determinare se è un fornitore o un centro vaccinale
    if (strncmp(buffer, "FORNITORE", 9) == 0) {
        // Legge i dati del fornitore
        Supplier supplier;
        sscanf(buffer, "FORNITORE:\nnome fornitore: %s\nvaccini disponibili: %d\nrichiesta minima: %d", supplier.name, &supplier.available_vaccines, &supplier.min_required);

        pthread_mutex_lock(&lock);
        suppliers[supplier_count++] = supplier;
        pthread_mutex_unlock(&lock);
    } else if (strncmp(buffer, "CENTRO", 6) == 0) {
        // Legge i dati del centro vaccinale
        VaccinationCenter center;
        sscanf(buffer, "CENTRO %s %d", center.name, &center.request);

        pthread_mutex_lock(&lock);
        centers[center_count++] = center;
        pthread_mutex_unlock(&lock);
    }
    close(client_sock);
    return NULL;
}

int main() {
    int sockfd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t addr_size = sizeof(cli_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(sockfd, 10);

    pthread_mutex_init(&lock, NULL);
    printf("Server in attesa di connessioni sulla porta %d...\n", port);

    while (1) {
        int *new_sock = malloc(sizeof(int));
        *new_sock = accept(sockfd, (struct sockaddr *)&cli_addr, &addr_size);
        
        printf("Nuova connessione accettata\n");
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void *)new_sock);
        pthread_detach(thread);
    }
    pthread_mutex_destroy(&lock);
    close(sockfd);
    return 0;
}



