CC=$(CROSS)gcc
PKG_CONFIG=$(CROSS)pkg-config
CFLAGS=-g -Wall

.PHONY: all clean install test test_clean bash_completion

#binaries
all: example

example: example.c src/cloptions.o
	$(CC) $(LDFLAGS) example.c src/cloptions.o $(LIBS) -o $@

install:
	make --directory src $@

#delete compiled binaries
clean:
	make --directory src $@
	- rm example
	- rm bash-completion.sh

bash-completion: example
	echo "#/usr/bin/env bash" > bash-completion.sh
	./example --bash-completion >> bash-completion.sh
	echo "Now, source bash-completion.sh."

#buid and run tests
test:
	$(MAKE) --directory test $@

test_clean:
	$(MAKE) --directory test $@

