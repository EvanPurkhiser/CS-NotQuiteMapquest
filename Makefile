all: nqmq nqmq.dat

clean:
	rm nqmq nqmq.dat

nqmq.dat:
	wget http://www.cs.uakron.edu/~toneil/cs477/Labs/nqmq.dat
	touch $@

nqmq: nqmq.c
	gcc -Wall -std=c99 -D_XOPEN_SOURCE=700 -pthread -o nqmq nqmq.c

