CC=gcc
CFLAGS=-Wall
LDFLAGS=-lrt

all: producer consumer

producer: producer.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

consumer: consumer.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -rf consumer producer
