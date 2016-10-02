#x64
.text
.global _start
.global aae_write



aae_write:
	pushq %rax
	pushq %rbx
	pushq %rcx
	movl $4, %eax
	movl %edi, %ebx
	movq %rsi, %rcx
	int $0x80
	popq %rcx
	popq %rbx
	popq %rax
	retq


_start:
	callq aae_main
	movl %eax, %ebx
	movl $1, %eax
	int $0x80
