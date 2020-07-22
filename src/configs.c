/*
 * configs.c
 *
 *  Created on: 22 jul. 2020
 *      Author: Gonza
 */

#include "configs.h"



void ConfigTimer0_A1(void)  {

    TA0CTL = TACLR;                 //Hago un Clear al timer por las dudas
    TA0CCR0 = F1KHz-1;              // Establece el periodo del PWM, dado por CCR0 (1000)
    SetBit(TA0CTL, TASSEL_2 + ID_0 + MC_1 + TAIE);  //Fuente de clock SMCLK. Divisor de entrada: 1. Modo: Up
                                                   //habilito interrupcion, comienza la cuenta
}


void configI2C(void)  {
    SetBit(UCB0CTL0,UCMODE_3 | UCSYNC);            //UCB0 modo I2C Slave
    SetBit(UCB0CTL1, UCSWRST);                     //modo receiver, held on reset (no importa el clock porque es slave)

    SetBit(UCB0I2COA, I2C_ADDRR);                  //seteo de propia direccion (slave)

    SetBit(P1SEL,SDA_PIN | SCL_PIN);                //multiplexacion de pines
    SetBit(P1SEL2,SDA_PIN | SCL_PIN);

}
