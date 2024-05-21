/* _loader.c */
#include <stdint.h>
#include <binaries.h>

extern char bss;
extern char endOfBinary;

extern void execve(int pid, void (*)(), int argc, char* argv[]);
extern void haltcpu();
int main();

void * memset(void * destiny, int32_t c, uint64_t length);

int _start() {
	//Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

	// syscall getpid()
	
	//char* argv[] =  {"./shell"};
	// exceve(padre, funcion, cant args, args);
	//execve(1,&shell, 1, argv);

	//while(1)			// making the wait process
	//	haltcpu();
	return main();
	//return 0;

}


void * memset(void * destiation, int32_t c, uint64_t length) {
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destiation;

	while(length--)
		dst[length] = chr;

	return destiation;
}
