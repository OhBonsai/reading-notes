	.file	"main.c"
	.text
	.globl	_start
	.type	_start, @function
_start:
.LFB9:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$2, %esi
	movl	$array, %edi
	call	sum
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE9:
	.size	_start, .-_start
	.globl	array
	.data
	.align 4
	.type	array, @object
	.size	array, 8
array:
	.long	1
	.long	2
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-28)"
	.section	.note.GNU-stack,"",@progbits
