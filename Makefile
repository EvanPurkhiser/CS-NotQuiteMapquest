all: nqmq nqmq.dat

clean:
	rm nqmq nqmq.dat

nqmq.dat:
	wget http://www.cs.uakron.edu/~toneil/cs477/Labs/nqmq.dat
	touch $@

nqmq: nqmq.c
	gcc -Wall -std=c99 -l pthread -o nqmq nqmq.c

