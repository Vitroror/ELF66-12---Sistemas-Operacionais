/*
******************************************************************************
LABORATÓRIO P5 - SISTEMAS OPERACIONAIS
DEPARTAMENTO ACADÊMICO DE ENGENHARIA ELETRÔNICA
UNIVERSIDADE TECNOLÓGICA FEDERAL DO PARANÁ

THIAGO MELCHER ARMÊNIO  - 2358565
VICTOR AUGUSTO DEL MONEGO - 2378345
*****************************************************************************
*/

#include "pingpong.h"

task_t *task_current, task_main, task_dispacther;
task_t *queue_ready = NULL;    
unsigned long int count_id;  //Como a primera task (main) é 0 a póxima tarefa terá o id 1

struct sigaction action ; // estrutura que define um tratador de sinal (deve ser global ou static)
struct itimerval timer; // estrutura de inicialização to timer
int quantum; //


void dispatcher_body(void* arg); 
task_t* scheduler();
void signal_handler(int singnum); // Função que trata os sinais recebidos

// Inicializa o sistema operacional; deve ser chamada no inicio do main()
void pingpong_init () {

    //desativação do buffer da saida padrao (stdout), usado pela função printf
    setvbuf (stdout, 0, _IONBF, 0) ;

    // Tarefa main possui id 0
    task_main.t_id = 0;
    count_id = 1;
    task_main.state = READY;

    //Inicialização do encademanto das tarefas
    task_main.prev = NULL;
    task_main.next = NULL;

    // Referência a si mesmo
    task_main.main = &task_main;
    task_main.t_type = USER_TASK;

    // criação da tarefa dispatcher
    task_create(&task_dispacther,(void*)(dispatcher_body), "dispatcher"); 
    task_dispacther.t_type = SYSTEM_TASK;
 
    // A tarefa em execução é a main
    task_current = &task_main;

    //Inicialização de timer e sinais e demais tratamentos para os mesmos
    #ifdef DEBUG
    printf("Setando timers e sinais\n")
    #endif
    action.sa_handler = signal_handler ;
    sigemptyset (&action.sa_mask) ;
    action.sa_flags = 0 ;
    if (sigaction (SIGALRM, &action, 0) < 0)
    {
        printf ("Erro em pingpong_init(): Erro em sigaction\n ") ;
        exit (1) ;
    }

    // ajusta valores do temporizador
    timer.it_value.tv_usec = 100 ;      // primeiro disparo, em micro-segundos
    timer.it_value.tv_sec  = 0 ;      // primeiro disparo, em segundos
    timer.it_interval.tv_usec = 1000 ;   // disparos subsequentes, em micro-segundos
    timer.it_interval.tv_sec  = 0 ;   // disparos subsequentes, em segundos

    // arma o temporizador ITIMER_REAL (vide man setitimer)
    if (setitimer (ITIMER_REAL, &timer, 0) < 0)
    {
        printf ("Erro em pingpong_init(): Erro em setitimer \n") ;
        exit (1) ;
    }



    #ifdef DEBUG
    printf("PingPongOS iniciado com êxito.\n");
    #endif
}

// Cria uma nova tarefa. Retorna um ID> 0 ou erro.
int task_create (task_t *task, void (*start_func)(void *), void *arg){

    char *stack = NULL ;
    task->next = NULL;
    task->prev = NULL;

    // Referência para a tarefa main
    task->main = &task_main;

    // Inicialização do contexto.
    getcontext(&(task->context));

    // Alocação da pilha
    stack = malloc(STACKSIZE);

    if(!stack){
        printf("Error on task_create: criação da stack falhou!\n");
        return -1;
    }

    // Pilha do contexto é atribuída
    task->context.uc_stack.ss_sp = stack;
    task->context.uc_stack.ss_size = STACKSIZE;
    task->context.uc_stack.ss_flags = 0;
    task->context.uc_link = NULL;

    // Cria o contexto com a função
    makecontext (&(task->context), (void*)(*start_func), 1, arg);

    // A nova tarefa recebe o contador, como id
    task->t_id = count_id;
    // O contador é incrementado para que futuras tarefas possam utiliza-lo
    count_id = count_id + 1;

    //A criaçao da tarefa terminou, seu estado passa a ser READY ('r')
    task->state = READY;

    // Seta a prioridade estática e dinâmica para 0
    task->priority_static = 0;
    task->priority_dynamic = 0;

    if( task->t_id > 1){
        task->t_type = USER_TASK;
        queue_append((queue_t**)&queue_ready,(queue_t*) (task));
        task->ptr_queue = (queue_t**)&queue_ready;
        #ifdef DEBUG
        printf("task_create: Task %d foi adicionada a fila de prontos\n", task->t_id);
        #endif
    }
  
    #ifdef DEBUG
	printf ("task_create: tarefa %d criada com sucesso\n", task->t_id);
    #endif
    
    return ((int)task->t_id);
}

