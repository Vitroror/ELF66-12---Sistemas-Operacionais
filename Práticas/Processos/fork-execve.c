/*
Criação de processos em UNIX, com execução de outro binário

Compilar com gcc -Wall fork-execve.c -o fork-execve

Carlos Maziero, DINF/UFPR 2020
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char *argv[], char *envp[])
{
  int retval ;																			//declaramos a variavel de retorno do fork()
   
  printf ("Ola, sou o processo %5d\n", getpid()) ;										//o processo pai imprime a mensagem
  retval = fork () ;																	//criação do processo filho
  printf ("[retval: %5d] sou %5d, filho de %5d\n", retval, getpid(), getppid()) ;		//processo filho se identifica
             
  if ( retval < 0 )       																//erro no fork ()
  {
    perror ("Erro: ") ;																	//imprime mensagem de erro no prompt
    exit (1) ;																			//retorna com 1 indicando interrupção
  }
  else 
    if ( retval > 0 )     																//sou o processo pai
      wait (0) ;																		//aguarda para responder imediatamente após o filho retornar a sua função
    else                  																//sou o processo filho
    {
      execve ("pratica1_executable", argv, envp) ;										//executa um programa
      perror ("Erro") ;																	//imprime mensagem de erro no prompt caso o execve() retorne, o que significaria um erro
    }

  printf ("Tchau de %5d!\n", getpid()) ;												//se despede. Logo após a despedida do processo filho, o processo pai se despede
  exit (0) ;																			//retorna 0, fim do programa
}