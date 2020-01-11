// flex_lcd.c
//Pines a definir


#include "flex_lcd.h"

/*
 * Esta libreria solo funciona sin lectura del bus y con interfaz de 4bits
 */


static lcd_hal lcd_hw;

/*
 ****************************************************************************
                           FUNCIONES RELEVANTES
 ****************************************************************************

lcd_send_byte(int1 address, int8 n)
   Esta funcion envia el dato a escribir al LCD. Si address = 0, lo que se envia
   es un comando. Mientras este argumento sea distinto de cero, se enviara un
   dato.
   n es el dato a transmitir.

lcd_write(char c[])

   Una funcion mas completa de escritura, diseñada por mi ;). Soporta salto de
   linea con \n cadenas constantes o puestas en vectores.

lcd_read_byte(void)

   Lee un dato de la ram, lo devuelve como entero. Parece que es secuencial...
   Usar goto x-y

lcd_gotoxy(int8 x, int8 y)

   Como el LCD trabaja como una matriz de a x b (donde a es la cant de filas y b
   la cantidad de columnas) siendo la mas comun 2 x 16 (la parte no visible se
   extiende hasta 39) y numeradas desde el 0, como una matriz en C.
   Ejemplo: quiero posicionarme en el casillero 5 de la segunda linea
      lcd_gotoxy(1,4)

   OJO!!! Los lcd de 1 x 16 trabajan como si fueran 2 x 8 uno al lado del otro
   por lo que las direcciones del 0 al 7 corresponden a la primer linea y las
   direcciones del 8 al 15 son en realidad las 0 al 7 de la segunda linea
   Entonces, el LCD queda asi dividido:

   | 0x00 | 0x01 | ....... | 0x07 | 0x40 | 0x41 |.....| 0x47|
     (0,0)  (0,1)            (0,7) (1,0)  (1,1)        (1,7)

lcd_init(void)

   Esta funcion debe estar presente antes de hacer uso del LCD

lcd_setcursor(short visible, short blink)

   Esta funcion enciende el cursor o lo apaga y lo hace parpadear. Los
   argumentos son de tipo booleano (es decir, enteros, pero 1 o 0)
   Si visible = 1 ==> Cursor visible. Si blink = 1 ==> Parpadea.

void generate_char(int8 posicion,int8 V[])
   Esta funcion sirve para generar caracteres especiales, la posicion es el
   lugar de la memoria del LCD donde se guarda (va desde 0 a 7).
   El vector V[] tiene que estar en algun lugar para pasarlo como argumento.
   Este vector contiene los datos que conforman el caracter especial; para
   acceder a este caracter, solo debemos llamar a la funcion lcd_send_byte y
   enviar la dirreccion de memoria donde esta almacenado el caracter especial:
   ej: lcd_send_byte(1,0); ==> es el caracter especial que esta en la posicion 0
   de la CG Ram.

clear_lcd(void)
   Esta funcion limpia de texto el lcd y retorna a la posicion 00 del mismo
 */

/*
//-------------------------------------
void lcd_send_nibble(int8 nibble)  {
    // Note:  !! converts an integer expression
    // to a boolean (1 or 0).
    output_bit(DB4, !!(nibble & 1));
    output_bit(DB5, !!(nibble & 2));
    output_bit(DB6, !!(nibble & 4));
    output_bit(DB7, !!(nibble & 8));

    delay_us(10);          //Ojo antes era 1 ciclo
    output_high(EN);
    delay_us(20);          //Ojo antes era 2 us
    output_low(EN);
}


*/


static void toogle_enable_pin(void) {
    lcd_hw.delay_ms(1);         //toogle del pin EN necesario
    lcd_hw.bit_out(EN,HIGH);
    lcd_hw.delay_ms(1);
    lcd_hw.bit_out(EN,LOW);
}



//----------------------------------------
// Send a byte to the LCD.
//----------------------------------------

void lcd_send_byte(bool address, uint8_t n)  {
    lcd_hw.bit_out(RS,LOW)

    lcd_hw.delay_ms(1);    //Ojo antes 60 us

    if(address)
        lcd_hw.bit_out(RS,HIGH);
    else
        lcd_hw.bit_out(RS,LOW);

    lcd_hw.delay_ms(1);
    lcd_hw.bit_out(EN,LOW);

    lcd_hw.nibble_out(n >> 4);
    toogle_enable_pin();

    lcd_hw.nibble_out(n & 0x0F);
    toogle_enable_pin();
}

