#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syslog.h>
#include "logger.h"

#define FILE_BUFF_SIZE 10000
#define LOG_MSG_SIZE 200

int clientFileTransfer(int SID) {
    char file_to_open[FILE_BUFF_SIZE];
    char dest_path[FILE_BUFF_SIZE];
    printf("Enter name of the file to send: ");
    scanf("%s", file_to_open);
    printf("%s\n", file_to_open);
    printf("Enter name of the dest: ");
    scanf("%s", dest_path);
    fflush(stdin);
    char *msg = "Transfering ";
    char log_msg[LOG_MSG_SIZE];
    strcat(log_msg, "Transfering ");
    strcat(log_msg, file_to_open);
    strcat(log_msg, " to ");
    strcat(log_msg, dest_path);
    logMsg("[Client] - file transfer", log_msg, LOG_PID|LOG_CONS, LOG_USER, LOG_INFO);
    char file_buffer[FILE_BUFF_SIZE];
    FILE *file_open = fopen(file_to_open, "r");
    bzero(file_buffer, FILE_BUFF_SIZE);
    fread(file_buffer, sizeof(char), FILE_BUFF_SIZE, file_open);
    printf("\n\n%s\n\n%s\n\n", file_buffer, dest_path);
    if(send(SID, dest_path, FILE_BUFF_SIZE, 0) < 0) {
        return -1;
    }
    if(send(SID, file_buffer, FILE_BUFF_SIZE, 0) < 0) {
        return -1;
    }
    close(SID);
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
        logMsg("[Client] - socket", "failed", LOG_PID|LOG_CONS, LOG_USER, LOG_ERR);
    } else {
        logMsg("[Client] - socket", "success", LOG_PID|LOG_CONS, LOG_USER, LOG_INFO);
    }

    server.sin_port = htons(8081);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;

    if (connect(SID, (struct sockaddr *)&server, sizeof(server)) < 0) {
        logMsg("[Client] - status", "Connection to server failed", LOG_PID|LOG_CONS, LOG_USER, LOG_ERR);
        printf("connect failed. Error");
        return 1;
    }
    logMsg("[Client] - status", "Connected to server", LOG_PID|LOG_CONS, LOG_USER, LOG_INFO);
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
