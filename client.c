#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syslog.h>
#include "logger.h"

#define FILE_BUFF_SIZE 10000
#define SERVER_RES 20

int clientFileTransfer(int SID) {
    char server_res[SERVER_RES];
    char file_to_open[FILE_BUFF_SIZE];
    char dest_path[FILE_BUFF_SIZE];
    printf("Enter name of the file to send: ");
    scanf("%s", file_to_open);
    printf("%s\n", file_to_open);
    printf("Enter name of the dest: ");
    scanf("%s", dest_path);
    fflush(stdin);

    printf("%s\n%s", file_to_open, dest_path);
    char file_buffer[FILE_BUFF_SIZE];
    printf("[Client] Sending %s to the Server... ", file_to_open);
    FILE *file_open = fopen(file_to_open, "r");
    int block_size = 0;
    int n = 0;
    bzero(file_buffer, FILE_BUFF_SIZE);
    bzero(server_res, SERVER_RES);
    fread(file_buffer, sizeof(char), FILE_BUFF_SIZE, file_open);
    printf("\n\n%s\n\n%s\n\n", file_buffer, dest_path);
    if(send(SID, dest_path, FILE_BUFF_SIZE, 0) < 0) {
        return -1;
    }
    if(send(SID, file_buffer, FILE_BUFF_SIZE, 0) < 0) {
        return -1;
    }
    // sleep(10);
    // if (recv(SID, server_res, SERVER_RES, 0) < 0) {
    //     printf("IO error");
    //     return -1;
    // }
    close(SID);
    printf("\n%s\n", server_res);
    // free(file_buffer);
    bzero(server_res, SERVER_RES);
    bzero(file_buffer, FILE_BUFF_SIZE);
    bzero(dest_path, FILE_BUFF_SIZE);
    return 0;
}

int main(int argc, char *argv[]) {

    int SID;
    struct sockaddr_in server;
    char clientMsg[500];
    char serverMsg[500];

    SID = socket(AF_INET, SOCK_STREAM, 0);
    if (SID == -1) {
        printf("Error creatign socket");
    } else {
        printf("socket created");
    }

    server.sin_port = htons(8081);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;

    if (connect(SID, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("connect failed. Error");
        return 1;
    }
    printf("Connected to server ok!!\n");

    while(1) {
        int fileSent = clientFileTransfer(SID);
        if (fileSent == -1) {
            logMsg("[Client] - file transfer", "failed to transfer the file", LOG_PID|LOG_CONS, LOG_USER, LOG_ERR);
            exit(-1);
        }
        logMsg("[Client] - file transfer", "transfer success", LOG_PID|LOG_CONS, LOG_USER, LOG_INFO);
        break;
    }
    close(SID);
    return 0;
}
