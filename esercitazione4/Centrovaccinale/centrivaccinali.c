/* centrivaccinali.c - Client per i centri vaccinali */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUF_SIZE 1000

char *host_name = "127.0.0.1";
int port = 8000;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <Centro Nome> <Numero Vaccini>\n", argv[0]);
        return 1;
    }

    char message[BUF_SIZE];
    snprintf(message, BUF_SIZE, "CENTRO %s %s", argv[1], argv[2]);

    struct sockaddr_in serv_addr;
    struct hostent *server;

    if ((server = gethostbyname(host_name)) == NULL) {
        perror("Error resolving host");
        exit(1);
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error opening socket");
        exit(1);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    serv_addr.sin_port = htons(port);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Error connecting to socket");
        exit(1);
    }

    printf("Sending request: %s\n", message);
    if (send(sockfd, message, strlen(message) + 1, 0) == -1) {
        perror("Error sending request");
        exit(1);
    }

    close(sockfd);
    return 0;
}



