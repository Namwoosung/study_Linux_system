CC = gcc
CFLAGS =
LDFLAGS = -lpthread

.SUFFIXES: .c .o
.c.o:
	$(CC) -c $(CFLAGS)  $<

ALL = order tcps tcpc udps udpc tcpc_dns myusleep select tcps_process select_thread

all : $(ALL)

oreder : order.o
	$(CC) -o $@ $< $(LDFLAGS)

tcps : tcps.o
	$(CC) -o $@ $< $(LDFLAGS)

tcpc : tcpc.o
	$(CC) -o $@ $< $(LDFLAGS)

udps : udps.o
	$(CC) -o $@ $< $(LDFLAGS)

udpc : udpc.o
	$(CC) -o $@ $< $(LDFLAGS)

tcpc_dns : tcpc_dns.o
	$(CC) -o $@ $< $(LDFLAGS)

myusleep : myusleep.o
	$(CC) -o $@ $< $(LDFLAGS)

select : select.o
	$(CC) -o $@ $< $(LDFLAGS)

tcps_process : tcps_process.o
	$(CC) -o $@ $< $(LDFLAGS)

select_thread : select_thread.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -rf *.o $(ALL)
