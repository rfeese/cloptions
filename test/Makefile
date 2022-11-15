SHELL=/bin/sh
CC=$(CROSS)gcc
PKG_CONFIG=$(CROSS)pkg-config
CFLAGS=-g -Wall
UNITY=../../Unity/src/unity.c

.PHONY: all test clean

# default - run tests
all test:  test_cloptions
	-./test_cloptions


# build a test
test_cloptions: $(UNITY) test_cloptions.c ../src/cloptions.h ../src/cloptions.c
	$(CC) $(CFLAGS) $(UNITY) test_cloptions.c ../src/cloptions.c -o test_cloptions

# delete compiled binaries
clean test_clean:
	- rm test_cloptions
