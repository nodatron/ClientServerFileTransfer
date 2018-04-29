#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syslog.h>
#include "socket_client.h"
#include "logger.h"

int fileSend(int SID, char *file_name, int SIZE) {
    char tmp[SIZE];
    char *file_buffer = (char *) malloc(SIZE);
    printf("[Client] Sending %s to the Server... ", file_name);
    FILE *file_open = fopen(file_name, "r");
    bzero(file_buffer, SIZE); 
    int block_size = 1;
    int n = 0;
    int i = 0;
    while((n = fread(tmp, sizeof(char), SIZE, file_open)) > 0) {
        block_size += n;
        // if the bytes read in is too big reallocate memeory
        if (block_size > SIZE) {
            file_buffer = (char *) realloc(file_buffer, block_size);
        }
        strcat(file_buffer, tmp);
    }
    printf("Data Sent %d\n",block_size);
    puts(file_buffer);
    if(send(SID, file_buffer, block_size, 0) < 0) {
        return -1;
    }
    free(file_buffer);
    // char *filename = "/home/niall/Desktop/test.html";
    // char file_buffer[512]; 
    // printf("[Client] Sending %s to the Server... ", filename);
    // FILE *file_open = fopen(filename, "r");
    // bzero(file_buffer, 512); 
    // int block_size,i=0; 
    // while((block_size = fread(file_buffer, sizeof(char), 512, file_open)) > 0) {
    //     printf("Data Sent %d = %d\n",i,block_size);
    //     if(send(SID, file_buffer, block_size, 0) < 0) {
    //         return -1;
    //     }
    //     bzero(file_buffer, 512);
    //     i++;
    // }
    // return 0;
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
        // int fileSent = fileSend(SID, "/home/niall/Desktop/test.html", 5);
        int fileSent = clientFileTransfer(SID);
        if (fileSent == -1) {
            logMsg("[Client] - file transfer", "failed to transfer the file", LOG_PID|LOG_CONS, LOG_USER, LOG_ERR);
            exit(-1);
        }
        // if (recv(SID, serverMsg, 500, 0) < 0) {
        //     printf("IO error");
        //     break;
        // }
        logMsg("[Client] - file transfer", "transfer success", LOG_PID|LOG_CONS, LOG_USER, LOG_INFO);
        break;
        // printf("\nEnter message: ");
        // scanf("%s", clientMsg);

        // // Send some data
        // if (send(SID, clientMsg, strlen(clientMsg), 0) < 0) {
        //     printf("Send failed");
        //     return 1;
        // }

        // if (recv(SID, serverMsg, 500, 0) < 0) {
        //     printf("IO error");
        //     break;
        // }

        // printf("\nServer sent: %s", serverMsg);
    }
    close(SID);
    return 0;
}
