#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "socket_client.h"


/*
    int clientFileTransfer(int SID, const char *file_name)
        - SID - the sokcet ID
        - file_name - the file to open including path
    returns
        - 0 - success
        - -1 - error
*/
int clientFileTransfer(int SID) {
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
    fread(file_buffer, sizeof(char), FILE_BUFF_SIZE, file_open);
    printf("\n\n%s\n\n%s\n\n", file_buffer, dest_path);
    if(send(SID, dest_path, FILE_BUFF_SIZE, 0) < 0) {
        return -1;
    }
    if(send(SID, file_buffer, FILE_BUFF_SIZE, 0) < 0) {
        return -1;
    }
    // if (recv(SID, tmp, FILE_BUFF_SIZE, 0) < 0) {
    //     printf("IO error");
    //     return -1;
    // }
    // free(file_buffer);
    bzero(file_buffer, FILE_BUFF_SIZE);
    bzero(dest_path, FILE_BUFF_SIZE);
    return 0;
}