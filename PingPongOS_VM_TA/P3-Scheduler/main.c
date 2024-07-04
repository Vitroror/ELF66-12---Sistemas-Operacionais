/*
******************************************************************************
LABORAT�RIO P3 - SISTEMAS OPERACIONAIS
DEPARTAMENTO ACAD�MICO DE ENGENHARIA ELETR�NICA
UNIVERSIDADE TECNOL�GICA FEDERAL DO PARAN�

THIAGO MELCHER ARM�NIO  - 2358565
VICTOR AUGUSTO DEL MONEGO - 2378345

main.c - C�DIGO PRINCPAL. PROCESSO QUE CHAMA O DISPATCHER

*****************************************************************************
*/

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "drivers/pinout.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

//Fun��es de teste do LAB P3
void dispatcher_tiva(void);


// Clock do sistema em Hz
uint32_t g_ui32SysClock;


// Rotina de erro chamada se driver library eventualmente encontrar um erro.
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif


// Configura��o do UART e seus pinos. Deve ser chamada antes de UARTprintf().
void
ConfigureUART(void)
{

    // Habilita o perif�rico usado pelo UART.
      ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Habilita UART0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    // Configura��o dos pinos para UART.
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Inicializar console I/O para UART.
    UARTStdioConfig(0, 115200, g_ui32SysClock);
}


// Corpo da fun��o.

int main(void)
{

    // Rodar PLL a 120MHz.
    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                SYSCTL_CFG_VCO_480), 120000000);

    // Configurar pinos de dispositivo.
    PinoutSet(false, false);

    // Inicializa o UART.
    ConfigureUART();


    UARTprintf("VICTOR AUGUSTO DEL MONEGO\nTHIAGO MELCHER ARMENIO\n\nAdentrando processo main.c\n");

    // Fun��o usada para chamar o dispatcher
	dispatcher_tiva();
	
}
