SHELL=/bin/sh
CC=$(CROSS)gcc
PKG_CONFIG=$(CROSS)pkg-config
CFLAGS=-g -Wall
UNITY=../../Unity/src/unity.c

.PHONY: all test clean

# default - run tests
all test:  test_cloptions
	-./test_cloptions

test_internal: test_cloptions_internal
	-./test_cloptions_internal

# build tests
test_cloptions_internal: $(UNITY) test_cloptions_internal.c ../src/cloptions.h ../src/cloptions.c
	$(CC) $(CFLAGS) $(UNITY) -fno-builtin-printf test_cloptions_internal.c -o test_cloptions_internal

test_cloptions: $(UNITY) test_cloptions.c ../src/cloptions.h ../src/cloptions.c
	$(CC) $(CFLAGS) $(UNITY) -fno-builtin-printf test_cloptions.c ../src/cloptions.c -o test_cloptions

# delete compiled binaries
clean test_clean:
	- rm test_cloptions_internal
	- rm test_cloptions
