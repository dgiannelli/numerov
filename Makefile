CC = gcc
CFLAGS = -std=c99 -Wall -O3 -lm -lgsl -lgslcblas

.PHONY = test clean

#####

test: test.x
	./$<

#####

test.x: test.c potential.o linspace.o numerov.o
	$(CC) $(CFLAGS) $^ -o $@

######

potential.o: potential.c potential.h
	$(CC) $(CFLAGS) -c $< -o $@

linspace.o: linspace.c linspace.h
	$(CC) $(CFLAGS) -c $< -o $@

numerov.o: numerov.c numerov.h potential.h
	$(CC) $(CFLAGS) -c $< -o $@

#####

clean:
	rm -f *.o *.x
