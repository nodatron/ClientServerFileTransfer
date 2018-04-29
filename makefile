server: server.o socket_server.o logger.o
	gcc -o server server.o socket_server.o logger.o -l pthread

client: client.o socket_client.o logger.o
	gcc -o client client.o socket_client.o logger.o

server_com: server.c socket_server.h logger.h
	gcc -c server.c

client_com: client.c socket_client.h logger.h
	gcc -c client.c

socket_client: socket_client.c
	gcc -c socket_client.c

socket_server: socket_server.c
	gcc -c socket_server.c

logger: logger.c
	gcc -c logger.c

clean:
	rm client.o server.o client server socket_client.o