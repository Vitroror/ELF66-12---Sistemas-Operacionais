// UNIVERSIDADE TECNOLOGICA FEDERAL DO PARANA
// DEPARTAMENTO ACADEMICO DE ENGENHARIA ELETRONICA
// VICTOR AUGUSTO DEL MONEGO - 2378345 
// Codigo processB_receiver.c : recebe mensagens de uma fila de mensagens POSIX
// No prompt do Linux: compilar usando: gcc -o processB_receiver processB_receiver.c -lrt

#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <unistd.h>

// operating system check
#if defined(_WIN32) || (!defined(__unix__) && !defined(__unix) && (!defined(__APPLE__) || !defined(__MACH__)))
#warning Este codigo foi planejado para ambientes UNIX (LInux, *BSD, MacOS). A compilacao e execucao em outros ambientes e responsabilidade do usuario.
#endif
#define QUEUE "/my_queue"
int main (int argc, char *argv[])
{
	mqd_t queue;																					//descrição da fila
	struct mq_attr attr;																			//define os atributos da fila
	int message;																					//declara a variavel que irá receber a mensagem
	attr.mq_maxmsg  = 10;																			//isto representa a capacidade do vetor de mensagens
	attr.mq_msgsize = sizeof(message);																//especifica o tamanho de cada mensagem
	attr.mq_flags   = 0;
	
	umask(0);																						//mascara de permissão
	
	// esta seção do código é referente a criação da fila com permissões 0666
	if((queue = mq_open(QUEUE, O_RDWR|O_CREAT, 0666, &attr)) < 0) 
	{
		perror("mq_open");																			//imprime mensagem de erro
		exit(1);
	}	
	//esta seção se refere à recepção de mensagens
	for(;;)
	{
		if((mq_receive (queue, (void*) &message, sizeof(message), 0)) < 0)							//verifica se houve erro de recepção
		{
			perror("mq_receive:");																	//imprime mensagem de erro
			exit(1);
		}
		printf("Recebido a mensagem com valor %d\n", message);										//imprime um aviso de recebimento de mensagem
	}
}