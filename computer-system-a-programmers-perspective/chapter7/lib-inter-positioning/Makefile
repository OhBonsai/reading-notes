CC = gcc
CFLAGS = -Wall

all: intr intl intc

#####
# Run-time interpositioning
#
intr: int.c mymalloc.c
	$(CC) $(CFLAGS) -DRUNTIME -shared -fpic -o mymalloc.so mymalloc.c -ldl
	$(CC) $(CFLAGS) -o intr int.c 

runr: intr
	(LD_PRELOAD="./mymalloc.so" ./intr 10 100 1000)	

#####
# Link-time interpositioning
#
intl: int.c mymalloc.c
	$(CC) $(CFLAGS) -DLINKTIME -c mymalloc.c
	$(CC) $(CFLAGS) -c int.c
	$(CC) $(CFLAGS) -Wl,--wrap,malloc -Wl,--wrap,free -o intl int.o mymalloc.o

runl: intl
	./intl 10 100 1000



#####
# Compile-time interposition
#
intc: int.c mymalloc.c
	$(CC) $(CFLAGS) -DCOMPILETIME -c mymalloc.c
	$(CC) $(CFLAGS) -I. -o intc int.c mymalloc.o

runc: intc
	./intc 10 100 1000


clean:
	rm -f *~ intr intl intc *.so *.o

