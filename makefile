server: server.o logger.o
	gcc -o server server.o logger.o -l pthread

client: client.o logger.o
	gcc -o client client.o logger.o

server_com: server.c logger.h
	gcc -c server.c

client_com: client.c logger.h
	gcc -c client.c

logger: logger.c
	gcc -c logger.c

clean:
	rm client.o server.o client server logger.o