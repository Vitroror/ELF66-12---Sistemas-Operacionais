/*
Criação de threads POSIX em UNIX, com operação join().

Compilar com gcc -Wall thread-join.c -o thread-join -lpthread

Carlos Maziero, DINF/UFPR 2020
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 16

void *threadBody (void *id)															// função que denota o comportamento das threads criadas
{
  long tid = (long) id ;															// ID da thread

  printf ("t%02ld: Olá!\n", tid) ;													// imprime uma mensagem de saudação juntamente com o ID da thread
  sleep (3) ;   																	// a função dorme por 3 segundos
  printf ("t%02ld: Tchau!\n", tid) ;												// a thread se despede
  pthread_exit (NULL) ;																// encerramento da thread
}

int main (int argc, char *argv[])
{
  pthread_t thread [NUM_THREADS] ;													// um vetor de threads é criado baseado no tamanho pré estabelecido
  pthread_attr_t attr ;   
  long i, status ;
  
// para permitir a operação "join" sobre as threads   
  pthread_attr_init (&attr) ;														// torna a thread capaz de receber atributos
  pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE) ; 					// uma thread ser "joinable" significa que ela pode ser atrelada ao programa principal

  for (i=0; i<NUM_THREADS; i++)
  {
    printf ("Main: criando thread %02ld\n", i) ;									// imprime uma mensagem para sinalizar a criação da proxima thread      
    status = pthread_create (&thread[i], &attr, threadBody, (void *) i) ;			// status recebe o valor de retorno da criação da thread
    if (status)																		// se houve erro na criação dessa thread
    {
      perror ("pthread_create") ;													// imprime uma mensagem de erro
      exit (1) ;
    }
  }
   
  for (i=0; i<NUM_THREADS; i++)
  {
    printf ("Main: aguardando thread %02ld\n", i);									// sinaliza que o programa principal vai aguardar essa thread terminar
    status = pthread_join (thread[i], NULL) ;										// atrela a thread ao programa principal
    if (status)																		// se houver erro de join
    {
      perror ("pthread_join") ;														// imprime mensagem de erro
      exit (1) ;
    }
  }
  printf ("Main: fim\n") ;															// ao fim da CRIAÇÃO das threads, o programa imprime "fim"
  pthread_attr_destroy (&attr) ;   													// limpa os atributos das threads
  pthread_exit (NULL) ;																// encerra a thread do programa principal, mantendo as outras threads funcionando independentemente para terminarem sua execução
}