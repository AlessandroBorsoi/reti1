CC=gcc
CFLAGS+=-Wall -Wextra -std=c11 -pedantic -g -I"$(PWD)/include"
export CC
export CFLAGS

.PHONY: all bin apps clean test

all: bin test apps

bin:
	cd bin && $(MAKE) all

apps:
	cd apps && $(MAKE) all

test: bin
	cd test && $(MAKE) all

clean:
	cd bin && $(MAKE) clean
	cd apps && $(MAKE) clean
	cd test && $(MAKE) clean