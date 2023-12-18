CC = gcc
CFLAGS = 
LDFLAGS = -lpthread

.SUFFIXES: .c .o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = sig thread myps server.c client.c

all : $(ALL)

sig : sig.o
	$(CC) -o $@ $< $(LDFLAGS)

thread : thread.o
	$(CC) -o $@ $< $(LDFLAGS)

myps : myps.o
	$(CC) -o $@ $< $(LDFLAGS)

server : server.o
	$(CC) -o $@ $< $(LDFLAGS)

client : client.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -rf *.o $(ALL)

