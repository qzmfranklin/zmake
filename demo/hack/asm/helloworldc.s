# mark_description "Intel(R) C Intel(R) 64 Compiler XE for applications running on Intel(R) 64, Version 14.0.3.174 Build 2014042";
# mark_description "2";
# mark_description "-S -fsource-asm";
	.file "helloworldc.c"
	.text
..TXTST0:
# -- Begin  main
# mark_begin;
       .align    16,0x90
	.globl main
main:
# parameter 1: %edi
# parameter 2: %rsi
..B1.1:                         # Preds ..B1.0

### {

..___tag_value_main.1:                                          #3.1
        pushq     %rbp                                          #3.1
..___tag_value_main.3:                                          #
        movq      %rsp, %rbp                                    #3.1
..___tag_value_main.4:                                          #
        andq      $-128, %rsp                                   #3.1
        subq      $128, %rsp                                    #3.1
        movq      $0x000000000, %rsi                            #3.1
        movl      $3, %edi                                      #3.1
        call      __intel_new_feature_proc_init                 #3.1
                                # LOE rbx r12 r13 r14 r15
..B1.5:                         # Preds ..B1.1
        stmxcsr   (%rsp)                                        #3.1

### 	puts("hello, world!");

        movl      $.L_2__STRING.0, %edi                         #4.2
        orl       $32832, (%rsp)                                #3.1
        ldmxcsr   (%rsp)                                        #3.1
        call      puts                                          #4.2
                                # LOE rbx r12 r13 r14 r15
..B1.2:                         # Preds ..B1.5

### 	return 0;

        xorl      %eax, %eax                                    #5.9
        movq      %rbp, %rsp                                    #5.9
        popq      %rbp                                          #5.9
..___tag_value_main.6:                                          #
        ret                                                     #5.9
        .align    16,0x90
..___tag_value_main.8:                                          #
                                # LOE
# mark_end;
	.type	main,@function
	.size	main,.-main
	.data
# -- End  main
	.section .rodata.str1.4, "aMS",@progbits,1
	.align 4
	.align 4
.L_2__STRING.0:
	.long	1819043176
	.long	1998597231
	.long	1684828783
	.word	33
	.type	.L_2__STRING.0,@object
	.size	.L_2__STRING.0,14
	.data
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 8
	.4byte 0x00000014
	.8byte 0x7801000100000000
	.8byte 0x0000019008070c10
	.4byte 0x00000000
	.4byte 0x00000034
	.4byte 0x0000001c
	.8byte ..___tag_value_main.1
	.8byte ..___tag_value_main.8-..___tag_value_main.1
	.byte 0x04
	.4byte ..___tag_value_main.3-..___tag_value_main.1
	.2byte 0x100e
	.byte 0x04
	.4byte ..___tag_value_main.4-..___tag_value_main.3
	.4byte 0x8610060c
	.2byte 0x0402
	.4byte ..___tag_value_main.6-..___tag_value_main.4
	.8byte 0x00000000c608070c
	.2byte 0x0000
# End