// alterna a execução para a tarefa indicada
int task_switch (task_t *task){

    task_t *task_previous = NULL;

    if(task == NULL){
        printf("Error on task_switch: A tarefa é nula! \n");
        return -1;
    }
    // ponteiro auxiliar que indica a tarefa anterior.
    task_previous = task_current;
    //  task_previous.state
    // a tarefa corrente passa a ser a nova tarefa recebida.
    task_current = task;

    #ifdef DEBUG
    printf ("task_switch: trocando contexto %d -> %d\n", task_previous->t_id, task->t_id) ;
    #endif 
    // troca de contextos
    if (swapcontext (&(task_previous->context), &(task_current->context)) < 0) {
        printf ("Error on task_switch: Troca de Contexto falhou! \n");
        return -1;
    }
    return 0;
}

// Termina a tarefa corrente, indicando um valor de status encerramento
void task_exit (int exitCode) {
    #ifdef DEBUG
    printf ("task_exit: tarefa %d sendo encerrada\n", task_current->t_id) ;
    #endif  
    //Se a tarefa corrente que está pra sair não for o dispatcher, o dispacther assume
    if (task_current != &task_dispacther) {
        task_switch(&task_dispacther);
    }
    // Se o dispatcher for sair, o controle passa para a main
    else {
        task_switch(&task_main);
    }
}

// retorna o identificador da tarefa corrente (main é 0)
int task_id (){
    #ifdef DEBUG
    printf ("task_id: ID da tarefa %d\n", task_current->t_id) ;
    #endif 
    return (task_current->t_id);
}

// em função da implementação do dia 23/09, agora o algoritmo está mais para um PRIOp

task_t *scheduler(){
    #ifdef DEBUG
    printf("schdueler: Iniciando\n");
    #endif
    task_t *task_aux = NULL;

    // Se a fila estiver vazia
    if (queue_ready == NULL)
        return NULL;

    // Se somente um elemento está contido na fila de prontas
    // Este único elemento é escolhido;
    else if ( queue_ready == queue_ready->next )
	{
		task_aux = queue_ready;
	}

    //se a fila conter mais de um elemento
    else{
        // ponteiro para a tarefa que será escolhida
        task_t* priority_task = queue_ready;
        task_aux = queue_ready;
        // inteiro auxiliar quer armazzenara o valor da prioridade dinamica da tarefa a ser comparada
        int aux_prio_d;

        do{
            
            aux_prio_d = task_aux->priority_dynamic;
            // Critério: a tarefa prioritaria sempre será uma tarefa comprioridade menor ou igual a atual     
            if( priority_task->priority_dynamic >  aux_prio_d ){

                //Como a prioridade dinâica da tarefa seguinte é menor, ela é escolhida
                priority_task = task_aux;
            }
            //Proxima tarefa a ser comparada
            task_aux = task_aux->next;

        }while(task_aux != queue_ready);

        task_aux = queue_ready;
 
        do{
            //Envelhicento de tarefas, respeitando o range de prioridades
            if (task_aux->priority_dynamic > -20){
			    task_aux->priority_dynamic--;
            }
            task_aux = task_aux->next;

        }while(task_aux != queue_ready);

        task_aux = priority_task;
    }
    #ifdef DEBUG
    printf("scheduler: Task %d foi escolhida pelo scheduler\n", task_aux->t_id);
    #endif

     //como a tarefa foi escolhida sua pripridade dinâmica é resetada;
    task_aux->priority_dynamic =  task_aux->priority_static;

    return task_aux;
}

