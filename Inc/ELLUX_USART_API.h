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
    uint32_t BaudRate;
    uint32_t StopBitDuration;
    ellux_usart_parity_t ParityBit;
    ellux_usart_mode_t mode;
    ellux_usart_hwblock_t HwBlock;
    bool RxEnabled;
    bool TxEnabled;
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
    uint8_t* TxBufPtr;
    uint32_t TxBufSize;
    uint32_t TxCounter;
    ellux_usart_callback_t* TxCallbackPtr;
    uint8_t* RxBufPtr;
    uint32_t RxBufSize;
    uint32_t RxCounter;
    ellux_usart_callback_t* RxCallbacckPtr;
    
};

/*
    Create init struct with default parameters 
    After that, user must change them accordingly his preferences
    Returns status code
*/
ellux_usart_parity_t ELLUX_USART_Create(ellux_usart_init_t* const uartinit);

/*
    Initialisation of given USART/UART module 
    User must set all init options or it'll return error
    Returns status code
*/
ellux_usart_status_t ELLUX_USART_Init(ellux_usart_handle_t* const uarthandler, const ellux_usart_init_t* const uartinit);

/*
    Denitialisation of given USART/UART module 
    Returns status code
*/
ellux_usart_status_t ELLUX_USART_DeInit(ellux_usart_handle_t* const uarthandler);


/*
    Set callback function of given usart module to invoke after transmit
    Returns status code
*/
ellux_usart_status_t ELLUX_USART_SetTxCallback(ellux_usart_handle_t* const uarthandler, ellux_usart_callback_t* const txcallback);

/*
    Set callback function of given usart module to invoke upon recieve
    Returns status code
*/
ellux_usart_status_t ELLUX_USART_SetRxCallback(ellux_usart_handle_t* const uarthandler, ellux_usart_callback_t* const rxcallback);

/*
    Transmit function, data array must be allocated or declared statically
    Definitely returns error if transmitter is off
*/
ellux_usart_status_t ELLUX_USART_Transmit(ellux_usart_handle_t* const uarthandler, const uint8_t* const data, const uint32_t length);

ellux_usart_status_t ELLUX_USART_Recieve(ellux_usart_handle_t* const uarthandler, uint8_t* const data, const uint32_t length);

#endif // ELLUX_USART_API_H
