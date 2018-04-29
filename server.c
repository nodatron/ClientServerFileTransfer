#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syslog.h>
#include <pthread.h>
#include "logger.h"

#define FILE_BUFF_SIZE 10000

pthread_mutex_t lock;

void *client_handler(void *ptr) {
    pthread_mutex_lock(&lock);
    int client = *(int *)ptr;
    printf("\nThis is the client ID: %d\n", client);
    char tmp[FILE_BUFF_SIZE];
    char full_file_path[FILE_BUFF_SIZE];
    char file_buffer[FILE_BUFF_SIZE];

    bzero(tmp, FILE_BUFF_SIZE);
    bzero(full_file_path, FILE_BUFF_SIZE);
    bzero(file_buffer, FILE_BUFF_SIZE);
    int block_size = 0;
    int n = 0;
    int i = 0;
    while((n = recv(client, tmp, FILE_BUFF_SIZE, 0)) > 0) {
        if (i == 0) {
            strcat(full_file_path, tmp);
        } else {
            strcat(file_buffer, tmp);
        }
        i++;
    }
    write(client, "SUCCESS", strlen("SUCCESS"));
    printf("\nFILE: %s\nBUFF: %s\n%d\n", full_file_path, file_buffer, i);
    FILE *file_open = fopen(full_file_path, "w");
    if(file_open == NULL) {
        printf("File %s Cannot be opened file on server.\n", full_file_path);
        return -1;
    } else {
        fwrite(file_buffer, sizeof(char), strlen(file_buffer), file_open);
        // write(client, "SUCCESS", strlen("SUCCESS"));
    }
    // write(client, "SUCCESS", strlen("SUCCESS"));
    fclose(file_open);
    close(client);
    bzero(tmp, FILE_BUFF_SIZE);
    bzero(full_file_path, FILE_BUFF_SIZE);
    bzero(file_buffer, FILE_BUFF_SIZE);
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
    return 0;
}

int main(int argc, char *argv[]) {
    int sockDes;
    int clientSock;
    int connSize;
    int READSIZE;
    int *new_sock;

    struct sockaddr_in server, client;
    sockDes = socket(AF_INET, SOCK_STREAM, 0);
    if (sockDes == -1) {
        printf("Could not create socket");
    } else {
        printf("Socket Successfully Created!!");
    }

    // set sockaddr_in variables
    // set the prot for communication
    server.sin_port = htons(8081);
    // use IPV4
    server.sin_family = AF_INET;
    // when INADDR_ANY is specified in the bind call, the socket will be cound to all local interfaces
    server.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockDes, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("Bind issue!!");
        return 1;
    } else {
        printf("Bind Complete!!");
    }

    // listen for a connection
    listen(sockDes, 3);
    // Accept an incoming connection
    printf("Waiting for incoming connection from Client>>");
    connSize = sizeof(struct sockaddr_in);
    int tmp_sock;
    while(tmp_sock = accept(sockDes, (struct sockaddr *)&client, (socklen_t *)&connSize)) {
        pthread_t handler_thread;
        new_sock = malloc(4);
        *new_sock = tmp_sock;
        printf("\nThis is the client ID: %d %d\n", tmp_sock, new_sock);
        if (pthread_create(&handler_thread, NULL, client_handler, (void *) new_sock) < 0) {
            fprintf(stderr, "error");
            exit(-1);
        }
        pthread_join(handler_thread, NULL);
    }

    if (tmp_sock < 0) {
        perror("failed");
        return 1;
    }
    return 0;
}