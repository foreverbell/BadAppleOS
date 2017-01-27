[extern _kinitialize]
[extern _tmp_pgdir]
[section .text]
[bits 32]

; multiboot header
; TODO: test
dd 0x1badb002
dd 0x0
dd 0xe4524ffe

begin:
  ; initialize paging, notice cr3 requires physical address, so we need to
  ; translate _tmp_pgdir from virtual address to physical address.
  mov eax, _tmp_pgdir
  sub eax, 0xc0000000   ; 0xc0000000 is kernel base virtual address
  add eax, 0x10000      ; 0x10000 is the physical address where kernel is loaded to.
  mov cr3, eax

  mov eax, cr0
  or eax, 0x80000001    ; CR0_PG | CR0_PE
  mov cr0, eax

  mov eax, _kinitialize
  jmp eax
