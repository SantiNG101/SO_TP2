include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
STATICLIBS=./libc/libc.a

all: stdlib $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld _loader.c $(SOURCES) ./libc/libc.a -o ../$(MODULE) -I./libc/include

stdlib:
	cd libc; make all

clean: 
	rm -rf *.o

.PHONY: all clean print
