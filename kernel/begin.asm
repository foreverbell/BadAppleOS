[global begin]
[extern _kinitialize]
[extern _tmp_pgdir]
[section .text]
[bits 32]

begin:
  ; magic number
  dw 0xc031

  ; initialize paging
  mov eax, _tmp_pgdir
  sub eax, 0xc0000000
  add eax, 0x10000      ; virtual address to physical address
  mov cr3, eax

  mov eax, cr0
  or eax, 0x80000001    ; CR0_PG | CR0_PE
  mov cr0, eax

  mov eax, _kinitialize
  jmp eax
