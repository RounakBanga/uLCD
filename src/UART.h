#ifndef UART_H
#define UART_H

#include <stdint.h>

// UART channels
#define UART0  0
#define UART1  1
#define UART2  2
#define UART3  3

// Common baud rates
#define BAUD_9600    9600
#define BAUD_19200   19200
#define BAUD_38400   38400
#define BAUD_57600   57600
#define BAUD_115200  115200

// Function prototypes
void UART_Init(uint8_t uart_num, uint32_t baudrate);
void UART_SendChar(uint8_t uart_num, char c);
void UART_SendString(uint8_t uart_num, const char *str);
void UART_SendNum(uint8_t uart_num, int num);
char UART_ReceiveChar(uint8_t uart_num);
uint8_t UART_DataAvailable(uint8_t uart_num);

// Convenience printf-like function
void UART_Printf(uint8_t uart_num, const char *format, ...);

#endif
