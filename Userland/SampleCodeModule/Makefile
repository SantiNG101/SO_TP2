include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
STATICLIBS=./libc/libc.a

all: stdlib $(MODULE)

$(MODULE): $(SOURCES) opCode
	$(GCC) $(GCCFLAGS) -T sampleCodeModule.ld _loader.c $(SOURCES) opCode.a ./libc/libc.a -o ../$(MODULE) -I./libc/include

opCode: 
	$(ASM) $(ASMFLAGS) opCode.asm -o opCode.o
	$(AR) rvs opCode.a opCode.o

stdlib:
	cd libc; make all

clean: stdclean
	rm -rf *.o

stdclean:
	cd libc; make clean

.PHONY: all clean print
