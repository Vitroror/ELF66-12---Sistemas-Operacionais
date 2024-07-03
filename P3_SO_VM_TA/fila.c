/*
******************************************************************************
LABORATÓRIO P3 - SISTEMAS OPERACIONAIS
DEPARTAMENTO ACADÊMICO DE ENGENHARIA ELETRÔNICA
UNIVERSIDADE TECNOLÓGICA FEDERAL DO PARANÁ

THIAGO MELCHER ARMÊNIO  - 2358565
VICTOR AUGUSTO DEL MONEGO - 2378345

fila.c - CÓDIGO QUE DEFINE FUNÇÕES DE MANIPULAÇÃO DE FILA

*****************************************************************************
*/


#include "fila.h"

//Insere um elemento no final da fila
//Condições a verificar, gerando mensagem de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento não deve estar em outra fila

void queue_append (queue_t **queue, queue_t *elem)
{
    if(queue == NULL)
    {
        return;
    }

    if(elem == NULL)
    {
        return;
    }

    if(elem->next != NULL || elem->prev != NULL)
    {
        return;
    }

    if((*queue) == NULL)
    {
        *queue = elem;
        elem->next = elem;
        elem->prev = elem;
    }
    else{
        queue_t* aux = *queue;
        while(aux->next != (*queue))
        {
            aux = aux->next;
        }
        aux->next = elem;
        elem->prev = aux;
        elem->next = *queue;
        (*queue)->prev = elem;
    }
    return;

}
//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: apontador para o elemento removido, ou NULL se erro

queue_t *queue_remove (queue_t **queue, queue_t *elem)
{
    if(queue == NULL)
    {
        return NULL;
    }
    if((*queue) == NULL)
    {
        return NULL;
    }
    if(elem == NULL)
    {
        return NULL;
    }
    //if((*queue)->next != elem->next && (*queue)->prev != elem->prev)
    //{
    //    UARTprintf("### Erro: Elemento nao pertence a lista indicada\n");
    //    return NULL;
    //}

    queue_t *aux1, *aux2, *aux3;
    aux1 = *queue;

    if((aux1 == aux1->next && aux1 == aux1->prev)&& (aux1 != elem))
    {
        return NULL;
    }

    if((aux1 == aux1->next && aux1 == aux1->prev)&& (aux1 == elem)){
        *queue = NULL;
        elem->next = NULL;
        elem->prev = NULL;
        return elem;
    }

    if(elem == *queue)
    {
        aux1 = *queue;
        aux2 = aux1->prev;
        aux3 = aux1->next;

        *queue = aux3;
        aux2->next = aux3;
        aux3->prev = aux2;
        elem->next = NULL;
        elem->prev = NULL;

        return elem;
    }

    aux1 = *queue;
    aux1 = aux1->next;

    while(aux1 != *queue)
    {
        if(aux1 == elem)
        {
            aux2 = aux1->prev;
            aux3 = aux1->next;
            aux2->next = aux3;
            aux3->prev = aux2;
            elem->next = NULL;
            elem->prev = NULL;
            return elem;
        }
        aux1 = aux1->next;
    }
    //UARTprintf("### Erro: Elemento nao esta na lista\n");
    return NULL;

}

//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila

int queue_size (queue_t *queue)
{
    int tam = 0;
    queue_t* aux = queue;

    if(queue == NULL)
    {
        return tam;
    }
    tam++;
    aux = aux->next;

    while(aux != queue)
    {
        tam++;
        aux = aux->next;
    }
    return tam;
}

//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca.
//
// Essa função deve ter o seguinte protótipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir

void queue_print (char *name, queue_t *queue, void print_elem (void*) )
{
    queue_t* aux = queue;
    UARTprintf("%s: [",name);

    if(aux != NULL)
    {
       while(aux->next != queue && aux->next != NULL)
       {
           print_elem(aux);
           aux = aux->next;
           UARTprintf(" ");
       }
       print_elem(aux);

    }
    UARTprintf("] \n");

}
