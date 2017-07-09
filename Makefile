CC = gcc
CFLAGS = -std=c99 -Wall -O3 -lm -lgsl -lgslcblas

.PHONY = bound zero positive test clean

.PHONY = boundPlots

#####

boundPlots: boundPlots.tex
	pdflatex -halt-on-error $<

#####

bound: bound.x cleanBound
	#
	./$< 5.0 50. 125  1
	./$< 5.0 50. 250  1
	./$< 5.0 50. 500  1
	./$< 5.0 50. 1000 1
	./$< 5.0 50. 2000 1
	./$< 5.0 50. 4000 1
	#
	./$< 5.0 25. 4000 2
	./$< 5.0 30. 4000 2
	./$< 5.0 35. 4000 2
	./$< 5.0 40. 4000 2
	./$< 5.0 45. 4000 2
	./$< 5.0 50. 4000 2
	#
	./$< 2.5 50. 4000 3
	./$< 3.0 50. 4000 3
	./$< 3.5 50. 4000 3
	./$< 4.0 50. 4000 3
	./$< 4.5 50. 4000 3
	./$< 5.0 50. 4000 3
	#
	./$< 5.0 50. 4000 0
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
	rm -f *.o *.x *.log *.aux *.dat

