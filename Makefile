CC = gcc
CFLAGS = -std=c99 -Wall -O3 -lm -lgsl -lgslcblas

.PHONY = test clean

#####

test: test.x
	./$<

#####

test.x: test.c functions.o
	$(CC) $(CFLAGS) $^ -o $@

######

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c $< -o $@

#####

clean:
	rm -f *.o *.x
