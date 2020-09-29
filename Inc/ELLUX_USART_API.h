/* Write a public API to interact with a hardware UART (USART) block, which should include:

1. Initialization of the module;

2. Setting the baud rate;

3. Setting the parity bit;

4. Setting the duration of the stop bit;

5. Setting the callback function for the receiver / transmitter;

6. Data array transfer function. Data array will be sent by the transmitter;


If the requirements described above are not enough to ensure the functionality of the module, then other parameters can be passed at your discretion.

Additionally:

1. The hardware block only works with interrupts;

2. The software module must be able to work with several hardware blocks (USART1, USART2, etc.);

3. Public API should consist of only one header file (platform-specific source files are not needed); */

#ifndef ELLUX_USART_API_H
#define ELLUX_USART_API_H

#include <cstdint>
#include <cstdbool>

/*
    Enum for status codes to avoid magic return numbers 
*/
typedef enum 
{
    USART_STATUS_OK = 0, // No Error code
    USART_STATUS_ERROR = 1,  // Error code
    USART_STATUS_NOT_INITIALISED = 2 // Error when user tries to invoke functions of not initialised usart module
    // Other types of errors' codes can be placed here

} ellux_usart_status_t;

/*
    Enum for choosing usart/uart mode of hardware module
    "defines are evil" 
*/
typedef enum
{
    ELLUX_USART_MODE_UART =1,
    ELLUX_USART_MODE_USART
} ellux_usart_mode_t;

/*
    Enum for status codes to avoid magic return numbers 
*/
typedef enum 
{
    PARITY_BIT_NONE = 0,
    PARITY_BIT_ODD = 1,
    PARITY_BIT_EVEN = 2
} ellux_usart_parity_t;

/*
    Struct to set init options
*/
typedef struct _ELLUX_USART_INIT_T
{
    uint32_t BaudRate; // BaudRate for asynchronous communication

    uint32_t StopBitDuration; // Number in baud ticks of stop bit length

    ellux_usart_parity_t ParityBit; // Parity bit checking mode

    ellux_usart_mode_t Mode;// UART/USART

    ellux_usart_hwblock_t HwBlock; // See above

    bool RxEnabled;// Condition to set reception

    bool TxEnabled;// Condition to set transmission

} ellux_usart_init_t;
/*
    Forward declaration of main handle struct
*/
typedef struct _ELLUX_USART_HANDLE_T ellux_usart_handle_t;
/*
    Enum for USART hardware blocks 
    Presumably, this variable must lead to memory address of given USART hardware block
    Which is platform-specific
*/
typedef enum
{
    ELLUX_USART1 = 1,
    ELLUX_USART2 = 2,
    ELLUX_USART3 = 3,
    ELLUX_USART4 = 4,
    ELLUX_USART5 = 5,
    ELLUX_USART6 = 6,
    ELLUX_USART7 = 7,
    ELLUX_USART8 = 8
    //  number of modules must be platform-specific
} ellux_usart_hwblock_t;

/*
    Aliases for callback function pointers
*/
typedef void (ellux_usart_callback_t)(ellux_usart_handle_t* const uarthandler);

/*
    Main struct to keep all options
*/
struct _ELLUX_USART_HANDLE_T
{
    ellux_usart_hwblock_t HwBlock;

    bool IsInited; // this variable may be useful if user want to change options after
                   // initialisation, for example to restart USART module to make changes

    ellux_usart_status_t ErrorCode;// 

    uint8_t* TxBufPtr; //Pointer to buffer to transmit

    uint32_t TxBufSize; // Stores length of message to transmit

    uint32_t TxCounter; // Counts how many bytes was transffered, needed to invoke Txcallback

    ellux_usart_callback_t* TxCallbackPtr; // pointer to transmission callback function

    uint8_t* RxBufPtr;//Pointer to buffer where to recieve

    uint32_t RxBufSize;// Stores length of message to recieve

    uint32_t RxCounter;// Counts how many bytes was recieved, needed to invoke Rxcallback

    ellux_usart_callback_t* RxCallbackPtr; // pointer to reception callback function
    
};

/*
    Fills in init struct with default parameters 
    After that, user must change them accordingly to his preferences
*/
void ELLUX_USART_DefaultConstruct(ellux_usart_init_t* const uartinit);

/*
    Initialisation of given USART/UART module 
    fields belonging to transmitter/reciever,are initialised with NULL
    Returns status code
*/
ellux_usart_status_t ELLUX_USART_Init(ellux_usart_handle_t* const uarthandler, const ellux_usart_init_t* const uartinit);

/*
    Denitialisation of given USART/UART module,
    Stopping all communications
    Returns status code
*/
ellux_usart_status_t ELLUX_USART_DeInit(ellux_usart_handle_t* const uarthandler);


/*
    Set callback function of given usart module to invoke upon end of transmission
    Must return error USART_STATUS_NOT_INITIALISED if wasn't invoked init function or/and flag TxEnabled was false
*/
ellux_usart_status_t ELLUX_USART_SetTxCallback(ellux_usart_handle_t* const uarthandler, ellux_usart_callback_t* const txcallback);

/*
    Set callback function of given usart module to invoke upon end of reception
    Must return error USART_STATUS_NOT_INITIALISED if wasn't invoked init function or/and flag RxEnabled was false
*/
ellux_usart_status_t ELLUX_USART_SetRxCallback(ellux_usart_handle_t* const uarthandler, ellux_usart_callback_t* const rxcallback);

/*
    Start transmiting function
    After transmitting full length, callback'll be invoked
    Must return error USART_STATUS_NOT_INITIALISED if transmitter is off
*/
ellux_usart_status_t ELLUX_USART_StartTransmit(ellux_usart_handle_t* const uarthandler, const uint8_t* const data, const uint32_t length);

/*
    Start recieveing function, data array must be preallocated
    After recieving full length, callback'll be invoked
    Must return error USART_STATUS_NOT_INITIALISED if reciever is off
*/
ellux_usart_status_t ELLUX_USART_StartRecieve(ellux_usart_handle_t* const uarthandler, uint8_t* const data, const uint32_t length);

#endif // ELLUX_USART_API_H
