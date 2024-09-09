/*
Criação de threads POSIX em UNIX, com impressão de variáveis.

Compilar com gcc -Wall thread-print.c -o thread-print -lpthread

Carlos Maziero, DINF/UFPR 2020
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 16

int x = 0 ;  																// variável global

void *threadBody (void *id)													// função que denota o comportamento das threads criadas
{
  long tid = (long) id ; // ID da thread
   
  x++ ;																		// incrementa a variável global em 1
  printf ("t%02ld: Olá!   (x=%02d)\n", tid, x) ;							// a thread se identifica e reporta o valor de x
  sleep (3) ;   															// a função dorme por 3 segundos
  x++ ;																		// incrementa a variável global em 1
  printf ("t%02ld: Tchau! (x=%02d)\n", tid, x) ; 							// a thread se despede e reporta o valor de x 
  pthread_exit (NULL) ;														// a thread se extingue
}

int main (int argc, char *argv[])
{
  pthread_t thread [NUM_THREADS] ;											// criação do vetor de threads
  long i, status ;															// cria a variável de iteração e de armazenamento do status da thread
   
  for (i=0; i<NUM_THREADS; i++)
  {
    printf ("Main: criando thread %02ld\n", i) ;							// sinaliza que uma thread está sendo criada
    status = pthread_create (&thread[i], NULL, threadBody, (void *) i) ;	// sinaliza o status da thread  
    if (status)																// se houve erro de criação das threads
    {
      perror ("pthread_create") ; 											// imprime a mensagem de erro
      exit (1) ;
    }
  }
  printf ("Main: fim\n") ; 													// ao fim da CRIAÇÃO das threads, o programa imprime "fim"
  pthread_exit (NULL) ; 													// encerra a thread do programa principal, mantendo as outras threads funcionando independentemente para terminarem sua execução
}