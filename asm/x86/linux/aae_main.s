#x86
.text
.global aae_exit
.global aae_write
.global _start




aae_exit:
	pushl %eax
	pushl %ebx
	movl $1, %eax
	movl %edi, %ebx
	int $0x80
	popl %ebx
	popl %eax
	retl



aae_write:
	pushl %eax
	pushl %ebx
	pushl %ecx
	movl $4, %eax
	movl %edi, %ebx
	movl %esi, %ecx
	int $0x80
	popl %ecx
	popl %ebx
	popl %eax
	retl


_start:
	calll aae_main
	movl %eax, %edi
	calll aae_exit
	retl
