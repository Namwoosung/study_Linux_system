CC = gcc
CFLAGS = 
LDFLAGS = -lpthread

.SUFFIXES: .c .o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = print sort search exec fork thread

all : $(ALL)

print : print.o
	$(CC) -o $@ $< $(LDFLAGS)

sort : sort.o
	$(CC) -o $@ $< $(LDFLAGS)

search : search.o
	$(CC) -o $@ $< $(LDFLAGS)

exec : exec.o
	$(CC) -o $@ $< $(LDFLAGS)

fork : fork.o
	$(CC) -o $@ $< $(LDFLAGS)

thread : thread.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -rf *.o $(ALL)

