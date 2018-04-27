server: server.o
	gcc -o server server.o

client: client.o
	gcc -o client client.o

server_com: server.c
	gcc -c server.c

client_com: client.c
	gcc -c client.c

clean:
	rm client.o server.o client server