CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -pedantic
IFLAGS := -I.
LFLAGS :=

ifeq ($(OS),Windows_NT)
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Linux)
		LFLAGS += -lX11 -lX11-xcb -lxcb -lvulkan
	endif
endif

.PHONY: test
test:
	$(CC) $(CFLAGS) test.c -o test $(IFLAGS) $(LFLAGS)
	./test
	rm -f test

lepkc:
	$(CC) -std=c99 -pedantic -O3 lepk_compiler.c -o lepkc

lepkc_install: lepkc
	cp -f lepkc /usr/bin/lepkc
