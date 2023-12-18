CC = gcc
CFLAGS = 
LDFLAGS = -lpthread

.SUFFIXES: .c .o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = print

all : $(ALL)

print : print.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -rf *.o $(ALL)

