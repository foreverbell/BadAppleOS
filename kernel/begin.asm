[extern _kinitialize]
[extern _tmp_pgdir]
[bits 32]

[section .mbheader]
mbheader:
  dd 0x1badb002
  dd 0x0
  dd 0xe4524ffe

KERNEL_LMA equ 0x10000
KERNEL_VMA equ 0xc0010000
CR0_PE     equ 0x1
CR0_PG     equ 0x80000000
CR4_PSE    equ 0x10

%define VA2PA(x) ((x) - KERNEL_VMA + KERNEL_LMA)

[section .text]
begin:
  ; initialize kernel stack (reuse memory below LMA)
  mov ebp, KERNEL_LMA
  mov esp, ebp

  ; turn on page size extension (PSE) for 4Mbytes pages
  mov eax, cr4
  or  eax, CR4_PSE
  mov cr4, eax

  ; initialize paging, notice cr3 requires physical address
  mov eax, VA2PA(_tmp_pgdir)
  mov cr3, eax

  ; turn on paging
  mov eax, cr0
  or  eax, CR0_PE | CR0_PG
  mov cr0, eax

  jmp _kinitialize
