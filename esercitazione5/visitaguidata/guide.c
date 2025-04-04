#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define BUF_SIZE 1000

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Uso: %s <nome_guida> <min> <max>\n", argv[0]);
        return 1;
    }

    char msg[BUF_SIZE];
    sprintf(msg, "GUIDA:%s %s %s", argv[1], argv[2], argv[3]);

    struct sockaddr_in serv_addr;
    struct hostent* server = gethostbyname("127.0.0.1");
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
	memcpy(&serv_addr.sin_addr.s_addr, server->h_addr_list[0], server->h_length);
    serv_addr.sin_port = htons(8000);

    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    send(sockfd, msg, strlen(msg)+1, 0);

    char buf[BUF_SIZE];
    recv(sockfd, buf, BUF_SIZE, 0);
    printf("Inizio visita con %s visitatori.\n", buf);
    close(sockfd);
    return 0;
}



