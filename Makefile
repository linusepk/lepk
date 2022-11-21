CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -pedantic
IFLAGS := -Ilibs
LFLAGS :=

ifeq ($(OS),Windows_NT)
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Linux)
		LFLAGS += -lX11 -lX11-xcb -lxcb -lvulkan
	endif
endif

.PHONY: test
test: compile
	$(CC) $(CFLAGS) test.c -o test $(IFLAGS) $(LFLAGS)
	./test
	rm -f test

compile:
	lepkc impls/lepk_da.c headers/lepk_da.h LEPK_DA_IMPLEMENTATION libs/lepk_da.h
	lepkc impls/lepk_file.c headers/lepk_file.h LEPK_FILE_IMPLEMENATION libs/lepk_file.h

lepkc:
	$(CC) -std=c99 -pedantic -O3 -Ilibs bins/lepk_compiler.c -o bins/lepkc

lepkc_install:
	cp -f bins/lepkc /usr/bin/lepkc
