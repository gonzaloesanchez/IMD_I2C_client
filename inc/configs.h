/*
 * configs.h
 *
 *  Created on: 22 jul. 2020
 *      Author: Gonza
 */

#ifndef INC_CONFIGS_H_
#define INC_CONFIGS_H_

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "hwDefs.h"

#define F1KHz 1000


void ConfigTimer0_A1(void);
void configI2C(void);



#endif /* INC_CONFIGS_H_ */
