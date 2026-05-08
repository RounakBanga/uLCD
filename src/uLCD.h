#ifndef ULCD_H
#define ULCD_H

#include <stdint.h>

// Colors (RGB565 format)
#define ULCD_BLACK       0x0000
#define ULCD_WHITE       0xFFFF
#define ULCD_RED         0xF800
#define ULCD_GREEN       0x07E0
#define ULCD_BLUE        0x001F
#define ULCD_YELLOW      0xFFE0
#define ULCD_CYAN        0x07FF
#define ULCD_MAGENTA     0xF81F
#define ULCD_ORANGE      0xFD20
#define ULCD_PURPLE      0x801F

// Screen dimensions
#define ULCD_WIDTH       128
#define ULCD_HEIGHT      128

// Function prototypes
void ULCD_Init(uint8_t uart_num);
void ULCD_Clear(uint16_t color);
void ULCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ULCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ULCD_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ULCD_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void ULCD_DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color);
void ULCD_FillCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color);
void ULCD_DrawText(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg_color, uint8_t size);
void ULCD_SetBrightness(uint8_t level);
void ULCD_Sleep(void);
void ULCD_Wake(void);

#endif
