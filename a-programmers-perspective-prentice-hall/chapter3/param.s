	.file	"param.c"
	.text
	.globl	proc
	.type	proc, @function
proc:
.LFB0:
	.cfi_startproc
	movq	16(%rsp), %rax
	addq	%rdi, (%rsi)
	addl	%edx, (%rcx)
	addw	%r8w, (%r9)
	movl	8(%rsp), %edx
	addb	%dl, (%rax)
	ret
	.cfi_endproc
.LFE0:
	.size	proc, .-proc
	.globl	call_proc
	.type	call_proc, @function
call_proc:
.LFB1:
	.cfi_startproc
	subq	$32, %rsp
	.cfi_def_cfa_offset 40
	movq	$1, 24(%rsp)
	movl	$2, 20(%rsp)
	movw	$3, 18(%rsp)
	movb	$97, 17(%rsp)
	leaq	17(%rsp), %rax
	movq	%rax, 8(%rsp)
	movl	$97, (%rsp)
	leaq	18(%rsp), %r9
	movl	$3, %r8d
	leaq	20(%rsp), %rcx
	movl	$2, %edx
	leaq	24(%rsp), %rsi
	movl	$1, %edi
	call	proc
	movslq	20(%rsp), %rdx
	addq	24(%rsp), %rdx
	movswl	18(%rsp), %eax
	movsbl	17(%rsp), %ecx
	subl	%ecx, %eax
	cltq
	imulq	%rdx, %rax
	addq	$32, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1:
	.size	call_proc, .-call_proc
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-28)"
	.section	.note.GNU-stack,"",@progbits
