CC      := gcc
CFLAGS  := -std=c99 -g -Wall -Wextra -pedantic
IFLAGS  := -Ilibs
LFLAGS  :=
DFLAGGS :=

ifeq ($(OS),Windows_NT)
	DFLAGS += -DLEPK_WINDOW_OS_WINDOWS
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Linux)
		LFLAGS += -lX11 -lX11-xcb -lxcb -lvulkan
		DFLAGS += -DLEPK_WINDOW_OS_LINUX
	endif
endif

.PHONY: test lepkc lepkc-install
test: compile
	$(CC) $(CFLAGS) test.c -o test $(IFLAGS) $(LFLAGS) $(DFLAGS)
	./test
	rm -f test

compile:
	lepkc impls/lepk_da.c     headers/lepk_da.h     LEPK_DA_IMPLEMENTATION     libs/lepk_da.h
	lepkc impls/lepk_file.c   headers/lepk_file.h   LEPK_FILE_IMPLEMENTATION   libs/lepk_file.h
	lepkc impls/lepk_window.c headers/lepk_window.h LEPK_WINDOW_IMPLEMENTATION libs/lepk_window.h
	lepkc impls/lepk_ht.c     headers/lepk_ht.h     LEPK_HT_IMPLEMENTATION     libs/lepk_ht.h

lepkc:
	$(CC) -std=c99 -pedantic -O3 -Ilibs bins/lepk_compiler.c -o bins/lepkc

lepkc-install:
	cp -f bins/lepkc /usr/bin/lepkc
