#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "socket_client.h"

// void openFileAndRead(const char *file_name, int SIZE) {
//     char tmp[SIZE];
//     char *file_buffer = (char *) malloc(SIZE);
//     printf("[Client] Sending %s to the Server... ", file_name);
//     FILE *file_open = fopen(file_name, "r");
//     bzero(file_buffer, SIZE); 
//     int block_size = 1;
//     int n = 0;
//     int i = 0;
//     while((n = fread(tmp, sizeof(char), SIZE, file_open)) > 0) {
//         block_size += n;
//         // if the bytes read in is too big reallocate memeory
//         if (block_size > SIZE) {
//             file_buffer = (char *) realloc(file_buffer, block_size);
//         }
//         strcat(file_buffer, tmp);
//     }
//     printf("Data Sent %d = %d\n",i,block_size);
//     if(send(SID, file_buffer, block_size, 0) < 0) {
//         return -1;
//     }
//     free(file_buffer);
//     // bzero(file_buffer, block_size);
//     // i++;
//     // while((block_size = fread(file_buffer, sizeof(char), SIZE, file_open)) > 0) {
//     //     strcat(tmp, file_buffer);
//     //     // printf("Data Sent %d = %d\n",i,block_size);
//     //     // if(send(SID, file_buffer, block_size, 0) < 0) {
//     //     //     return -1;
//     //     // }
//     //     // bzero(file_buffer, 512);
//     //     // i++;
//     // }
//     // puts(tmp)
//     // return 0;
// }

int clientFileTransfer(int SID, const char *file_name) {
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
    printf("Data Sent %d\n",block_size);
    puts(file_buffer);
    if(send(SID, file_buffer, block_size, 0) < 0) {
        return -1;
    }
    free(file_buffer);
    return 0;
}