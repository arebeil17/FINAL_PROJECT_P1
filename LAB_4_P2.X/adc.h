/* 
 * File:   adc.h
 * Author: Andres
 *
 * Created on March 9, 2016, 10:19 PM
 */

#ifndef ADC_H
#define	ADC_H
#include <xc.h>
#include "lcd.h"


#define INPUT 1
#define OUTPUT 0
#define ENABLED 1
#define ADC_RESOLUTION 1024 //Max value of 10 bit ADC result
#define ADC_MIDPOINT 511

#define TRIS_RB15_POT TRISBbits.TRISB15
#define ANSEL_REG_B15 ANSELBbits.ANSB15

#define TRIS_RA7_VRN TRISAbits.TRISA7
#define TRIS_RA6_VRP TRISAbits.TRISA6

//CONTROL REGISTERS FOR LINE SENSORS
#define S3_AN5_RB5 TRISBbits.TRISB5
#define S2_AN3_RB3 TRISBbits.TRISB3
#define S1_AN1_RB1 TRISBbits.TRISB1

void initADC();


#endif	/* ADC_H */

