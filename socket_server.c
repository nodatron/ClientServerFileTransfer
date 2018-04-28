#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "socket_server.h"

char *makeFilePath(char *file_name, char *path) {
    char *prefix_path = "/var/www/intranet/";
    int byte_len = strlen(prefix_path) + strlen(path) + strlen(file_name) + 1;
    printf("%d\n", byte_len);
    char *full_path = malloc(byte_len);
    strcat(full_path, prefix_path);
    if (strcmp(path, "./") != 0) {
        strcat(full_path, path);
    }
    strcat(full_path, file_name);
    return full_path;
}

int serverFileUpload(char *file_name, char *path, int client) {
    char *full_path = makeFilePath(file_name, path);
    char file_buffer[FILE_BUFF_SIZE]; // Receiver buffer
    // char *full_file_name = strncat(tmp, file_name, (strlen(tmp) + strlen(file_name) + 1));
    // puts(full_file_name);

    // char *file_name = "/var/www/intranet/copy.html";

    // FILE *file_open = fopen(full_file_name, "w");
    // if(file_open == NULL) {
    //     printf("File %s Cannot be opened file on server.\n", full_file_name);
    // } else {
    //     bzero(file_buffer, FILE_BUFF_SIZE); 
    //     int block_size = 0;
    //     int i=0;
    //     while((block_size = recv(clientSock, file_buffer, FILE_BUFF_SIZE, 0)) > 0) {
    //         printf("Data Received %d = %d\n",i,block_size);
    //         int write_sz = fwrite(file_buffer, sizeof(char), block_size, file_open);
    //         bzero(file_buffer, FILE_BUFF_SIZE);
    //         i++;
    //     }
    // }
    // printf("Transfer Complete!\n");
    // fclose(file_open);
}