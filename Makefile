CC = gcc
CFLAGS = -std=c99 -Wall -O3 -lm -lgsl -lgslcblas

.PHONY = bound zero positive test clean

#####

bound: bound.x
	./$<

zero: zero.x
	./$<

positive: positive.x
	./$<

test: test.x
	./$<

#####

bound.x: bound.c functions.o
	$(CC) $(CFLAGS) $^ -o $@

zero.x: zero.c functions.o
	$(CC) $(CFLAGS) $^ -o $@

positive.x: positive.c functions.o
	$(CC) $(CFLAGS) $^ -o $@

test.x: test.c functions.o
	$(CC) $(CFLAGS) $^ -o $@

######

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c $< -o $@

#####

clean:
	rm -f *.o *.x
