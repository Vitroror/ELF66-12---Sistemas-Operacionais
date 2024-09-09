/*
Criação de processos em UNIX.
Compilar com gcc -Wall fork.c -o fork
Carlos Maziero, DINF/UFPR 2020
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
int main ()
{
  int retval ; 																			// declarando a variavel retval, que receberá o retorno do fork()
  printf ("Ola, sou o processo %5d\n", getpid()) ;  									// o comando getpid() é utilizado para apresentar o Process ID do processo pai
  retval = fork () ;																	// a variável retval recebe o retorno do fork()
  printf ("[retval: %5d] sou %5d, filho de %5d\n", retval, getpid(), getppid()) ;		// requisitado que o processo se identifique, declarando o retorno do fork, apresentando seu próprio pid e também o pid do seu processo pai. na prática, retval indicará qual o id do proximo processo a se apresentar e ,nessa situação, como ambos os processos irão responder ao comando o processo pai se apresenta, e logo após o processo filho também se apresenta. Podemos perceber que o processo filho está se apresentando, pois ele irá apresentar um retval de zero. Caso contrário, seu retval sendo maior que 0 ira apresentar o pid do processo filho que irá se apresentar posteriormente.
  if ( retval < 0 )    																	// erro no fork()
  {
    perror ("Erro") ;																	// nesse caso, imprimir a mensagem de erro, e retornar 1 para a função main, indicando interrupção.
    exit (1) ;
  }
  else
    if ( retval > 0 )  																	// este trecho está sendo executado apenas pelo processo pai
      wait (0) ;																		// nesse caso, não esperar e imediatamente executar o comando final, após o processo filho.
    else               																	// por último, este trecho será executado apenas pelo processo filho. 
      sleep (5) ;																		// o processo filho dorme por 5 segundos.
  printf ("Tchau de %5d!\n", getpid()) ;												// o processo irá se "despedir". Seu processo filho irá se despedir antes e, devido ao tempo de sleep, não será imediatamente
  exit (0) ;																			// a função retorna 0 indicando término com êxito
 }