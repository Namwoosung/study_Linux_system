CC = gcc
CFLAGS = 
LDFLAGS = 

.SUFFIXS: .c .o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = hello output input args string

all:$(ALL)

hello: hello.o
	$(CC) -o $@ $< $(LSFALGS)

output: output.o
	$(CC) -o $@ $< $(LSFALGS)

input: input.o
	$(CC) -o $@ $< $(LSFALGS)

args: args.o
	$(CC) -o $@ $< $(LSFALGS)

string: string.o
	$(CC) -o $@ $< $(LSFALGS)

clean:
	rm -rf *.o $(ALL)
