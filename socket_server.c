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
    char *full_path = malloc(byte_len);
    strcat(full_path, prefix_path);
    if (strcmp(path, "./") != 0) {
        strcat(full_path, path);
    }
    strcat(full_path, file_name);
    return full_path;
}

int serverFileUpload(char *file_name, char *path, int client) {
    // char *full_path = makeFilePath(file_name, path);
    char tmp[FILE_BUFF_SIZE];
    char *full_file_path = (char *) malloc(FILE_BUFF_SIZE);
    char *file_buffer = (char *) malloc(FILE_BUFF_SIZE);

    bzero(tmp, FILE_BUFF_SIZE); 
    int block_size = 0;
    int n = 0;
    int i = 0;
    while((n = recv(client, tmp, FILE_BUFF_SIZE, 0)) > 0) {
        if (i == 0) {
            strcat(full_file_path, tmp);
        } else {
            block_size += n;
            if (block_size > FILE_BUFF_SIZE) {
                file_buffer = (char *) realloc(file_buffer, block_size);
            }
            strcat(file_buffer, tmp);
        }
        i++;
    }
    printf("\nFILE: %s\nBUFF: %s\n%d\n", full_file_path, file_buffer, i);
    FILE *file_open = fopen(full_file_path, "w");
    // FILE *file_open = fopen(full_path, "w");
    if(file_open == NULL) {
        printf("File %s Cannot be opened file on server.\n", full_file_path);
        return -1;
    } else {
    //     bzero(tmp, FILE_BUFF_SIZE); 
    //     int block_size = 0;
    //     int n = 0;
    //     int i = 0;
    //     while((n = recv(client, tmp, FILE_BUFF_SIZE, 0)) > 0) {
    //         if (i == 0) {
    //             strcat(full_file_path, tmp);
    //         } else {
    //             block_size += n;
    //             if (block_size > FILE_BUFF_SIZE) {
    //                 file_buffer = (char *) realloc(file_buffer, block_size);
    //             }
    //             strcat(file_buffer, tmp);
    //             printf("\nLine: %s\nIteration: %d\n", tmp, i);
    //         }
    //         i++;
    //     }
        fwrite(file_buffer, sizeof(char), block_size, file_open);
    //     puts(file_buffer);
    }
    bzero(tmp, FILE_BUFF_SIZE);
    free(full_file_path);
    free(file_buffer);
    printf("Transfer Complete!\n");
    // fclose(file_open);
    return 0;
}