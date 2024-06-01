#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idt/loader.h>
#include <process.h>
#include <speaker.h>
#include <stdio.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef uint8_t (*Function)();

extern void kernelASM(uint64_t addr);
int init_process();
extern void _cli(void);

extern void _sti(void);

extern void _hlt(void);

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	screen_Initialize();
	initialize_scheduler();
	return getStackBase();
}

int divisionBy(int x, int y){
	x = x/y;
	return x;
}


int main()
{
	// to change
	mem_initialize();
	load_idt();
	 // Activate interruptions
	char* argInit[2] = {"./init", NULL};
	process_create(0,&init_process,1,argInit,0);
	return 0;
}

int init_process(){

	char* argShell[2] = {"./shell", NULL};
	process_create(1,(Function) sampleCodeModuleAddress,1,argShell,1);
	_sti();
	while(1)
		_hlt();
	return 0;
}
