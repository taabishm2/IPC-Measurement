all: client server

client: client.c
	gcc client.c -o client -Wall

server: server.c
	gcc server.c -o server -Wall

.PHONY: clean
clean:
	rm -rf server client
