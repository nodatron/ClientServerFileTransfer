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
        char *msg = "File cannot be opened ";
        char log_msg[LOG_MSG_SIZE];
        strcat(log_msg, msg);
        strcat(log_msg, full_file_path);
        logMsg("[Server] - file transfer", log_msg, LOG_PID|LOG_CONS, LOG_USER, LOG_ERR);
        pthread_exit(NULL);
    } else {
        fwrite(file_buffer, sizeof(char), strlen(file_buffer), file_open);
        char *msg = "Transfer success to ";
        char log_msg[LOG_MSG_SIZE];
        strcat(log_msg, msg);
        strcat(log_msg, full_file_path);
        logMsg("[Server] - file transfer", log_msg, LOG_PID|LOG_CONS, LOG_USER, LOG_INFO);
    }
    fclose(file_open);
    close(client);
    bzero(tmp, FILE_BUFF_SIZE);
    bzero(full_file_path, FILE_BUFF_SIZE);
    bzero(file_buffer, FILE_BUFF_SIZE);
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
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
        logMsg("[Server] - Socket creation", "Failed", LOG_PID|LOG_CONS, LOG_USER, LOG_ERR);
    } else {
        logMsg("[Server] - Socket creation", "Success", LOG_PID|LOG_CONS, LOG_USER, LOG_INFO);
    }

    // set sockaddr_in variables
    // set the prot for communication
    server.sin_port = htons(8081);
    // use IPV4
    server.sin_family = AF_INET;
    // when INADDR_ANY is specified in the bind call, the socket will be cound to all local interfaces
    server.sin_addr.s_addr = INADDR_ANY;

    if(bind(sockDes, (struct sockaddr *) &server, sizeof(server)) < 0) {
        logMsg("[Server] - Bind", "Failed", LOG_PID|LOG_CONS, LOG_USER, LOG_ERR);
        return 1;
    } else {
        logMsg("[Server] - Bind", "Success", LOG_PID|LOG_CONS, LOG_USER, LOG_INFO);
    }

    // listen for a connection
    listen(sockDes, 3);
    // Accept an incoming connection
    logMsg("[Server] - status", "Waiting for incomming connections", LOG_PID|LOG_CONS, LOG_USER, LOG_INFO);
    connSize = sizeof(struct sockaddr_in);
    int tmp_sock;
    while(tmp_sock = accept(sockDes, (struct sockaddr *)&client, (socklen_t *)&connSize)) {
        pthread_t handler_thread;
        new_sock = malloc(4);
        *new_sock = tmp_sock;
        logMsg("[Server] - status", "Accepted new connection", LOG_PID|LOG_CONS, LOG_USER, LOG_INFO);
        if (pthread_create(&handler_thread, NULL, client_handler, (void *) new_sock) < 0) {
            logMsg("[Server] - status", "Thread Creation Error", LOG_PID|LOG_CONS, LOG_USER, LOG_ERR);
            exit(-1);
        }
        pthread_join(handler_thread, NULL);
    }

    if (tmp_sock < 0) {
        perror("failed");

        return 1;
    }
    logMsg("[Server] - status", "Shutting Down", LOG_PID|LOG_CONS, LOG_USER, LOG_INFO);
    return 0;
}