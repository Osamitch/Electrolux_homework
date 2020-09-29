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
#include <assert.h>

#define MAX_BUFFERS 2 // Represents length of queue for trasmitter/resiver,
                      // presumably it should be enough to handle given task, but it can be easily increased

ellux_usart_handle_t Uart1, Uart2;
uint8_t Buffers[MAX_BUFFERS][128];
uint8_t RxIndex=0, TxIndex=0;
ellux_usart_status_t ErrorCode;
bool Recieved=false, Transmitted = false;

static void Uart1Init();
static void Uart2Init();
static ellux_usart_callback_t Uart1RxCallback;
static ellux_usart_callback_t Uart2TxCallback;

int main(void)
{

    Uart1Init();
    Uart2Init();
    ErrorCode =ELLUX_USART_StartRecieve(&Uart1, Buffers[RxIndex], 128);
    assert(ErrorCode==0);
    while(1)
    {
        // Buffers are used in turn, FIFO style
        if(Recieved)
        {
            Recieved=false;
            Transmitted =false;
            ErrorCode =ELLUX_USART_StartTransmit(&Uart2,Buffers[TxIndex],128);
            assert(ErrorCode==0);
            TxIndex = (TxIndex+1) % MAX_BUFFERS;
        }
        if(Transmitted && ((TxIndex+1)%MAX_BUFFERS < RxIndex))
        {
            Transmitted=false;
            ErrorCode =ELLUX_USART_StartTransmit(&Uart2,Buffers[TxIndex],128);
            assert(ErrorCode==0);
            TxIndex = (TxIndex+1) % MAX_BUFFERS;
        }
        // To not burn down our MCU,it's recommended to insert sleep(1 ms) here
    }
}

static void Uart1Init()
{
    ellux_usart_init_t Uart1Init;
    ELLUX_USART_DefaultConstruct(&Uart1Init);
    Uart1Init.BaudRate= 9600;
    Uart1Init.HwBlock = ELLUX_USART1;
    Uart1Init.Mode = ELLUX_USART_MODE_UART;
    Uart1Init.ParityBit = PARITY_BIT_NONE;
    Uart1Init.StopBitDuration= 1;
    Uart1Init.TxEnabled = false;
    Uart1Init.RxEnabled = true;
    ErrorCode = ELLUX_USART_Init(&Uart1, &Uart1Init);
    assert(ErrorCode==0); // assertion to check initialisation
    ELLUX_USART_SetRxCallback(&Uart1, &Uart1RxCallback);
}

static void Uart2Init()
{
    ellux_usart_init_t Uart2Init;
    ELLUX_USART_DefaultConstruct(&Uart2Init);
    Uart2Init.BaudRate= 115200;
    Uart2Init.HwBlock = ELLUX_USART2;
    Uart2Init.Mode = ELLUX_USART_MODE_UART;
    Uart2Init.ParityBit = PARITY_BIT_NONE;
    Uart2Init.StopBitDuration= 1;
    Uart2Init.TxEnabled = true;
    Uart2Init.RxEnabled = false;
    ErrorCode = ELLUX_USART_Init(&Uart2, &Uart2Init);
    assert(ErrorCode==0);// assertion to check initialisation
    ELLUX_USART_SetTxCallback(&Uart2, &Uart2TxCallback);
}

static void Uart1RxCallback(ellux_usart_handle_t* const uarthandler )
{
    Recieved=true;
    RxIndex = (RxIndex+1) % MAX_BUFFERS;
    ErrorCode = ELLUX_USART_StartRecieve(&Uart1,Buffers[RxIndex],128);
    assert(ErrorCode==0);
}
static void Uart2TxCallback(ellux_usart_handle_t* const uarthandler )
{
    Transmitted = true;
}