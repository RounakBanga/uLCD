#include "ulcd.h"
#include "uart.h"

static uint8_t ulcd_uart = 0;

// Send command to uLCD
static void ULCD_SendCommand(uint8_t cmd) {
    UART_SendChar(ulcd_uart, cmd);
}

// Send 16-bit value (MSB first)
static void ULCD_Send16(uint16_t value) {
    UART_SendChar(ulcd_uart, (value >> 8) & 0xFF);
    UART_SendChar(ulcd_uart, value & 0xFF);
}

// Wait for ACK from uLCD
static uint8_t ULCD_WaitACK(void) {
    uint32_t timeout = 100000;
    while(timeout-- > 0) {
        if(UART_DataAvailable(ulcd_uart)) {
            char response = UART_ReceiveChar(ulcd_uart);
            if(response == 0x06) return 1;  // ACK
        }
    }
    return 0;  // Timeout
}

// Initialize uLCD
void ULCD_Init(uint8_t uart_num) {
    ulcd_uart = uart_num;
    
    // uLCD uses 9600 baud by default
    UART_Init(uart_num, BAUD_9600);
    
    // Small delay for uLCD boot
    for(volatile int i = 0; i < 1000000; i++);
    
    // Clear screen to black
    ULCD_Clear(ULCD_BLACK);
}

// Clear screen with color
void ULCD_Clear(uint16_t color) {
    ULCD_SendCommand(0xFF);  // Clear screen command
    ULCD_SendCommand(0xCD);
    ULCD_Send16(color);
    ULCD_WaitACK();
}

// Draw a pixel
void ULCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    ULCD_SendCommand(0xFF);  // Draw pixel command
    ULCD_SendCommand(0xC1);
    ULCD_Send16(x);
    ULCD_Send16(y);
    ULCD_Send16(color);
    ULCD_WaitACK();
}

// Draw a line
void ULCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
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
void ULCD_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
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
void ULCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
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
void ULCD_DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color) {
    ULCD_SendCommand(0xFF);  // Draw circle command
    ULCD_SendCommand(0xCC);
    ULCD_Send16(x);
    ULCD_Send16(y);
    ULCD_Send16(radius);
    ULCD_Send16(color);
    ULCD_WaitACK();
}

// Draw filled circle
void ULCD_FillCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color) {
    ULCD_SendCommand(0xFF);  // Filled circle command
    ULCD_SendCommand(0xCB);
    ULCD_Send16(x);
    ULCD_Send16(y);
    ULCD_Send16(radius);
    ULCD_Send16(color);
    ULCD_WaitACK();
}

// Draw text
void ULCD_DrawText(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg_color, uint8_t size) {
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
void ULCD_SetBrightness(uint8_t level) {
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
