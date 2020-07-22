/*
 * hwDefs.h
 *
 *  Created on: 22 jul. 2020
 *      Author: Gonza
 */

#ifndef INC_HWDEFS_H_
#define INC_HWDEFS_H_


#define SetBit(Dir, Bit) Dir |= (Bit)
#define RstBit(Dir, Bit) Dir &= ~(Bit)
#define ToggleBit(Dir, Bit) Dir ^= (Bit)


/********************************************************
 * Definiciones de pines para LCD
********************************************************/
//P3.0 = LCD_DB4
//P3.1 = LCD_DB5
//P3.2 = LCD_DB6
//P3.3 = LCD_DB7
#define LCD_DB4_PIN     BIT0
#define LCD_DB5_PIN     BIT1
#define LCD_DB6_PIN     BIT2
#define LCD_DB7_PIN     BIT3

#define LCD_EN_PIN              BIT4
#define LCD_RS_PIN              BIT6
#define LCD_BACKLIGHT_PIN       BIT5

#define LCD_DIR      P3DIR
#define LCD_OUT      P3OUT


/********************************************************
 * Definiciones de pines para Leds en placa
********************************************************/
#define LED_R_PIN   BIT3
#define LED_A_PIN   BIT4
#define LED_V_PIN   BIT5

#define LEDS_DIR    P1DIR
#define LEDS_OUT    P1OUT



/********************************************************
 * Definiciones de pines para I2C
********************************************************/
#define SDA_PIN     BIT7
#define SCL_PIN     BIT6

#define I2C_DIR     P1DIR
#define I2C_OUT     P1OUT

#define I2C_ADDRR   0x7A


#endif /* INC_HWDEFS_H_ */
