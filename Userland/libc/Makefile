# Se debe completar para que arme el módulo del libc
include ../Makefile.inc

MODULE=0001-libcModule.bin
SOURCES=$(wildcard [^_]*.c)

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T libcModule.ld $(SOURCES) -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean print