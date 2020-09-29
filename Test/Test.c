/* Based on the first step, write a software module that uses the public API UART (USART) software module to receive and send messages between two UART hardware blocks (USART).

 

Data for hardware block 1 (USART1):

                1. Baud rate is 9600;

                2. Receives data only (RX line is active);

 

Data for hardware block 2 (USART2):

                1. Baud rate is 115200;

                2. Transmits data only (TX line is active);

 

General data for the second task:

                1. After receiving data via USART1, they must be saved and sent via USART2;

                2. Transmission via USART2 is carried out in blocks of 128 bytes, so enough data must be accumulated before sending via USART2;

                3. At the moment of transmission of the data block via USART2, it is possible to receive new data via USART1, so new / transmitted data should not be lost or damaged;

                4. To check the functionality, a test should be created (the type of the test is at your discretion); */

#include "ELLUX_USART_API.h"
#include <string.h>

ellux_usart_handle_t Uart1, Uart2;
uint8_t rxbuff[128], txbuff[128];

static void Uart1Init();
static void Uart2Init();
static ellux_usart_callback_t Uart1RxCallback;
static ellux_usart_callback_t Uart2TxCallback;

int main(void)
{

    Uart1Init();
    Uart2Init();
    ELLUX_USART_Recieve(&Uart1, rxbuff, 128);
    while(1)
    {

    }
}

static void Uart1Init()
{
    ELLUX_USART_SetUsartModule(&Uart1, ELLUX_USART1);
    ELLUX_USART_SetMode(&Uart1, ELLUX_USART_MODE_UART);
    ELLUX_USART_SetBaudRate(&Uart1, 9600);
    ELLUX_USART_SetParityBit(&Uart1, PARITY_BIT_NONE);
    ELLUX_USART_SetStopBitDuration(&Uart1, 1);
    ELLUX_USART_SetTxSwitch(&Uart1, false);
    ELLUX_USART_SetRxSwitch(&Uart1, true);
    ELLUX_USART_SetRxCallback(&Uart1, &Uart1RxCallback);
    ELLUX_USART_Init(&Uart1);
}

static void Uart2Init()
{
    ELLUX_USART_SetUsartModule(&Uart2, ELLUX_USART1);
    ELLUX_USART_SetMode(&Uart2, ELLUX_USART_MODE_UART);
    ELLUX_USART_SetBaudRate(&Uart2, 115200);
    ELLUX_USART_SetParityBit(&Uart2, PARITY_BIT_NONE);
    ELLUX_USART_SetStopBitDuration(&Uart2, 1);
    ELLUX_USART_SetTxSwitch(&Uart2, true);
    ELLUX_USART_SetRxSwitch(&Uart2, false);
    ELLUX_USART_SetTxCallback(&Uart2, &Uart2TxCallback);
    ELLUX_USART_Init(&Uart2);
}

static void Uart1RxCallback(ellux_usart_handle_t* const uarthandler )
{
    memcpy(txbuff,rxbuff,128);
    ELLUX_USART_Recieve(&Uart1,rxbuff,128);
    ELLUX_USART_Transmit(&Uart2, txbuff,128);
}
static void Uart2TxCallback(ellux_usart_handle_t* const uarthandler )
{
    
}