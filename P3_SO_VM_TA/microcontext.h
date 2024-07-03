#ifndef __MICROCONTEXT__
#define __MICROCONTEXT__

typedef struct stack_t
{
	char *ss_sp;
	long ss_size;
	int ss_flags;
} stack_t;

typedef struct mcontext_t
{
	int regPC;         // Program Counter
	int regSP;         // Stack Pointer
	int regCPSR;       // Status Register
	int regR0;
	int regR1;
	int regR2;
	int regR3;
	int regR4;
	int regR5;
	int regR6;
	int regR7;
	int regR8;
	int regR9;
	int regR10;
	int regR11;
	int regR12;
	int regR13;
	int regR14;
	int regR15;
} mcontext_t;

typedef struct ucontext_t
{
	struct ucontext_t *uc_link;
	//sigset_t uc_sigmask;
	stack_t uc_stack;
	mcontext_t uc_mcontext;
	int func;
	int initialized;
} ucontext_t;

extern int getcontext(ucontext_t *);
extern int get_context_asm(ucontext_t *);

int setcontext( ucontext_t *);
int set_context_asm( ucontext_t *);

void makecontext(ucontext_t *context, int bodyAdress, int n_parms, char *parm);
void makecontextMain(ucontext_t *context, int start_routine, int n_parms, char* parm);

void setgearclock(int);

int swapcontext(ucontext_t *, ucontext_t *);
int swap_context_asm (ucontext_t *, ucontext_t *);

#endif
