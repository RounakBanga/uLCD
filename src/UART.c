#include "UART.h"
#include "LPC17xx.h"
#include <stdarg.h>
#include <stdio.h>

// Get UART peripheral pointer based on channel
static LPC_UART_TypeDef* UART_GetPeripheral(uint8_t uart_num) {
    switch(uart_num) {
        case 0: return (LPC_UART_TypeDef*)LPC_UART0;
        case 1: return (LPC_UART_TypeDef*)LPC_UART1;
        case 2: return LPC_UART2;
        case 3: return LPC_UART3;
        default: return (LPC_UART_TypeDef*)LPC_UART0;
    }
}

// Initialize UART
void UART_Init(uint8_t uart_num, uint32_t baudrate) {
    LPC_UART_TypeDef *uart = UART_GetPeripheral(uart_num);
    uint32_t pclk;
    uint16_t divider;
    
    // Power on UART peripheral
    if(uart_num == 0) {
        LPC_SC->PCONP |= (1 << 3);   // UART0
        LPC_SC->PCLKSEL0 &= ~(3 << 6);
        LPC_SC->PCLKSEL0 |= (1 << 6); // PCLK = CCLK/1
    } else if(uart_num == 1) {
        LPC_SC->PCONP |= (1 << 4);   // UART1
        LPC_SC->PCLKSEL0 &= ~(3 << 8);
        LPC_SC->PCLKSEL0 |= (1 << 8);
    } else if(uart_num == 2) {
        LPC_SC->PCONP |= (1 << 24);  // UART2
        LPC_SC->PCLKSEL1 &= ~(3 << 16);
        LPC_SC->PCLKSEL1 |= (1 << 16);
    } else if(uart_num == 3) {
        LPC_SC->PCONP |= (1 << 25);  // UART3
        LPC_SC->PCLKSEL1 &= ~(3 << 18);
        LPC_SC->PCLKSEL1 |= (1 << 18);
    }
    
    // Configure pins for UART0 (TXD0=P0.2, RXD0=P0.3)
    if(uart_num == 0) {
        LPC_PINCON->PINSEL0 &= ~(0xF << 4);
        LPC_PINCON->PINSEL0 |= (0x5 << 4);  // TXD0, RXD0
    }
    
    // 8-bit data, 1 stop bit, no parity, enable DLAB
    uart->LCR = 0x83;
    
    // Set baud rate (assuming PCLK = 96MHz)
    pclk = 96000000;
    divider = pclk / (16 * baudrate);
    
    uart->DLL = divider & 0xFF;
    uart->DLM = (divider >> 8) & 0xFF;
    
    // Disable DLAB
    uart->LCR = 0x03;
    
    // Enable FIFO
    uart->FCR = 0x07;
}

// Send a single character
void UART_SendChar(uint8_t uart_num, char c) {
    LPC_UART_TypeDef *uart = UART_GetPeripheral(uart_num);
    
    // Wait until transmit buffer is empty
    while(!(uart->LSR & (1 << 5)));
    
    uart->THR = c;
}

// Send a string
void UART_SendString(uint8_t uart_num, const char *str) {
    while(*str) {
        UART_SendChar(uart_num, *str++);
    }
}

// Send a number
void UART_SendNum(uint8_t uart_num, int num) {
    char buffer[16];
    int i = 0;
    int is_negative = 0;
    
    if(num < 0) {
        is_negative = 1;
        num = -num;
    }
    
    if(num == 0) {
        buffer[i++] = '0';
    }
    
    while(num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    if(is_negative) {
        buffer[i++] = '-';
    }
    
    // Send in correct order
    for(int j = i - 1; j >= 0; j--) {
        UART_SendChar(uart_num, buffer[j]);
    }
}

// Receive a character
char UART_ReceiveChar(uint8_t uart_num) {
    LPC_UART_TypeDef *uart = UART_GetPeripheral(uart_num);
    
    // Wait until data is available
    while(!(uart->LSR & 0x01));
    
    return uart->RBR;
}

// Check if data is available
uint8_t UART_DataAvailable(uint8_t uart_num) {
    LPC_UART_TypeDef *uart = UART_GetPeripheral(uart_num);
    return (uart->LSR & 0x01);
}

// Printf-like function
void UART_Printf(uint8_t uart_num, const char *format, ...) {
    char buffer[128];
    va_list args;
    
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    UART_SendString(uart_num, buffer);
}
