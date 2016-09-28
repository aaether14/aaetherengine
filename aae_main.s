.section .text
.global _start



_start:
	call aae_main
        movl $1, %eax
	xorl %ebx, %ebx
	int $0x80