//----------------------------
void lcd_init(lcd_hal hw_functions)  {

    lcd_hw = hw_functions;

    lcd_hw.bit_out(RS,LOW);
    lcd_hw.bit_out(EN,LOW);
    lcd_hw.delay_ms(15);

    /*
     * TODO: Ver que corcho son los MAGIC NUMBERS
     */
    lcd_hw.nibble_out(0x03);
    toogle_enable_pin();
    lcd_hw.delay_ms(5);

    lcd_hw.nibble_out(0x03);
    toogle_enable_pin();
    lcd_hw.delay_ms(1);

    lcd_hw.nibble_out(0x03);
    toogle_enable_pin();

    lcd_hw.nibble_out(0x02);
    toogle_enable_pin();


    lcd_send_byte(0,(0x20 | (lcd_type << 2)));  // Func set: ver lcd_type
    lcd_hw.delay_ms(5);

    lcd_send_byte(0,0x08);                    // Display off
    lcd_hw.delay_ms(5);

    lcd_send_byte(0,0x01);                    // Clear display
    lcd_hw.delay_ms(5);

    lcd_send_byte(0,0x06);                    // Increment cursor
    delay_ms(5);

    lcd_send_byte(0,0x0E);                    //Cursor on
    lcd_hw.delay_ms(5);


    // If the R/W signal is not used, then
    // the busy bit can't be polled.  One of
    // the init commands takes longer than
    // the hard-coded delay of 60 us, so in
    // that case, lets just do a 5 ms delay
    // after all four of them.

}

//----------------------------

void lcd_gotoxy(uint8_t x, uint8_t y)
{
    uint8_t address;

    if(x == 1)
        address = ADDR_LINE_TWO;
    else
        address = 0;

    address += y;
    lcd_send_byte(0, 0x80 | address);
}

//-----------------------------
void lcd_write(char c[])  {
    int8 cont = 0;
    while(c[cont] != NULL) {
        if (c[cont] == '\n')
            lcd_gotoxy(1,0);
        else
            lcd_send_byte(1, c[cont]);
        /*
      #if (lcd_type == 0)
      if (cont == 7)
         lcd_gotoxy(1,0);
      #elif (lcd_type > 0)
      if (cont == 15)
         lcd_gotoxy(1,0);
      #endif
         */
        cont++;
    }
}

//------------------------------

void lcd_setcursor(bool visible, bool blink) {
    uint8_t Aux = 0x0C;

    if (visible)
        Aux |= 0x02;        //set high bit 1
    if (blink)
        Aux |= 0x01;        //set high bit 0
    lcd_send_byte(0, Aux);
}

#ifdef CGRAM
void generate_char(uint8_t posicion, uint8_t V[])  {
    uint8_t a;

    posicion *= 8; //ojo, la CG Ram esta por byte de renglon, si no multiplicamos
                    //por 8 se descuajeringa y se mezclan todos los pixeles
    lcd_send_byte(0, (0x40 | posicion));
    for (a = 0; a < 8;a++)
        lcd_send_byte(1,V[a]);
}
#endif

void clear_lcd()  {
    lcd_send_byte(0,1);
    lcd_gotoxy(0,0);
}

/* Esta funcion lo que hace es limpiar la linea actual (borrarla) y vuelve al
   principio. 
   El argumento se toma como sigue:
   Linea 1, 16x2 = 0 
   Linea 2, 16x2 = 1
   Linea 1, 20x2 = 2 
   Linea 2, 20x2 = 3

 */


/*
 * TODO: No existe una funcion para limpiar la linea? Debe ser hecha escribiendo?
 */
void clear_lcd_line(uint8_t linea, uint8_t cant_chars)  {
    uint8_t address;

    switch (linea)  {
    case 0:
        address = 0;
        break;
    case 1:
        address = ADDR_LINE_TWO;
        break;

    default:
        ;
    }

    lcd_send_byte(0, 0x80 | address);         //ir a principio de linea

    while (--cant_chars) {
        lcd_send_byte(1, ' ');                 //se envian cuantos caracteres en
    }                                         //blanco tenga la linea

    lcd_send_byte(0, 0x80 | address);           //ir a principio de linea
}

#endif
