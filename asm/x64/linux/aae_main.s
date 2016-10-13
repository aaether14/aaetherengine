#x64
.text
.global aae_exit
.global aae_write
.global _start




aae_exit:
	pushq %rax
	pushq %rbx
	movl $1, %eax
	movl %edi, %ebx
	int $0x80
	popq %rbx
	popq %rax
	retq



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
	movl %eax, %edi
	callq aae_exit
	retq
