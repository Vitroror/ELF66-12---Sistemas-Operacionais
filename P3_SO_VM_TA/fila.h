// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Vers�o 1.0 -- Mar�o de 2015

// Defini��o e opera��es em uma fila gen�rica.

#ifndef __FILA__
#define __FILA__

#ifndef NULL
#define NULL ((void *) 0)
#endif

//------------------------------------------------------------------------------
// Estrutura de uma fila gen�rica, sem conte�do definido.

typedef struct queue_t
{
   struct queue_t *prev ;  // Aponta para o elemento anterior na fila
   struct queue_t *next ;  // Aponta para o elemento seguinte na fila
} queue_t ;

//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condi��es a verificar, gerando mensagens de erro:
// - A fila deve existir
// - O elemento deve existir
// - O elemento n�o deve estar em outra fila

void queue_append (queue_t **queue, queue_t *elem) ;

//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condi��es a verificar, gerando mensagens de erro:
// - A fila deve existir
// - A fila nao deve estar vazia
// - O elemento deve existir
// - O elemento deve pertencer a fila indicada
// Retorno: apontador para o elemento removido, ou NULL se erro

queue_t *queue_remove (queue_t **queue, queue_t *elem) ;

//------------------------------------------------------------------------------
// Conta o n�mero de elementos na fila
// Retorno: n�mero de elementos na fila

int queue_size (queue_t *queue) ;

//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conte�do. A impress�o de cada
// elemento � feita por uma fun��o externa, definida pelo programa que
// usa a biblioteca.
//
// Essa fun��o deve ter o seguinte prot�tipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir

void queue_print (char *name, queue_t *queue, void print_elem (void*) ) ;

#endif
