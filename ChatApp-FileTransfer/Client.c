#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s [server address = %s] [port number = %s] [file name = %s]\n", argv[0], argv[1], argv[2], argv[3]);
        exit(0);
    }

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    char buffer[MAX_BUFFER_SIZE];
    FILE *file;

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        printf("ERROR opening socket");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0){
        printf("ERROR invalid server address");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("ERROR connecting");
        exit(1);
    }

    if (write(sockfd, argv[3], strlen(argv[3])) < 0){
        printf("ERROR writing to socket");
        exit(1);
    }

    file = fopen("receive.txt", "w+");
    if (!file){
        printf("ERROR opening file");
        exit(1);
    }

    bzero(buffer, MAX_BUFFER_SIZE);
    while ((n = read(sockfd, buffer, MAX_BUFFER_SIZE)) > 0) {
        if (fwrite(buffer, 1, n, file) < n){
        printf("ERROR writing to file");
        exit(1);
    }
        bzero(buffer, MAX_BUFFER_SIZE);
    }

    if (n < 0){
        printf("ERROR reading from socket");
        exit(1);
    }

    fclose(file);
    close(sockfd);
    return 0;
}
