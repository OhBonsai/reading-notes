	.file	"doublecall.c"
	.text
	.globl	Q
	.type	Q, @function
Q:
.LFB0:
	.cfi_startproc
	leaq	3(%rdi), %rax
	ret
	.cfi_endproc
.LFE0:
	.size	Q, .-Q
	.globl	P
	.type	P, @function
P:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbp
	movq	%rsi, %rdi
	call	Q
	movq	%rax, %rbx
	movq	%rbp, %rdi
	call	Q
	addq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1:
	.size	P, .-P
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-28)"
	.section	.note.GNU-stack,"",@progbits
