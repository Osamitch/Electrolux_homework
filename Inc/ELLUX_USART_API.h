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
// Enum for status codes to avoid magic return numbers 
typedef enum 
{
    STATUS_OK =0, // No Error code
    STATUS_ERROR=1  // Error code
    // Other types of errors' codes can be placed here

} ellux_usart_status_t;

// Enum for choosing usart/uart mode of hardware module
// "defines are evil"
typedef enum
{
    ELLUX_USART_MODE_UART =1,
    ELLUX_USART_MODE_USART
} ellux_usart_mode_t;


/*
    Struct to set init options
*/
typedef struct _ELLUX_USART_INIT_T
{
    ellux_usart_mode_t mode =ELLUX_USART_MODE_UART;
    uint32_t BaudRate;
    uint8_t ParityBit;
    uint32_t StopBitDuration;
    bool RxOn;
    bool TxOn;

} ellux_usart_init_t;

/*
    Enum for USART hardware blocks 
    Presumably, this variable must lead to memory address of given USART hardware block
    Which is platfrom-specific
*/
typedef enum _ELLUX_USART_HWBLOCK_T
{
    ELLUX_USART1 = 1,
    ELLUX_USART2 = 2,
    //  number of this addresses must be platfrom-specific
} ellux_usart_hwblock_t;

/*
    Main sturct to keep all options
*/
typedef struct _ELLUX_USART_HANDLE_T
{
    ellux_usart_hwblock_t HwBlock;
    ellux_usart_init_t Init;
    bool IsInited; // this variable may be useful if user want to change options after
                   // initialisation, for example to restart USART module to make changes
    
    void* TxCallback(ellux_usart_handle_t* const huart, void* arg);
    void* RxCallback(ellux_usart_handle_t* const huart, void* arg);
} ellux_usart_handle_t;

/*
    Initialisation of given USART/UART module 
    User must set all init options and hardware block number via direct change in
    handle struct's fields or wrapping functions listed below, else it'll return error
    Returns status code
*/
ellux_usart_status_t ELLUX_USART_Init(ellux_usart_handle_t* const huart);

/*
    Wrapping Functions to set individual init options before or after initialisation
    Return status code
*/
ellux_usart_status_t ELLUX_USART_SetMode(ellux_usart_handle_t* const huart, const ellux_usart_mode_t mode);

ellux_usart_status_t ELLUX_USART_SetBaudRate(ellux_usart_handle_t* const huart, const uint32_t baudrate);

ellux_usart_status_t ELLUX_USART_SetParityBit(ellux_usart_handle_t* const huart, const uint8_t paritybit);

ellux_usart_status_t ELLUX_USART_SetStopBitDuration(ellux_usart_handle_t* const huart, const uint32_t stopbitduration);

ellux_usart_status_t ELLUX_USART_SetRxOnOff(ellux_usart_handle_t* const huart, const bool rxon);

ellux_usart_status_t ELLUX_USART_SetTxOnOff(ellux_usart_handle_t* const huart, const bool txon);

/*
    Callback functions setters 
*/
ellux_usart_status_t ELLUX_USART_SetTxCallback(ellux_usart_handle_t* const huart, void* const callback(ellux_usart_handle_t* const huart,void* arg ));

ellux_usart_status_t ELLUX_USART_SetRxCallback(ellux_usart_handle_t* const huart, void* const callback(ellux_usart_handle_t* const huart,void* arg ));

/*
    Transmit function, data array must be allocated or declared statically
    Definitely returns error if transmitter is off
*/
ellux_usart_status_t ELLUX_USART_Transmit(ellux_usart_handle_t* const huart, uint8_t * const data, uint32_t length);

#endif // ELLUX_USART_API_H
