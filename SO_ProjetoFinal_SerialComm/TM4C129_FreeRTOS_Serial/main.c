/* FreeRTOS 10 Tiva Demo
 *
 * main.c
 *
 * Andy Kobyljanec
 *
 * This is a simple demonstration project of FreeRTOS 8.2 on the Tiva Launchpad
 * EK-TM4C1294XL.  TivaWare driverlib sourcecode is included.
 */

#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"


// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "driverlib/uart.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"

#define SAMPLE_COUNT 4 // Quantidade de amostras para média

volatile uint32_t sensorADCValue;


// Demo Task declarations
void LDRRead(void *pvParameters);
void SerialSend(void *pvParameters);
void UARTReceive(void *pvParameters);

// Main function
int main(void)
{
    // Initialize system clock to 120 MHz
    uint32_t output_clock_rate_hz;
    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               SYSTEM_CLOCK);
    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    // Initialize the GPIO pins for the Launchpad
    PinoutSet(false, false);

    // Set up the UART which is connected to the virtual COM port
    UARTStdioConfig(0, 57600, SYSTEM_CLOCK);

    // Ativa o ADC0 e o GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // Configura o pino PE3 como entrada analógica (ADC0, canal 0)
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);

    // Configura o sequenciador 3 do ADC0 com prioridade 0 e uma única amostra
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3);

    // Limpa o flag de interrupção para o sequenciador 3
    ADCIntClear(ADC0_BASE, 3);

    // Configura o pino PE0 do LED de saída
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0);

    // Create demo tasks
    xTaskCreate(LDRRead, (const portCHAR *)"Sensor",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(SerialSend, (const portCHAR *)"SendSerial",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(UARTReceive, (const portCHAR *)"UARTReceive",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();
    
    // Code should never reach this point
    return 0;
}


// Flash the LEDs on the launchpad
void LDRRead(void *pvParameters)
{
    while(1){
    uint32_t ui32ADC0Value[SAMPLE_COUNT];
    uint32_t ui32TempAvg;
    int i;

    // Realiza a conversão de várias amostras e calcula a média
    for(i = 0; i < SAMPLE_COUNT; i++) {
        // Dispara a conversão
        ADCProcessorTrigger(ADC0_BASE, 3);

        // Espera a conversão ser concluída
        while(!ADCIntStatus(ADC0_BASE, 3, false)) {}

        // Lê o valor do ADC
        ADCSequenceDataGet(ADC0_BASE, 3, &ui32ADC0Value[i]);

        // Limpa o flag de interrupção para o próximo passo
        ADCIntClear(ADC0_BASE, 3);
    }

    // Calcula a média dos valores lidos
    ui32TempAvg = 0;
    for(i = 0; i < SAMPLE_COUNT; i++) {
        ui32TempAvg += ui32ADC0Value[i];
    }
    ui32TempAvg /= SAMPLE_COUNT;

    sensorADCValue = ui32TempAvg;
    vTaskDelay(300);
    }
}


// Write text over the Stellaris debug interface UART port
void SerialSend(void *pvParameters)
{

    for (;;)
    {
        UARTprintf("%d\n",sensorADCValue);
        vTaskDelay(500);
    }
}

// Task to receive commands from UART
void UARTReceive(void *pvParameters)
{
    char buffer[16]; // Ajuste o tamanho conforme necessário
    int i = 0;

    while (1)
    {
        if (UARTCharsAvail(UART0_BASE))
        {
            char receivedChar = UARTCharGet(UART0_BASE);

            // Verificar se é um caractere válido
            if (receivedChar != '\r' && receivedChar != '\n')
            {
                buffer[i++] = receivedChar;

                // Verificar se chegamos ao fim do buffer
                if (i >= sizeof(buffer))
                {
                    // Buffer cheio, tratar erro ou limpar buffer
                    i = 0;
                }
            }
            else if (i > 0)
            {
                // Fim do comando, adicionar null terminator
                buffer[i] = '\0';

                // Processar comando
                if (strcmp(buffer, "LED_ON") == 0)
                {
                    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_PIN_0); // Acender LED
                    UARTprintf("LED ligado\n");
                }
                else if (strcmp(buffer, "LED_OFF") == 0)
                {
                    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0, 0); // Apagar LED
                    UARTprintf("LED desligado\n");
                }

                // Limpar buffer para o próximo comando
                i = 0;
            }
        }

        vTaskDelay(100); // Ajuste conforme necessário
    }
}
/*  ASSERT() Error function
 *
 *  failed ASSERTS() from driverlib/debug.h are executed in this function
 */
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}
