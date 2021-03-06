/*
 * Copyright (c) 2013 by Kyle Isom <kyle@tyrfingr.is>.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */


#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>


/*
 * chain computes the length of a collatz chain for the given starting
 * number.
 */
uint64_t
chain(uint64_t n)
{
	uint64_t	count = 1;

	while (n > 1) {
		count++;
		if (0 == n%2)
			n /= 2;
		else
			n = (n * 3) + 1;
	}

	return count;
}


/*
 * collatz computes the number of Collatz chains from one to the maximum
 * starting number where the length of the chain is greater than bound.
 */
int
main(int argc, char *argv[])
{
	struct timeval	stime, etime;
	size_t		msec = 0;
	uint64_t	bound = 40;
	uint64_t	start = 100;
	uint64_t	i;
	int		count = 0;
	int		opt;

	while ((opt = getopt(argc, argv, "b:s:")) != -1) {
		switch (opt) {
		case 'b':
			bound = (uint64_t)strtoull(optarg, NULL, 10);
			break;
		case 's':
			start = (uint64_t)strtoull(optarg, NULL, 10);
			break;
		default: /* NOT REACHED */
			abort();
		}
	}

	if (gettimeofday(&stime, NULL) == -1) {
		perror("gettimeofday");
		return EXIT_FAILURE;
	}

	for (i = 1; i <= start; i++) {
		if (chain(i) > bound)
			count++;
	}

	if (gettimeofday(&etime, NULL) == -1) {
		perror("gettimeofday");
		return EXIT_FAILURE;
	}

	msec = etime.tv_sec * 1000000;
	msec += etime.tv_usec;
	msec -= (stime.tv_sec * 1000000);
	msec -= stime.tv_usec;

	printf("Number of chains: %d\n", count);
	printf("Run time: %lu us\n", (long unsigned)msec);
}
