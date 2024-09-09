// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Vers�o 1.2 -- Outubro de 2016

// Interface do n�cleo para as aplica��es

#ifndef __PPOS__
#define __PPOS__

#include "ppos_d.h"		// Estruturas de dados necess�rias

// Macros importantes ==========================================================
// Otimiza��es podem atrapalhar c�digo que manipula contexto
#ifdef __OPTIMIZE__
#error "Please do not use optimizations (-O1, -O2, ...)"
#endif

// Fun��es gerais ==============================================================
// Inicializa o sistema operacional; deve ser chamada no inicio do main()
void ppos_init () ;

// Gerenciamento de tarefas =========================================================
// Cria uma nova tarefa. Retorna um ID> 0 ou erro.
int task_create (task_t *task,			// descritor da nova tarefa
                 void (*start_func)(void *),	// funcao corpo da tarefa
                 void *arg) ;			// argumentos para a tarefa
// Termina a tarefa corrente, indicando um valor de status encerramento
void task_exit (int exitCode) ;
// Alterna a execução para a tarefa indicada
int task_switch (task_t *task) ;
// Retorna o identificador da tarefa corrente (main deve ser 0)
int task_id () ;

// Opera��es de escalonamento ==================================================
// Libera o processador para a pr�xima tarefa, retornando à fila de tarefas
// prontas ("ready queue")
void task_yield () ;
// define a prioridade est�tica de uma tarefa (ou a tarefa atual)
void task_setprio (task_t *task, int prio) ;
// retorna a prioridade est�tica de uma tarefa (ou a tarefa atual)
int task_getprio (task_t *task) ;

// Opera��es de sincroniza��o ==================================================
// Tarefa corrente aguarda o encerramento de outra task
int task_join (task_t *task) ;

// Opera��es de gest�o do tempo ================================================
// Suspende a tarefa corrente por t milissegundos
void task_sleep (int t) ;
// Retorna o rel�gio atual (em milisegundos)
unsigned int systime () ;

// Opera��es de IPC ============================================================

// Sem�foros

// Cria um sem�foro com valor inicial "value"
int sem_create (semaphore_t *s, int value) ;
// Requisita o sem�foro
int sem_down (semaphore_t *s) ;
// Libera o sem�foro
int sem_up (semaphore_t *s) ;
// Destroi o sem�foro, liberando as tarefas bloqueadas
int sem_destroy (semaphore_t *s) ;

// Mutexes

// Inicializa um mutex (sempre inicialmente livre)
int mutex_create (mutex_t *m) ;
// Solicita um mutex
int mutex_lock (mutex_t *m) ;
// Libera um mutex
int mutex_unlock (mutex_t *m) ;
// Destr�i um mutex
int mutex_destroy (mutex_t *m) ;

// Barreiras

// Inicializa uma barreira
int barrier_create (barrier_t *b, int N) ;
// Chega a uma barreira
int barrier_join (barrier_t *b) ;
// Destr�i uma barreira
int barrier_destroy (barrier_t *b) ;

// Filas de mensagens

// Cria uma fila para at� max mensagens de size bytes cada
int mqueue_create (mqueue_t *queue, int max, int size) ;
// Envia uma mensagem para a fila
int mqueue_send (mqueue_t *queue, void *msg) ;
// Recebe uma mensagem da fila
int mqueue_recv (mqueue_t *queue, void *msg) ;
// Destr�i a fila, liberando as tarefas bloqueadas
int mqueue_destroy (mqueue_t *queue) ;
// Informa o n�mero de mensagens atualmente na fila
int mqueue_msgs (mqueue_t *queue) ;

//==============================================================================
// Redefinir fun��es POSIX "proibidas" como "FORBIDDEN" (gera erro ao compilar)

// POSIX threads
#define pthread_create			FORBIDDEN
#define pthread_join			FORBIDDEN
#define pthread_exit			FORBIDDEN

// POSIX semaphores
#define sem_init			FORBIDDEN
#define sem_post			FORBIDDEN
#define sem_wait			FORBIDDEN
#define sem_trywait			FORBIDDEN

// POSIX message queues
#define mq_open				FORBIDDEN
#define mq_send				FORBIDDEN
#define mq_receive			FORBIDDEN
#define mq_close			FORBIDDEN

// POSIX condvars
#define pthread_cond_init		FORBIDDEN
#define pthread_cond_wait		FORBIDDEN
#define pthread_cond_signal		FORBIDDEN
#define pthread_cond_timedwait		FORBIDDEN

// POSIX barriers
#define pthread_barrier_init		FORBIDDEN
#define pthread_barrier_wait		FORBIDDEN
#define pthread_barrier_destroy		FORBIDDEN

// POSIX mutexes
#define pthread_mutex_init		FORBIDDEN
#define pthread_mutex_lock		FORBIDDEN
#define pthread_mutex_unlock		FORBIDDEN
#define pthread_mutex_timedlock		FORBIDDEN
#define pthread_mutex_trylock		FORBIDDEN
#define pthread_mutex_destroy		FORBIDDEN

// POSIX RW-locks
#define pthread_rwlock_init		FORBIDDEN
#define pthread_rwlock_rdlock		FORBIDDEN
#define pthread_rwlock_wrlock		FORBIDDEN
#define pthread_rwlock_unlock		FORBIDDEN
#define pthread_rwlock_tryrdlock	FORBIDDEN
#define pthread_rwlock_tryrwlock	FORBIDDEN
#define pthread_rwlock_timedrdlock	FORBIDDEN
#define pthread_rwlock_timedrwlock	FORBIDDEN
#define pthread_rwlock_destroy		FORBIDDEN

// POSIX spinlocks
#define pthread_spin_init		FORBIDDEN
#define pthread_spin_lock		FORBIDDEN
#define pthread_spin_unlock		FORBIDDEN
#define pthread_spin_trylock		FORBIDDEN
#define pthread_spin_destroy		FORBIDDEN

#endif
