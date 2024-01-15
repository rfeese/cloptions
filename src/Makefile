.PHONY: install clean

libcloptions.a: cloptions.o
	ar cr $@ cloptions.o

install: libcloptions.a cloptions.h
	cp libcloptions.a $(DESTDIR)/lib/
	cp cloptions.h $(DESTDIR)/include/

clean:
	- rm *.a
	- rm *.o
