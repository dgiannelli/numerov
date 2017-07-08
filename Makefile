CC = gcc
CFLAGS = -std=c99 -Wall -O3 -lm -lgsl -lgslcblas

.PHONY = bound zero positive test clean

#####

bound: bound.x cleanBound
	#
	./$< 5.0 40. 2500  1
	./$< 5.0 40. 5000  1
	./$< 5.0 40. 10000 1
	./$< 5.0 40. 20000 1
	./$< 5.0 40. 40000 1
	./$< 5.0 40. 80000 1
	#
	./$< 5.0 20. 40000 2
	./$< 5.0 25. 40000 2
	./$< 5.0 30. 40000 2
	./$< 5.0 35. 40000 2
	./$< 5.0 40. 40000 2
	#
	./$< 3.0 40. 40000 3
	./$< 4.0 40. 40000 3
	./$< 5.0 40. 40000 3
	./$< 6.0 40. 40000 3
	#
	./$< 5.0 40. 40000 0
	#

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

