#x64
.text
.global aae_exit
.global aae_write
.global _start




aae_exit:
	pushq %rax
	pushq %rbx
	movq $1, %rax
	movq %rdi, %rbx
	int $0x80
	popq %rbx
	popq %rax
	retq



aae_write:
	pushq %rax
	pushq %rbx
	pushq %rcx
	movq $4, %rax
	movq %rdi, %rbx
	movq %rsi, %rcx
	int $0x80
	popq %rcx
	popq %rbx
	popq %rax
	retq


_start:
	callq aae_main
	movq %rax, %rdi
	callq aae_exit
	retq
