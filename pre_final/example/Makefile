CC = gcc
CFLAGS = 
LDFLAGS = 

.SUFFIXES : .c .o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = del

all : $(ALL)

del : del.o
	$(CC) -o $@ $< $(LDFALGS)

clean :
	rm -rf *.o $(ALL) file.hole
