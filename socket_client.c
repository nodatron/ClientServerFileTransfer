#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
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
int clientFileTransfer(int SID, const char *file_name, const char *dest) {
    char tmp[FILE_BUFF_SIZE];
    char *file_buffer = (char *) malloc(FILE_BUFF_SIZE);
    printf("[Client] Sending %s to the Server... ", file_name);
    FILE *file_open = fopen(file_name, "r");
    int block_size = 0;
    int n = 0;
    while((n = fread(tmp, sizeof(char), FILE_BUFF_SIZE, file_open)) > 0) {
        block_size += n;
        // if the bytes read in is too big reallocate memeory
        if (block_size > FILE_BUFF_SIZE) {
            file_buffer = (char *) realloc(file_buffer, block_size);
        }
        strcat(file_buffer, tmp);
    }
    // char *data_to_send = (char *) malloc(block_size + strlen(dest) + 1);
    printf("Data Sent %d\n",block_size);
    puts(file_buffer);
    if(send(SID, dest, block_size, 0) < 0) {
        return -1;
    }
    if(send(SID, file_buffer, block_size, 0) < 0) {
        return -1;
    }
    free(file_buffer);
    return 0;
}