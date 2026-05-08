#include "uLCD.h"
#include "uart.h"

static  ulcd_uart = 0;

// Send command to uLCD
static void ULCD_SendCommand(unsigned char cmd) {
    UART_SendChar(ulcd_uart, cmd);
}

// Send 16-bit value (MSB first)
static void ULCD_Send16(unsigned short value) {
    UART_SendChar(ulcd_uart, (value >> 8) & 0xFF);
    UART_SendChar(ulcd_uart, value & 0xFF);
}

// Wait for ACK from uLCD
static unsigned char ULCD_WaitACK(void) {
    unsigned int timeout = 100000;
    while(timeout-- > 0) {
        if(UART_DataAvailable(ulcd_uart)) {
            char response = UART_ReceiveChar(ulcd_uart);
            if(response == 0x06) return 1;  // ACK
        }
    }
    return 0;  // Timeout
}

// Initialize uLCD
void ULCD_Init(unsigned char uart_num) {
    ulcd_uart = uart_num;
    
    // uLCD uses 9600 baud by default
    UART_Init(uart_num, BAUD_9600);
    
    // Small delay for uLCD boot
    for(volatile int i = 0; i < 1000000; i++);
    
    // Clear screen to black
    ULCD_Clear(ULCD_BLACK);
}

// Clear screen with color
void ULCD_Clear(unsigned short color) {
    ULCD_SendCommand(0xFF);  // Clear screen command
    ULCD_SendCommand(0xCD);
    ULCD_Send16(color);
    ULCD_WaitACK();
}

// Draw a pixel
void ULCD_DrawPixel(unsigned short x, unsigned short y, unsigned short color) {
    ULCD_SendCommand(0xFF);  // Draw pixel command
    ULCD_SendCommand(0xC1);
    ULCD_Send16(x);
    ULCD_Send16(y);
    ULCD_Send16(color);
    ULCD_WaitACK();
}

// Draw a line
void ULCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color) {
    ULCD_SendCommand(0xFF);  // Draw line command
    ULCD_SendCommand(0xC8);
    ULCD_Send16(x1);
    ULCD_Send16(y1);
    ULCD_Send16(x2);
    ULCD_Send16(y2);
    ULCD_Send16(color);
    ULCD_WaitACK();
}

// Draw rectangle outline
void ULCD_DrawRect(unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned short color) {
    ULCD_SendCommand(0xFF);  // Draw rectangle command
    ULCD_SendCommand(0xC5);
    ULCD_Send16(x);
    ULCD_Send16(y);
    ULCD_Send16(x + w);
    ULCD_Send16(y + h);
    ULCD_Send16(color);
    ULCD_WaitACK();
}

// Draw filled rectangle
void ULCD_FillRect(unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned short color) {
    ULCD_SendCommand(0xFF);  // Filled rectangle command
    ULCD_SendCommand(0xC4);
    ULCD_Send16(x);
    ULCD_Send16(y);
    ULCD_Send16(x + w);
    ULCD_Send16(y + h);
    ULCD_Send16(color);
    ULCD_WaitACK();
}

// Draw circle outline
void ULCD_DrawCircle(unsigned short x, unsigned short y, unsigned short radius, unsigned short color) {
    ULCD_SendCommand(0xFF);  // Draw circle command
    ULCD_SendCommand(0xCC);
    ULCD_Send16(x);
    ULCD_Send16(y);
    ULCD_Send16(radius);
    ULCD_Send16(color);
    ULCD_WaitACK();
}

// Draw filled circle
void ULCD_FillCircle(unsigned short x, unsigned short y, unsigned short radius, unsigned short color) {
    ULCD_SendCommand(0xFF);  // Filled circle command
    ULCD_SendCommand(0xCB);
    ULCD_Send16(x);
    ULCD_Send16(y);
    ULCD_Send16(radius);
    ULCD_Send16(color);
    ULCD_WaitACK();
}

// Draw text
void ULCD_DrawText(unsigned short x, unsigned short y, const char *str, unsigned short color, unsigned short bg_color, unsigned char size) {
    ULCD_SendCommand(0xFF);  // Text command
    ULCD_SendCommand(0x73);  // String (graphics format)
    ULCD_Send16(x);
    ULCD_Send16(y);
    ULCD_SendCommand(size);  // Font size (1-3)
    ULCD_Send16(color);
    ULCD_Send16(bg_color);
    
    // Send string
    while(*str) {
        UART_SendChar(ulcd_uart, *str++);
    }
    UART_SendChar(ulcd_uart, 0x00);  // Null terminator
    
    ULCD_WaitACK();
}

// Set brightness (0-15)
void ULCD_SetBrightness(unsigned char level) {
    if(level > 15) level = 15;
    ULCD_SendCommand(0xFF);
    ULCD_SendCommand(0xD9);
    ULCD_SendCommand(level);
    ULCD_WaitACK();
}

// Put display to sleep
void ULCD_Sleep(void) {
    ULCD_SendCommand(0xFF);
    ULCD_SendCommand(0x5A);
    ULCD_SendCommand(0x01);
    ULCD_WaitACK();
}

// Wake display
void ULCD_Wake(void) {
    ULCD_SendCommand(0xFF);
    ULCD_SendCommand(0x59);
    ULCD_SendCommand(0x00);
    ULCD_WaitACK();
}

