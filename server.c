#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syslog.h>
#include <pthread.h>
#include "logger.h"
#include "socket_server.h"

void *print_msg(void *ptr) {
    sleep(3);
    char *msg = (char *) ptr;
    printf("%s\n", msg);
}

void *client_handler(void *ptr) {
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
    close(client);
    // write(client, "SUCCESS", strlen("SUCCESS"));
    printf("\nFILE: %s\nBUFF: %s\n%d\n", full_file_path, file_buffer, i);
    FILE *file_open = fopen(full_file_path, "w");
    if(file_open == NULL) {
        printf("File %s Cannot be opened file on server.\n", full_file_path);
        return -1;
    } else {
        fwrite(file_buffer, sizeof(char), strlen(file_buffer), file_open);
        // write(client, "SUCCESS", strlen("SUCCESS"));
    }
    fclose(file_open);
    bzero(tmp, FILE_BUFF_SIZE);
    bzero(full_file_path, FILE_BUFF_SIZE);
    bzero(file_buffer, FILE_BUFF_SIZE);
    return 0;
}

int main(int argc, char *argv[]) {
    int sockDes;
    int clientSock;
    int connSize;
    int READSIZE;

    pthread_t threads[3];
    char *m1 = "Hi";
    char *m2 = "Hey";
    int iret1, iret2;
    int *new_sock;

    // iret1 = pthread_create(&threads[0], NULL, print_msg, (void *)m1);
    // if(iret1) {
    //     fprintf(stderr, "error");
    //     exit(-1);
    // }
    // pthread_join(threads[0], NULL);
    // iret2 = pthread_create(&threads[1], NULL, print_msg, (void *)m2);
    // if(iret2) {
    //     fprintf(stderr, "error");
    //     exit(-1);
    // }
    // pthread_join(threads[1], NULL);

    struct sockaddr_in server, client;
    char msg[500];
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
    }

    // clientSock = accept(sockDes, (struct sockaddr *)&client, (socklen_t *)&connSize);
    // if (clientSock < 0) {
    //     perror("Cant establish connection");
    //     return 1;
    // } else {
    //     printf("Connection from client accepted");
    // }

    // while(1) {
    //     int response = serverFileUpload("copy.html", "./", clientSock);     
    //     if (response == -1) {
    //         logMsg("[Server] - file transfer", "failed to copy the file", LOG_PID|LOG_CONS, LOG_USER, LOG_ERR);
    //         exit(-1);
    //     }
    //     // write(clientSock, "SUCCESS", strlen("SUCCESS"));
    //     logMsg("[Server] - file transfer", "complete transfer of file", LOG_PID|LOG_CONS, LOG_USER, LOG_INFO);
    //     break;
    // }

    // if(READSIZE == 0) {
    //     puts("Client Disconnected");
    //     fflush(stdout);
    // } else if (READSIZE == -1) {
    //     perror("read error");
    // }
    if (tmp_sock < 0) {
        perror("failed");
        return 1;
    }
    return 0;
}