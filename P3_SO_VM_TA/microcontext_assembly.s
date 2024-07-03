;LABORATÓRIO P3 - SISTEMAS OPERACIONAIS
;DEPARTAMENTO ACADÊMICO DE ENGENHARIA ELETRÔNICA
;UNIVERSIDADE TECNOLÓGICA FEDERAL DO PARANÁ

;THIAGO MELCHER ARMÊNIO  - 2358565
;VICTOR AUGUSTO DEL MONEGO - 2378345

;microcontext_assembly.c - CÓDIGO QUE DEFINE FUNÇÕES UTILIZADAS PARA A TROCA
;DE CONTEXTO, UTILIZADAS EM "microcontext.c"

.text

P_ucontext_t           .set  0
stack_t_Pss_sp         .set  1 * 4
stack_t_ss_size        .set  2 * 4
stack_t_ss_flags       .set  3 * 4
mcontext_t_regPC       .set  4 * 4  ;  Program Counter
mcontext_t_regSP       .set  5 * 4  ;  Stack Pointer
mcontext_t_regCPSR     .set  6 * 4  ;  Status Register
mcontext_t_regR0       .set  7 * 4
mcontext_t_regR1       .set  8 * 4
mcontext_t_regR2       .set  9 * 4
mcontext_t_regR3       .set 10 * 4
mcontext_t_regR4       .set 11 * 4
mcontext_t_regR5       .set 12 * 4
mcontext_t_regR6       .set 13 * 4
mcontext_t_regR7       .set 14 * 4
mcontext_t_regR8       .set 15 * 4
mcontext_t_regR9       .set 16 * 4
mcontext_t_regR10      .set 17 * 4
mcontext_t_regR11      .set 18 * 4
mcontext_t_regR12      .set 19 * 4
mcontext_t_regR13      .set 20 * 4
mcontext_t_regR14      .set 21 * 4
mcontext_t_regR15      .set 22 * 4
ucontext_t_func        .set 23 * 4
ucontext_t_initialized .set 24 * 4


   .global   get_context_asm
get_context_asm:

