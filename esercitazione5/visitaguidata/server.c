#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include "list.h"

#define BUF_SIZE 1000
LIST guide_list;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* handle_connection(void* arg) {
    int sockfd = *(int*)arg;
    free(arg);
    char buf[BUF_SIZE];
    recv(sockfd, buf, BUF_SIZE, 0);

    if (strncmp(buf, "GUIDA:", 6) == 0) {
        Guide g;
        sscanf(buf, "GUIDA:%s %d %d", g.name, &g.min_visitors, &g.max_visitors);
        g.current_visitors = 0;
        g.guide_socket = sockfd;

        pthread_mutex_lock(&lock);
        guide_list = EnqueueLast(guide_list, g);
        pthread_mutex_unlock(&lock);

        printf("Guida %s registrata (da %d a %d persone) con successo!\n", g.name, g.min_visitors, g.max_visitors);

        while (1) {
            sleep(1); // Wait until visit can start
            pthread_mutex_lock(&lock);
            Guide* ptr = NULL;
            NODE* iter = guide_list;
            while (iter) {
                if (iter->item.guide_socket == sockfd) {
                    ptr = &iter->item;
                    break;
                }
                iter = iter->next;
            }
            if (ptr && ptr->current_visitors >= ptr->min_visitors) {
                sprintf(buf, "%d", ptr->current_visitors);
                send(sockfd, buf, strlen(buf)+1, 0);
                guide_list = DequeueGuideBySocket(guide_list, sockfd);
                pthread_mutex_unlock(&lock);
                break;
            }
            pthread_mutex_unlock(&lock);
        }
    } else if (strncmp(buf, "VISITATORE:", 11) == 0) {
        int num;
        sscanf(buf, "VISITATORE:%d", &num);

        pthread_mutex_lock(&lock);
        Guide* g = FindSuitableGuide(guide_list, num);
        if (!g) {
            pthread_mutex_unlock(&lock);
            strcpy(buf, "NESSUNA GUIDA DISPONIBILE");
            send(sockfd, buf, strlen(buf)+1, 0);
            close(sockfd);
            return NULL;
        }

        g->current_visitors += num;
        strcpy(buf, g->name);
        send(sockfd, buf, strlen(buf)+1, 0);
        pthread_mutex_unlock(&lock);
    }
    close(sockfd);
    return NULL;
}

int main() {
    guide_list = NewList();

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8000);

    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    listen(sockfd, 10);
    printf("Server in ascolto sulla porta 8000...\n");
    printf("...in attesa di guide o visitatori...\n");

    while (1) {
        struct sockaddr_in cli;
        socklen_t len = sizeof(cli);
        int* client_sock = malloc(sizeof(int));
        *client_sock = accept(sockfd, (struct sockaddr*)&cli, &len);
        pthread_t t;
        pthread_create(&t, NULL, handle_connection, client_sock);
        pthread_detach(t);
    }
    return 0;
}


