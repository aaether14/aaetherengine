#x86
.text
.global aae_exit
.global aae_write
.global _start




aae_exit:
	pushl %ebx
	movl $1, %eax
	movl 8(%esp), %ebx
	int $0x80
	popl %ebx
	retl



aae_write:
	pushl %ebx
	movl $4, %eax
	movl 8(%esp), %ebx
	movl 12(%esp), %ecx
	movl 16(%esp), %edx
	int $0x80
	popl %ebx
	retl


_start:
	calll aae_main
	pushl %eax
	calll aae_exit
	popl %eax
	retl
