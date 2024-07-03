/*
******************************************************************************
LABORATÓRIO P3 - SISTEMAS OPERACIONAIS
DEPARTAMENTO ACADÊMICO DE ENGENHARIA ELETRÔNICA
UNIVERSIDADE TECNOLÓGICA FEDERAL DO PARANÁ

THIAGO MELCHER ARMÊNIO  - 2358565
VICTOR AUGUSTO DEL MONEGO - 2378345

ppos_core.c - CÓDIGO QUE DEFINE AS FUNÇÕES DE ESCALONAMENTO

*****************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "drivers/pinout.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "fila.h"                  // Filas
#include "microcontext.h"          // Para troca de contexto
#include "ppos.h"                  // Declaracao de funcoes e defines do PPOS
#include "ppos_d.h"                // Estruturas de dados do Ping Pong

#define STACKSIZE 512

task_t *taskMain, *taskAtual;      // Task principal e atual
typedef enum idd {main, dispatcher} idd;

int taskID;                             // Contagem de ID de tasks
task_t taskdisp;                 	    // Despachante
task_t *taskQueue;                      // Fila de tarefas
task_t *taskNext;                       // Próxima tarefa a ser executada


// Escalonamento de tasks

task_t *scheduler ()                    // Função chamada para escalonar as threads
{
    task_t *taskSelection, *taskAux;
    taskSelection = taskQueue;          
    taskAux = taskQueue;                


    #ifdef DEBUG                        
		UARTprintf("scheduler: proxima tarefa: %d - %s\n",
		taskSelection->id, taskSelection->name);
    #endif
    return taskSelection;
}

void Bodydispatcher (void * arg)
{
    #if defined (DEBUG)|| defined(DEBUGlite)
        UARTprintf("dispatcher: iniciado\n");
    #endif

    while (taskQueue){
        taskNext = scheduler();
        if (taskNext->status == PRONTA) {
            queue_remove ((queue_t **) &taskQueue, (queue_t*) taskNext);
            task_switch(taskNext);
        }
    }
    task_exit(1) ;
}


// Inicializa o sistema operacional; deve ser chamada no inicio do main()
void ppos_init ()
{
	setvbuf (stdout, 0, _IONBF, 0);                     
	taskID = 0;
	
	taskMain = (task_t*)malloc(sizeof(task_t)); 		
	char *stack;      							                            
    stack = malloc (STACKSIZE);
	
	if(stack)
	{
		taskMain->context.uc_stack.ss_sp = stack;       
		taskMain->context.uc_stack.ss_size = STACKSIZE;
		taskMain->context.uc_stack.ss_flags = 0;
		taskMain->context.uc_link = 0;
		taskMain->name = "main";
	}
	else
	{
		UARTprintf("Erro na criacao da pilha.\n");
        exit(1);
	}
    taskMain->next = NULL;
    taskMain->prev = NULL;
    taskMain->id = taskID;

    taskAtual = taskMain;
    get_context_asm(&(taskMain->context));
    makecontextMain (&(taskMain->context), 0, 1, "main");
	


    #if defined (DEBUG)|| defined(DEBUGlite)                                                               
        UARTprintf("ppos_init: PPOS inicializado\n");
    #endif
    task_create (&taskdisp,Bodydispatcher, "dispatcher");     // Criação do despachante

}
// Cria uma nova tarefa. Retorna um ID> 0 ou erro.
int task_create (task_t *task,					        // Descritor da nova tarefa
                 void (*start_func)(void *),	        // Funcao corpo da tarefa
                 void *arg)						        // Argumentos para a tarefa
{
	char *stack;                                        // Criacao da pilha para contexto
    stack = malloc (STACKSIZE);
    if (stack)
    {
        task->context.uc_stack.ss_sp = stack;
        task->context.uc_stack.ss_size = STACKSIZE;
        task->context.uc_stack.ss_flags = 0;
        task->context.uc_link = 0;
        task->context.initialized = 0;
		task->status = PRONTA;
        task->name = arg;
    }
    else
    {
        UARTprintf("Erro na criacao da pilha: ");
        exit(1);
    }
    
    taskID++;
    task->next = NULL;
    task->prev = NULL;
    task->id = taskID;

    get_context_asm(&(task->context));                  
    makecontext(&(task->context), (int)(*start_func), 1, (char*)arg);

                                      
    if (task != &taskdisp)     // Adiciona a tarefa criada ao final da fila
                queue_append ((queue_t **) &taskQueue, (queue_t*) task);


    #if defined(DEBUG) || defined(DEBUGlite)    // Mensagem de debug
        UARTprintf("task_create: tarefa %d (%s) - criada com sucesso\n", task->id, task->name);
    #endif

    return task->id;
}


// Alterna a execução para a tarefa indicada
int task_switch (task_t *task)                              
{
    task_t *taskAux = taskAtual;                              
    taskAtual = task;                                     

    #ifdef DEBUG                                            
        UARTprintf("task_switch: mudanca de contexto em andamento: %d->%d\n", taskAux->id, task->id);
    #endif


    swap_context_asm(&(taskAux->context),&(task->context));     // Mudança do contexto em execucao

    return 0;
}

// Retorna o identificador da tarefa corrente (main deve ser 0)
int task_id ()
{
    #ifdef DEBUG                                        
        UARTprintf("task_id: tarefa atual: %d \n", taskAtual->id);
    #endif

    return taskAtual->id;                             // Retorna o id da tarefa em execucao
}


// Encerramento de tarefa
void task_exit (int exitCode)
{
    #if defined (DEBUG)|| defined(DEBUGlite)                                                                    
        UARTprintf("task_exit: encerramento da tarefa %d (%s) \n", taskAtual->id, taskAtual->name);
    #endif


    if (taskAtual->id == dispatcher)                                                                         
        task_switch(taskMain);
    else {                                                                                                      
        if (taskQueue){
            task_t *taskFinalizada = (task_t *) queue_remove((queue_t **) &taskQueue, (queue_t *) taskAtual); 
            free (taskFinalizada);                                                                              
        }
		
        task_switch(&taskdisp);
    }

}

// Aciona o despachante
void task_yield ()
{
    #ifdef DEBUG
        UARTprintf("task_yeld: chamada pela tarefa %d (%s)\n", taskAtual->id, taskAtual->name);
    #endif

    if (taskAtual->id != main && taskAtual->id != dispatcher){
        queue_append((queue_t **) &taskQueue, (queue_t *) taskAtual);
    }

    task_switch (&taskdisp);
}
