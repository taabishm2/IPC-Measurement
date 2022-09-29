all: client server producer_sem consumer_sem

client: client.c
	gcc client.c -o client -Wall

producer_sem: producer_sem.c
	gcc producer_sem.c -o producer_sem -Wall

consumer_sem: consumer_sem.c
	gcc consumer_sem.c -o consumer_sem -Wall

server: server.c
	gcc server.c -o server -Wall

.PHONY: clean
clean:
	rm -rf server client consumer_sem producer_sem