;	push     {lr}
	str.w       r0,  [r0,#mcontext_t_regR0]
	str.w       r1,  [r0,#mcontext_t_regR1]
	str.w       r2,  [r0,#mcontext_t_regR2]
	str.w       r3,  [r0,#mcontext_t_regR3]
	str.w       r4,  [r0,#mcontext_t_regR4]
	str.w       r5,  [r0,#mcontext_t_regR5]
	str.w       r6,  [r0,#mcontext_t_regR6]
	str.w       r7,  [r0,#mcontext_t_regR7]
	str.w       r8,  [r0,#mcontext_t_regR8]
	str.w       r9,  [r0,#mcontext_t_regR9]
	str.w       r10, [r0,#mcontext_t_regR10]
	str.w       r11, [r0,#mcontext_t_regR11]
	str.w       r12, [r0,#mcontext_t_regR12]
	str.w       r13, [r0,#mcontext_t_regR13]
	str.w       r14, [r0,#mcontext_t_regR14]
	push	  {r3}
;	mov	      r3, #regR15
;   mov       r3, r15
    mov         r3, pc ; r15
;   add       r3, #6
	str.w       r3,  [r0,#mcontext_t_regR15]
	pop       {r3}
;	pop     {pc}
    bx        lr ; return


   .global set_context_asm
set_context_asm:


;//    push    {lr}
	LDR	 r1 ,  [r0 ,#mcontext_t_regR1]
    LDR  r2 ,  [r0 ,#mcontext_t_regR2]
    LDR  r3 ,  [r0 ,#mcontext_t_regR3]
    LDR  r4 ,  [r0 ,#mcontext_t_regR4]
    LDR  r5 ,  [r0 ,#mcontext_t_regR5]
    LDR  r6 ,  [r0 ,#mcontext_t_regR6]
    LDR  r7 ,  [r0 ,#mcontext_t_regR7]
    LDR  r8 ,  [r0 ,#mcontext_t_regR8]
    LDR  r9 ,  [r0 ,#mcontext_t_regR9]
    LDR  r10,  [r0 ,#mcontext_t_regR10]
    LDR  r11,  [r0 ,#mcontext_t_regR11]
    LDR  r12,  [r0 ,#mcontext_t_regR12]
;//    LDR  r13,  [r0 ,#mcontext_t_regR13]
    LDR  r14,  [r0 ,#mcontext_t_regR14]

	LDR	 r1 ,  [r0 ,#ucontext_t_initialized]
	CMP  r1 , #0
	BNE  inicializado

    ; ainda não inicializado
    mov  r1,   #1
	STR  r1 ,  [r0 ,#ucontext_t_initialized]
	LDR	 r1 ,  [r0 ,#mcontext_t_regR1]
	LDR  r13,  [r0 ,#stack_t_Pss_sp]
    LDR  r15,  [r0 ,#ucontext_t_func]
;//	pop     {pc}
    bx     lr

inicializado:
	LDR	 r1 ,  [r0 ,#mcontext_t_regR1]
    LDR  r13,  [r0 ,#mcontext_t_regR13]
;    LDR  r15,  [r0 ,#mcontext_t_regR15]
    LDR  r15,  [r0 ,#mcontext_t_regR14]
;	pop     {pc}
    bx     lr
;	.end
;////////////////////////  SWAP BEG /////////////////////
;;;;;;;;;;;;;;;;;;;;;;;;;  SWAP BEG ;;;;;;;;;;;;;;;;;;;;;.global swap_context_asm
    .global    swap_context_asm
swap_context_asm:

	str.w       r0,  [r0,#mcontext_t_regR0]
	str.w       r1,  [r0,#mcontext_t_regR1]
	str.w       r2,  [r0,#mcontext_t_regR2]
	str.w       r3,  [r0,#mcontext_t_regR3]
	str.w       r4,  [r0,#mcontext_t_regR4]
	str.w       r5,  [r0,#mcontext_t_regR5]
	str.w       r6,  [r0,#mcontext_t_regR6]
	str.w       r7,  [r0,#mcontext_t_regR7]
	str.w       r8,  [r0,#mcontext_t_regR8]
	str.w       r9,  [r0,#mcontext_t_regR9]
	str.w       r10, [r0,#mcontext_t_regR10]
	str.w       r11, [r0,#mcontext_t_regR11]
	str.w       r12, [r0,#mcontext_t_regR12]
	str.w       r13, [r0,#mcontext_t_regR13]
	str.w       r14, [r0,#mcontext_t_regR14]
	push	  {r3}
;	mov	      r3, #regR15
;   mov       r3, r15
    ;mov         r3, pc ; r15
    mov         r3, r15
 ;   add       r3, #6
	str.w       r3,  [r0,#mcontext_t_regR15]
	pop       {r3}
;	pop     {pc}
;    bx        lr //; return
;///////////////////////  end get   beg set

;	//LDR	 r1 ,  [r1 ,#mcontext_t_regR1]
    LDR  r2 ,  [r1 ,#mcontext_t_regR2]
    LDR  r3 ,  [r1 ,#mcontext_t_regR3]
    LDR  r4 ,  [r1 ,#mcontext_t_regR4]
    LDR  r5 ,  [r1 ,#mcontext_t_regR5]
    LDR  r6 ,  [r1 ,#mcontext_t_regR6]
    LDR  r7 ,  [r1 ,#mcontext_t_regR7]
    LDR  r8 ,  [r1 ,#mcontext_t_regR8]
    LDR  r9 ,  [r1 ,#mcontext_t_regR9]
    LDR  r10,  [r1 ,#mcontext_t_regR10]
    LDR  r11,  [r1 ,#mcontext_t_regR11]
    LDR  r12,  [r1 ,#mcontext_t_regR12]
;    LDR  r13,  [r0 ,#mcontext_t_regR13]
    LDR  r14,  [r1 ,#mcontext_t_regR14]

	LDR	 r0 ,  [r1 ,#ucontext_t_initialized]
	CMP  r0 , #0
	BNE  inicializado1

    ; ainda não inicializado
    mov  r0,   #1
	STR  r0 ,  [r1 ,#ucontext_t_initialized]
	;LDR	 r1 ,  [r1 ,#mcontext_t_regR1]
	LDR  r13,  [r1 ,#stack_t_Pss_sp]
    LDR  r0 ,  [r1 ,#mcontext_t_regR0] ;21_09_2017//
    LDR  r15,  [r1 ,#ucontext_t_func]
    bx     lr

inicializado1:
	;LDR	 r1 ,  [r1 ,#mcontext_t_regR1]
    LDR  r13,  [r1 ,#mcontext_t_regR13]
;    LDR  r15,  [r1 ,#mcontext_t_regR15]
    LDR  r0 ,  [r1 ,#mcontext_t_regR0] ;21_09_2017//
    LDR  r15,  [r1 ,#mcontext_t_regR14]
;	pop     {pc}
    bx     lr

;;;;;;;;;;;;;;;;;  SWAP END ;;;;;;;;;;;;;;;;;;;


