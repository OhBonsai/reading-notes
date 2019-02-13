	.file	"call.c"
	.text
	.globl	call2
	.type	call2, @function
call2:
.LFB12:
	.cfi_startproc
	movl	%edi, %eax
	imull	%edi, %eax
	ret
	.cfi_endproc
.LFE12:
	.size	call2, .-call2
	.globl	call1
	.type	call1, @function
call1:
.LFB11:
	.cfi_startproc
	movl	$0, %eax
	call	call2
	addl	$1, %eax
	ret
	.cfi_endproc
.LFE11:
	.size	call1, .-call1
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"result is %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$3, %edi
	call	call1
	movl	%eax, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-28)"
	.section	.note.GNU-stack,"",@progbits
