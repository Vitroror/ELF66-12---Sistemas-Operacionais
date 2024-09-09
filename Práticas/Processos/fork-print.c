/*
Criação de processos em UNIX, com impressão de valores de variável.

Compilar com gcc -Wall fork-print.c -o fork-print

Carlos Maziero, DINF/UFPR 2020
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main ()
{
  int retval, x ;														//declara a variavel que retorna o valor do fork

  x = 0 ;																//declara a variavel x como 0
  retval = fork () ;													//fork()
  printf ("No processo %5d x vale %d\n", getpid(), x) ;					//o processo pai e flho se identificam e apresentam o valor de x atual
  x++ ;																	// x se incrementa em 1
  
  if ( retval < 0 )														//erro no fork()
  {
    perror ("Erro") ;										 			//imprime mensagem de erro
    exit (1) ;															//retorna 1, interrupção
  }
  else
    if ( retval > 0 )													//sou o processo pai
    {
      x = 0 ;															//reinicia x para 0
      wait (0) ;														//o pai segue a proxima instrução imediatamente após o filho seguir
    }
    else																//sou o processo filho
    {
      x++ ;																//x se incrementa em 1
      sleep (5) ;														//o filho aguarda 5 segundos antes de seguir a proxima instrução
    }

  printf ("No processo %5d x vale %d\n", getpid(), x) ;					//pai e o filho apresentam os seus respectivos valores de x, sendo que o pai retorna 0 e o filho retorna 2
  exit (0) ;
}