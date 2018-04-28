#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#define FILE_BUFF_SIZE 512

int clientFileTransfer(int SID, const char *file_name, const char *dest);

#endif /* SOCKET_CLIENT_H*/