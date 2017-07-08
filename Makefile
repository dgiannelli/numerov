CC = gcc
CFLAGS = -std=c99 -Wall -O3 -lm -lgsl -lgslcblas

.PHONY = bound zero positive test clean

#####

bound: bound.x cleanBound
	./$< 3. 40. 10000 0

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

cleanBound:
	rm -f bound*.dat

clean:
	rm -f *.o *.x *.dat

