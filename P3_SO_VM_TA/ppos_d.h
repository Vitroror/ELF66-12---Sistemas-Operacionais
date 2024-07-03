// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Vers√£o 1.1 -- Julho de 2016

// Estruturas de dados internas do sistema operacional

#ifndef __PPOS_D__
#define __PPOS_D__

#include "fila.h"		                                            // Biblioteca de filas gen√©ricas
#include "microcontext.h"		                                    // Biblioteca POSIX de trocas de contexto
typedef enum task_state {PRONTA, SUSPENSA, TERMINADA} task_state;

// Estrutura que define um Task Control Block (TCB)
typedef struct task_t
{
   struct task_t *prev, *next ;     // Ponteiros para usar em filas
   int id ;				            // Identificador da tarefa
   ucontext_t context ;			    // Contexto armazenado da tarefa
  task_state status;                // Status (PRONTA, SUSPENSA, TERMINADA)
   char *name;                      // Nome
} task_t ;

// Estrutura que define um sem·foro
typedef struct
{
// Preencher quando neces·rio
} semaphore_t ;

// Estrutura que define um mutex
typedef struct
{
// Preencher quando necess·rio
} mutex_t ;

// Estrutura que define uma barreira
typedef struct
{
// Preencher quando necess·rio
} barrier_t ;

// Estrutura que define uma fila de mensagens
typedef struct
{
// Preencher quando necess·rio
} mqueue_t ;

#endif
