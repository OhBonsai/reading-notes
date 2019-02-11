	.file	"sum.c"
	.text
	.globl	sum
	.type	sum, @function
sum:
.LFB0:
	.cfi_startproc
	movl	$0, %eax
	movl	$0, %edx
	jmp	.L2
.L3:
	movslq	%edx, %rcx
	addl	(%rdi,%rcx,4), %eax
	addl	$1, %edx
.L2:
	cmpl	%esi, %edx
	jl	.L3
	rep ret
	.cfi_endproc
.LFE0:
	.size	sum, .-sum
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-28)"
	.section	.note.GNU-stack,"",@progbits
