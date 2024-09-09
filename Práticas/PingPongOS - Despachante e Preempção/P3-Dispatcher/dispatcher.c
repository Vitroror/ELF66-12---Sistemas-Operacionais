/*
******************************************************************************
LABORATÓRIO P3 - SISTEMAS OPERACIONAIS
DEPARTAMENTO ACADÊMICO DE ENGENHARIA ELETRÔNICA
UNIVERSIDADE TECNOLÓGICA FEDERAL DO PARANÁ

THIAGO MELCHER ARMÊNIO  - 2358565
VICTOR AUGUSTO DEL MONEGO - 2378345

dispatcher.c - CÓDIGO QUE DEFINE O DISPATCHER DE ESCALONAMENTO

*****************************************************************************
*/

#include "ppos.h"

extern void dispatcher_tiva ();

task_t Pang, Peng, Ping, Pong, Pung ;               


// Corpo das threads
void Body (void * arg)
{
  int i ;

  UARTprintf ("%s: inicio\n",(char *)arg) ;

  for (i=0; i<5; i++)
  {
    UARTprintf ("%s: %d\n", (char *) arg, i) ;
    task_yield ();                                  
  }
  
  UARTprintf ("%s: fim\n", (char *) arg) ;
  task_exit (0) ;                                   
}


void dispatcher_tiva ()
{
    UARTprintf ("dispatcher: inicio\n");

  ppos_init () ;                                            

  task_create (&Pang, Body, "    Pang") ;
  task_create (&Peng, Body, "        Peng") ;
  task_create (&Ping, Body, "            Ping") ;
  task_create (&Pong, Body, "                Pong") ;
  task_create (&Pung, Body, "                    Pung") ;
  task_yield () ;                                           

  UARTprintf ("dispatcher: fim\n");
}
