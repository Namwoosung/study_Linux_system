CC = gcc
CFLAGS = 
LDFLAGS = -lpthread

.SUFFIXES: .c .o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = prodcons_t producer consumer producer_s consumer_s prodcons dining dining2 prodcons_m prodcons_s sipc1 sipc2 sipc_mutex sipc_semaphore

all : $(ALL)

prodcons_t : prodcons_t.o
	$(CC) -o $@ $< $(LDFLAGS)

producer : producer.o
	$(CC) -o $@ $< $(LDFLAGS)

consumer : consumer.o
	$(CC) -o $@ $< $(LDFLAGS)

producer_s : producer_s.o semlib.o
	$(CC) -o $@ $^ $(LDFLAGS)

consumer_s : consumer_s.o semlib.o
	$(CC) -o $@ $^ $(LDFLAGS)

prodcons : prodcons.o
	$(CC) -o $@ $< $(LDFLAGS)

dining : dining.o
	$(CC) -o $@ $< $(LDFLAGS)

dining2 : dining2.o
	$(CC) -o $@ $< $(LDFLAGS)

prodcons_m : prodcons_m.o
	$(CC) -o $@ $< $(LDFLAGS)

prodcons_s : prodcons_s.o semlib2.o
	$(CC) -o $@ $^ $(LDFLAGS)

sipc1 : sipc1.o
	$(CC) -o $@ $< $(LDFLAGS)

sipc2 : sipc2.o
	$(CC) -o $@ $< $(LDFLAGS)

sipc_mutex : sipc_mutex.o
	$(CC) -o $@ $< $(LDFLAGS)

sipc_semaphore : sipc_semaphore.o
	$(CC) -o $@ $< $(LDFLAGS)

clean :
	rm -rf *.o $(ALL)
