/*
Criação de threads POSIX em UNIX.

Compilar com gcc -Wall thread-create.c -o thread-create -lpthread

Carlos Maziero, DINF/UFPR 2020
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 16

void *threadBody (void *id)													// função que denota o comportamento das threads criadas
{
  long tid = (long) id ; 													// ID da thread

  printf ("t%02ld: Olá!\n", tid) ;											// imprime uma mensagem de saudação juntamente com o ID da thread
  sleep (3) ;   															// a função dorme por 3 segundos
  printf ("t%02ld: Tchau!\n", tid) ;  										// a thread se despede
  pthread_exit (NULL) ;														// encerramento da thread
}

int main (int argc, char *argv[])
{
  pthread_t thread [NUM_THREADS] ;											// um vetor de threads é criado baseado no tamanho pré estabelecido
  long i, status ;															// declara uma variavel para armazenar a iteração e outra para o valor de retorno da função de criação da tread
   
  for (i=0; i<NUM_THREADS; i++)												// entramos em um laço de iteração 
  {
    printf ("Main: criando thread %02ld\n", i) ;							// imprime uma mensagem para sinalizar a criação da proxima thread
    status = pthread_create (&thread[i], NULL, threadBody, (void *) i) ; 	// status recebe o valor de retorno da criação da thread
    if (status)																// se houve erro na criação dessa thread
    {
      perror ("pthread_create") ;											// imprime uma mensagem de erro
      exit (1) ;
    }
  }
  printf ("Main: fim\n") ;													// ao fim da CRIAÇÃO das threads, o programa imprime "fim"
  pthread_exit (NULL) ;														// encerra a thread do programa principal, mantendo as outras threads funcionando independentemente para terminarem sua execução
																			// em resumo, a thread 0 é o programa principal nesse caso.
