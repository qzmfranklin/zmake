## Copied from an online forum:
## http://stackoverflow.com/questions/3314919/compile-run-assembler-in-linux 

.data
.globl str_helloworld
str_helloworld:
.string "Hello world!"

.text
.globl main
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	$str_helloworld,%rdi
	call	puts
	xor	%eax, %eax
	;leave
	;movq	%rbp, %rsp
	;popq	%rbp
	ret
