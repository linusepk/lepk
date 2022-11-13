CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -pedantic
IFLAGS := -I.
LFLAGS :=

ifeq ($(OS),Windows_NT)
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Linux)
		LFLAGS += -lxcb -lvulkan
	endif
endif

.PHONY: test
test:
	$(CC) $(CFLAGS) test.c -o test $(IFLAGS) $(LFLAGS)
	./test
	rm -f test
