	.file	"localstore.c"
	.text
	.globl	swap_add
	.type	swap_add, @function
swap_add:
.LFB0:
	.cfi_startproc
	movq	(%rdi), %rdx
	movq	(%rsi), %rax
	movq	%rax, (%rdi)
	movq	%rdx, (%rsi)
	addq	%rdx, %rax
	ret
	.cfi_endproc
.LFE0:
	.size	swap_add, .-swap_add
	.globl	caller
	.type	caller, @function
caller:
.LFB1:
	.cfi_startproc
	subq	$16, %rsp
	.cfi_def_cfa_offset 24
	movq	$534, 8(%rsp)
	movq	$1057, (%rsp)
	movq	%rsp, %rsi
	leaq	8(%rsp), %rdi
	call	swap_add
	movq	8(%rsp), %rdx
	subq	(%rsp), %rdx
	imulq	%rdx, %rax
	addq	$16, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1:
	.size	caller, .-caller
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-28)"
	.section	.note.GNU-stack,"",@progbits
