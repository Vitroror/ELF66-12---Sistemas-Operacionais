// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.0 -- Março de 2015

// Definição e operações em uma fila genérica.

#ifndef __FILA__
#define __FILA__

#ifndef NULL
#define NULL ((void *) 0)
#endif

//------------------------------------------------------------------------------
// Estrutura de uma fila genérica, sem conteúdo definido.

typedef struct queue_t
{
   struct queue_t *prev ;  // Aponta para o elemento anterior na fila
   struct queue_t *next ;  // Aponta para o elemento seguinte na fila
} queue_t ;

//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condições a verificar, gerando mensagens de erro:
// - A fila deve existir
// - O elemento deve existir
// - O elemento não deve estar em outra fila

void queue_append (queue_t **queue, queue_t *elem) ;

//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condições a verificar, gerando mensagens de erro:
// - A fila deve existir
// - A fila nao deve estar vazia
// - O elemento deve existir
// - O elemento deve pertencer a fila indicada
// Retorno: apontador para o elemento removido, ou NULL se erro

queue_t *queue_remove (queue_t **queue, queue_t *elem) ;

//------------------------------------------------------------------------------
// Conta o número de elementos na fila
// Retorno: número de elementos na fila

int queue_size (queue_t *queue) ;

//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca.
//
// Essa função deve ter o seguinte protótipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir

void queue_print (char *name, queue_t *queue, void print_elem (void*) ) ;

#endif
