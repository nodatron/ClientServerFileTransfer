server: server.o
	gcc -o server server.o

client: client.o mysocket.o
	gcc -o client client.o mysocket.o

server_com: server.c
	gcc -c server.c

client_com: client.c mysocket.h
	gcc -c client.c

mysocket: mysocket.c
	gcc -c mysocket.c

clean:
	rm client.o server.o client server mysocket.o