CC = gcc
CFLAGS = 
LDFLAGS =

.SUFFIXES: .c .o
.c.o:
	$(CC) -c $(CFLAGS) $<

all:
	(cd 03_tool; make)
	(cd 04_c; make)
	(cd 05_fileIO; make)
	(cd 06_fileDir; make)
	(cd 07_process; make)

clean:
	(cd 03_tool; make clean)
	(cd 04_c; make clean)
	(cd 05_fileIO; make clean)
	(cd 06_fileDir; make clean)
	(cd 07_process; make)
