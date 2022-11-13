CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -pedantic
IFLAGS := -I.

.PHONY: test
test:
	$(CC) $(CFLAGS) test.c -o test $(IFLAGS)
	./test
	rm -f test
