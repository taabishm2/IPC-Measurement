CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lrt -lpthread

all: producer consumer

producer: producer.c shm.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

consumer: consumer.c shm.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf consumer producer