void dispatcher_body (void *arg) // dispatcher é uma tarefa
{
    
    int size_of_queue = (int)queue_size((queue_t*)queue_ready);
    // Enquanto houverem tarefas prontas para serem executadas o loop continua
    while ( size_of_queue > 0 )
    {
        task_t *task_next;
        task_next = NULL;
        // a funçao scheduler decide qual a procima tarefa a ser executada
        task_next = scheduler() ; // scheduler retorna a proxima tarefa

        if (task_next != NULL){
            // Remoção da proxima tarefa a ser executa da fila de prontos, por meio do casting para queue_t
            queue_remove((queue_t**)&queue_ready, (queue_t*)task_next);
            #ifdef DEBUG
             printf("dispatcher_body: Task %d foi removida da fila de prontos\n", task_next->t_id);
            #endif
            task_next->ptr_queue = NULL;
            //O quantum é resetado para o valor padrão do sistema:
            quantum = QUANTUM;

            // a tarafa next é lançada  
            task_switch (task_next) ; // transfere controle para a tarefa "next"
            // ... // ações após retornar da tarefa "next", se houverem
            
        }
        size_of_queue = (int)queue_size((queue_t*)queue_ready);
    }
    task_exit(0) ; // encerra a tarefa dispatcher
}

// libera o processador para a próxima tarefa, retornando à fila de tarefas
// prontas ("ready queue")

void task_yield () {
    
    if (task_current->t_id > 1) {

        queue_append((queue_t**)&queue_ready, (queue_t*)(task_current));
        task_current->ptr_queue = (queue_t**)(&queue_ready);
        task_current->state = READY;

        #ifdef DEBUG
        printf("task_yield: Task %d foi adicionada a fila de prontos\n", task_current->t_id);
        #endif
    }
    // Dispatcher assume o controle
    task_switch(&task_dispacther);
}

void signal_handler(int singnum){

    //Se a tarefa atual for uma tarefa de usário ela poderá ser “preemptada” caso necessário
    // então a cada chama do tratador seu quantum diminui
	if(task_current->t_type == USER_TASK){
        if(quantum < 1){
			#ifdef DEBUG
			printf("signal_handler: Tarefa chegou ao final do quantum: %d\n", task_corrente->tid);
			#endif
			task_yield();
		}
		else{
			quantum--;
		}
	}
    return;
}


// suspende uma tarefa, retirando-a de sua fila atual, adicionando-a à fila
// queue e mudando seu estado para "suspensa"; usa a tarefa atual se task==NULL
void task_suspend (task_t *task, task_t **queue){
    queue_t *aux = NULL;

    if(task == NULL){
        task = task_current;
    }
    // Remoção da tarefa indicada da fila de prontos

    //REMOVE DA QUEUE_READY?
    if(queue != NULL){
        aux = queue_remove((queue_t**)(&task->ptr_queue),(queue_t*)(task));
        // Se o retorno for Nulo a tarefa nao existe na fila de prontos, logo deve apontar erro
        if(aux == NULL){
            printf("Error on task_suspend: A tarefa nao pode ser removida da fila de prontos!");
        }
        else{
            // Mudança de estado para suspenso
            task->state = SUSPENDED;
            // Inclusão da tarefa na fila de suspensos
            queue_append((queue_t**)(&queue),(queue_t*)(task));
            task->ptr_queue = (queue_t**)(&queue);
        }
    }
}
// acorda uma tarefa, retirando-a de sua fila atual, adicionando-a à fila de
// tarefas prontas ("ready queue") e mudando seu estado para "pronta"
void task_resume (task_t *task){
    task_t* task_aux = NULL;
    task_aux = (task_t*)queue_remove((queue_t**)(&task->ptr_queue),(queue_t*)(&task));
    task_aux->state = READY;
    queue_append((queue_t**)(&queue_ready),(queue_t*)(task_aux));
    task->ptr_queue = (queue_t**)(&queue_ready);
}
// recolhe a prioridade estática da tarefa task
int task_getprio (task_t *task) {

    if (task != NULL)
        return task->priority_static;
    else
        return task_current->priority_static;
}
// seta a prioridade
void task_setprio (task_t *task, int prio){

    if (prio < -20 || prio > 20)
        printf("Error on task_setprio: Impossível setar prioridade fora do range\n");

    if (task != NULL){
        task->priority_dynamic =prio;
        task->priority_static = prio;
    }
    else{
        task_current->priority_dynamic = prio;
        task_current->priority_static = prio;
    }
    
}



 