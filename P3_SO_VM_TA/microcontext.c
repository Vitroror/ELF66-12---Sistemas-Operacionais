/*
******************************************************************************
 LABORATÓRIO P3 - SISTEMAS OPERACIONAIS
 DEPARTAMENTO ACADÊMICO DE ENGENHARIA ELETRÔNICA
 UNIVERSIDADE TECNOLÓGICA FEDERAL DO PARANÁ

 THIAGO MELCHER ARMÊNIO  - 2358565
 VICTOR AUGUSTO DEL MONEGO - 2378345

 microcontext.c - CÓDIGO QUE DEFINE FUNÇÕES DE MICROCONTEXTO, INCLUINDO A CHAMADA DE TROCA DE CONTEXTO

*****************************************************************************
*/

#include "microcontext.h"
extern int flag;
extern int memPC;
extern set_context_asm(ucontext_t*);
extern get_context_asm (ucontext_t*);
//int r;
//int r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15;

//int t1 = 4;


void makecontext(ucontext_t *context, int start_routine, int n_parms, char* parm)
{
	context->func = start_routine;
	context->uc_mcontext.regR0 = parm;
}
void makecontextMain(ucontext_t *context, int start_routine, int n_parms, char* parm)
{
    context->func = start_routine;
    context->initialized = 1;
	context->uc_mcontext.regR0 = parm;

}

int swapcontext(ucontext_t *saida, ucontext_t *entrada)
{
    get_context_asm(saida);
    set_context_asm(entrada);

    return 0;
}
