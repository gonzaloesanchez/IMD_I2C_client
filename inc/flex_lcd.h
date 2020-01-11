#ifndef _FLEX_C
#define _FLEX_C

#include <stdint.h>
#include <stdbool.h>

//Definiciones de constantes para los pines de datos
#define DB4   0x00
#define DB5   0x01
#define DB6   0x02
#define DB7   0x04

//Definiciones de constantes para los pines de control
#define RS    0x08
#define EN    0x10


#define BLINK_ON 1
#define BLINK_OFF 0
#define CURSOR_ON 1
#define CURSOR_OFF 0

#define LOW     false
#define HIGH    true

// If you only want a 6-pin interface to your LCD, then
// connect the R/W pin on the LCD to ground, and comment
// out the following line.

//#define USE_LCD_RW   1

//========================================

#define LCD_TYPE 2       // 0= 5x7, 1 linea en 16x2 o 1/2 en 16x1
                         // 1= 5x10, 1 linea en 16x2 o 1/2 en 16x1
                         // 2= 5x7, 2 lineas en 16x2 o 1 en 16x1
                         // 3= 5x10 2 lineas en 16x2 o 1 en 16x1


#define ADDR_LINE_TWO 0x40 // LCD RAM address for the 2nd line


//===============================================================
/*  Este define que pongo aca es para la Character Generator RAM, si no se
    usa, lo comentas al define y listo */

#define CGRAM 1


/*
 * DEFINICION DE ESTRUCTURA DE FUNCIONES HAL
 */

struct _lcd_hal  {
    void (*delay_us)(uint16_t X);
    void (*delay_ms)(uint32_t X);
    void (*nibble_out)(uint8_t X);
    void (*bit_out)(uint8_t pin, bool out);
};

typedef struct _lcd_hal lcd_hal;

#endif
