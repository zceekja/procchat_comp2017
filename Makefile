CC=gcc
CFLAGS=-Wall  -Werror -Wvla -std=gnu11 -fsanitize=address
PFLAGS=-fprofile-arcs -ftest-coverage
DFLAGS=-g
HEADERS=server.h
SRC=server.c

procchat: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(DFLAGS) $(SRC) -o $@ 

test:
	$(CC) $(CFLAGS) $(PFLAGS) $(SRC) -o server
	$(CC)  clientA.c -o clientA
	$(CC)  clientB.c -o clientB
	$(CC)  clientC.c -o clientC
	./test_all.sh
clean:
	rm -f procchat
	rm -f server
	rm -f clientA
	rm -f clientB
	rm -f clientC
	rm -f outA.txt
	rm -f outB.txt
	rm -f outC.txt
	rm -f gevent
