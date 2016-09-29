.text
.global _start
.global aae_fwrite




aae_fwrite:
	movq $4, %rax
	movq %rdi, %rbx
	movq %rsi, %rcx
	#rdx already goes into rdx
	int $0x80
	ret


_start:
	call aae_main
        movl $1, %eax
	xorl %ebx, %ebx
	int $0x80


