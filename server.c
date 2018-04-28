#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syslog.h>
#include "logger.h"
#include "socket_server.h"

int main(int argc, char *argv[]) {
    int sockDes;
    int clientSock;
    int connSize;
    int READSIZE;

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

    clientSock = accept(sockDes, (struct sockaddr *)&client, (socklen_t *)&connSize);
    if (clientSock < 0) {
        perror("Cant establish connection");
        return 1;
    } else {
        printf("Connection from client accepted");
    }

    while(1) {
        int response = serverFileUpload("copy.html", "./", clientSock);
        if (response == -1) {
            logMsg("[Server] - file transfer", "failed to copy the file", LOG_PID|LOG_CONS, LOG_USER, LOG_ERR);
            exit(-1);
        }
        logMsg("[Server] - file transfer", "complete transfer of file", LOG_PID|LOG_CONS, LOG_USER, LOG_INFO);
        break;
    }

    if(READSIZE == 0) {
        puts("Client Disconnected");
        fflush(stdout);
    } else if (READSIZE == -1) {
        perror("read error");
    }
    return 0;
}