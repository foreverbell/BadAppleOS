
#include <stdint.h>
#include <system.h>
#include <stdio.h>
#include <linkage.h>

asmlinkage {
void isr_handler0();
void isr_handler1();
void isr_handler2();
void isr_handler3();
void isr_handler4();
void isr_handler5();
void isr_handler6();
void isr_handler7();
void isr_handler8();
void isr_handler9();
void isr_handler10();
void isr_handler11();
void isr_handler12();
void isr_handler13();
void isr_handler14();
void isr_handler15();
void isr_handler16();
void isr_handler17();
void isr_handler18();
}

namespace isr {

void initialize(void) {
#define set_isr(n) idt::set_gate(n, (uint32_t) isr_handler##n, 0x8, 0x8e);
	set_isr(0);
	set_isr(1);
	set_isr(2);
	set_isr(3);
	set_isr(4);
	set_isr(5);
	set_isr(6);
	set_isr(7);
	set_isr(8);
	set_isr(9);
	set_isr(10);
	set_isr(11);
	set_isr(12);
	set_isr(13);
	set_isr(14);
	set_isr(15);
	set_isr(16);
	set_isr(17);
	set_isr(18);
#undef set_isr
}

const char *exception_message[] = {
	"Division By Zero Exception",            // fault
	"Debug Exception",                       // fault/trap
	"Non Maskable Interrupt Exception",      // interrupt
	"Breakpoint Exception",                  // trap
	"Into Detected Overflow Exception",      // trap
	"Out of Bounds Exception",               // fault
	"Invalid Opcode Exception",              // fault
	"No Coprocessor Exception",              // fault
	"Double Fault Exception",                // abort
	"Coprocessor Segment Overrun Exception", // fault
	"Bad TSS Exception",                     // fault
	"Segment Not Present Exception",         // fault
	"Stack Fault Exception",                 // fault
	"General Protection Fault Exception",    // fault
	"Page Fault Exception",                  // fault
	"Unknown Interrupt Exception",
	"Coprocessor Fault Exception",           // fault
	"Alignment Check Exception",             // fault
	"Machine Check Exception"                // abort
};

#pragma pack(push, 1)
struct isr_context_t {
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t exception, error_code;
    uint32_t eip, cs, eflags, user_esp, ss;
} __attribute__((packed));
#pragma pack(pop)

static void dispatcher(isr_context_t *ptr) {
	printf("Exception: %s, with error code: %d\n", exception_message[ptr->exception], ptr->error_code);
	printf("Registers:\n");
	printf("\tcs=0x%x, ds=0x%x, es=0x%x, fs=0x%x, gs=0x%x, ss=0x%x\n", ptr->cs, ptr->ds, ptr->es, ptr->fs, ptr->gs, ptr->ss);
	printf("\teax=0x%x, ebx=0x%x, ecx=0x%x, edx=0x%x\n", ptr->eax, ptr->ebx, ptr->ecx, ptr->edx);
	printf("\tesp=0x%x, ebp=0x%x, esi=0x%x, edi=0x%x\n", ptr->esp, ptr->ebp, ptr->esi, ptr->edi);
	printf("\teip=0x%x, user_esp=0x%x, eflags=0x%x\n", ptr->eip, ptr->user_esp, ptr->eflags);
	
	printf("System halted.\n");
	cpu::die();
}

} /* isr */ 

asmlinkage void isr_dispatcher(isr::isr_context_t *ptr) {
	isr::dispatcher(ptr);
}
