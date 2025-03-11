#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <time.h>

#define BUF_SIZE 1000
#define REQUESTS_NUM 5  // Number of concurrent clients

char *host_name = "127.0.0.1"; /* local host */
int port = 8000;

void client_instance(int id) {
    struct sockaddr_in serv_addr;
    struct hostent* server;
    int num, answer;

    srand(time(NULL) + id);  // Unique seed for each process
    num = rand() % 100;      // Generate a random number

    if ((server = gethostbyname(host_name)) == 0) {
        perror("Error resolving local host\n");
        exit(1);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = ((struct in_addr *)(server->h_addr))->s_addr;
    serv_addr.sin_port = htons(port);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error opening socket\n");
        exit(1);
    }

    if (connect(sockfd, (void*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("Error connecting to socket\n");
        exit(1);
    }

    printf("Client %d sending: %d\n", id, num);

    if (send(sockfd, &num, sizeof(num), 0) == -1) {
        perror("Error on send\n");
        exit(1);
    }

    if (recv(sockfd, &answer, sizeof(answer), 0) == -1) {
        perror("Error receiving response\n");
        exit(1);
    }

    printf("Client %d received: %d\n", id, answer);
    close(sockfd);
}

int main() {
    for (int cnt = 0; cnt < REQUESTS_NUM; cnt++) {
        pid_t pid = fork();
        if (pid == 0) {
            client_instance(cnt);
            exit(0);
        }
    }

    // Wait for all child processes to finish
    for (int cnt = 0; cnt < REQUESTS_NUM; cnt++) {
        wait(NULL);       // untill every process will finish
    }
    return 0;
}

