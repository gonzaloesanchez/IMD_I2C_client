#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "flex_lcd.h"


#define SetBit(Dir, Bit) Dir |= (Bit)
#define RstBit(Dir, Bit) Dir &= ~(Bit)
#define ToggleBit(Dir, Bit) Dir ^= (Bit)

#define F1KHz 1000

//P1.4 = LCD_DB4
//P1.5 = LCD_DB5
//P1.6 = LCD_DB6
//P1.7 = LCD_DB7
#define LCD_DB4_PIN     BIT4
#define LCD_DB5_PIN     BIT5
#define LCD_DB6_PIN     BIT6
#define LCD_DB7_PIN     BIT7

#define LCD_EN_PIN      BIT0
#define LCD_RS_PIN      BIT1

#define LCD_DIR      P1DIR
#define LCD_OUT      P1OUT

//TODO: Este micro tiene conversor AD, hacer multiplexacion de los 6 botones con esto
//TODO: Si se complica puedo multiplexar con un CD (serie/paralelo)


lcd_hal funciones_hal_lcd;
uint32_t msTicks;


void ConfigTimer0_A1(void)  {

    TA0CTL = TACLR;                 //Hago un Clear al timer por las dudas
    TA0CCR0 = F1KHz-1;              // Establece el periodo del PWM, dado por CCR0 (1000)
    SetBit(TA0CTL, TASSEL_2 + ID_0 + MC_1 + TAIE);  //Fuente de clock SMCLK. Divisor de entrada: 1. Modo: Up
                                                   //habilito interrupcion, comienza la cuenta
}


/************************************************
 * Servicio de Interrupcion Generado por
 * CCR0 del Timer0A
************************************************/
#pragma vector = TIMER0_A1_VECTOR
__interrupt void    TimerA0_TA(void)  {
    uint16_t TAIV_Status;

    TAIV_Status = TA0IV;
    if(TAIV_Status == TA0IV_TAIFG)  {
        msTicks++;
        __low_power_mode_off_on_exit();

    }
}




void delay_ms(uint32_t delayTicks)  {
    msTicks = 0;
    while((msTicks) < delayTicks)
        LPM0;                                //SMCK queda activo, lo necesitamos para el tick
}


void output_nible(uint8_t X) {
    uint8_t puerto;

    /*
     * Se lee el valor actual del puerto para no afectar las señales de control
     * Se limpia la parte baja, se desplazan los bits recibidos como argumento
     * 4 posiciones (nibble bajo pasa a ser nibble alto)
     * Se hace un OR con el valor anterior de la parte baja del puerto
     * se saca por el puerto.
     *
     * Esto se puede hacer porque los pines [4..7] coinciden con LCD_DB[4..7]
     */

    puerto = LCD_OUT;
    puerto &= 0x0F;
    puerto |= (X & 0x0F) << 4;
    LCD_OUT = puerto;
}

void output_bit(uint16_t pin_id, bool out) {

    switch(pin_id)  {
    case RS:
        if(out)
            SetBit(LCD_OUT,LCD_RS_PIN);
        else
            RstBit(LCD_OUT,LCD_RS_PIN);
        break;

    case EN:
        if(out)
            SetBit(LCD_OUT,LCD_EN_PIN);
        else
            RstBit(LCD_OUT,LCD_EN_PIN);
        break;

    case DB4:
        break;

    case DB5:
        break;

    case DB6:
        break;

    case DB7:
        break;

    default:
        ;
    }
}



void main(void)  {
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer

	if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF)
	    while(1);                    // Si los valores de calibracion
	                                // estan borrados salta a FaultRoutine()

	BCSCTL1 = CALBC1_1MHZ;                 // Set range
	DCOCTL = CALDCO_1MHZ;                  // Set DCO step + modulation
	SetBit(BCSCTL3, LFXT1S_2);                     // LFXT1 = VLO
	RstBit(IFG1, OFIFG);                                 // Clear OSCFault flag
	SetBit(BCSCTL2, SELM_0 + DIVM_0 + DIVS_0);          // MCLK = DCO/1, SMCLK = DCO/4
	//MCLK = 1MHz; SMCLK = 1MHz
	ConfigTimer0_A1();

	SetBit(LCD_DIR, LCD_DB4_PIN | LCD_DB5_PIN | LCD_DB6_PIN | LCD_DB7_PIN);    //LCD_DB [4..7] -> Salida
	SetBit(LCD_DIR, LCD_EN_PIN | LCD_RS_PIN );    //Señales de control LCD (EN y RS) -> Salida


	funciones_hal_lcd.delay_ms = delay_ms;          //funcion de retardo
	funciones_hal_lcd.nibble_out = output_nible;    //funcion salida nible
	funciones_hal_lcd.bit_out = output_bit;         //funcion salida bit

	lcd_init(funciones_hal_lcd);

	lcd_gotoxy(0, 0);
	lcd_write("Hola mundo!");

	while(1)  	{
	    LPM0;
	}
}
