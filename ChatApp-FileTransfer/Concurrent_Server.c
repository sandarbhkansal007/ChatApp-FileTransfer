#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

void handle_client(int newsockfd) {
    char buffer[MAX_BUFFER_SIZE];
    int n;

    bzero(buffer, MAX_BUFFER_SIZE);
    n = read(newsockfd, buffer, MAX_BUFFER_SIZE);
    if (n < 0){
        printf("ERROR reading from socket");
        exit(1);
    }

    printf("Requested file: %s\n", buffer);

    FILE *file = fopen(buffer, "r+");
    if (!file) {
        printf("ERROR opening file");
        exit(1);
    }

    bzero(buffer, MAX_BUFFER_SIZE);
    while ((n = fread(buffer, 1, MAX_BUFFER_SIZE, file)) > 0) {
        if (write(newsockfd, buffer, n) < 0){
            printf("ERROR writing to socket");
            exit(1);
        }
        bzero(buffer, MAX_BUFFER_SIZE);
    }

    fclose(file);
    close(newsockfd);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [port_number]\n", argv[0]);
        exit(1);
    }

    int sockfd, newsockfd, portno, pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        printf("ERROR opening socket");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("ERROR on binding");
        exit(1);
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0){
            printf("ERROR on accept");
            exit(1);
        }

        pid = fork();
        if (pid < 0){
           printf("ERROR on fork");
           exit(1);
        }

        if (pid == 0) {
            close(sockfd);
            handle_client(newsockfd);
            exit(0);
        } 
        else {
            close(newsockfd);
        }
    }

    close(sockfd);
    return 0;
}