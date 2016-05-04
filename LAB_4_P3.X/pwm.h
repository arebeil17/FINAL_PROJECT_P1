
/* 
 * File:   pwm.h
 * Author: Andres
 *
 * Created on March 9, 2016, 11:02 PM
 */

#ifndef PWM_H
#define	PWM_H

#include <math.h>
#include <xc.h>
#include "adc.h"

#define ENABLE_ODC 0
#define DISABLE_ODC 1
#define ENABLED 1
#define FORWARD 1
#define BACKWARD 0
#define IDLE 2
#define SYNC 0
#define LEFT 3
#define RIGHT 4
#define INPUT 1
#define OUTPUT 0
#define FLIP_CW -180
#define FLIP_CCW 180

//CONTROL REGISTERS FOR PWM
#define OC1 0b1100
#define OC3 0b1011
#define MAP_R1_OC1 RPD0Rbits.RPD0R
#define MAP_R2_OC1 RPD1Rbits.RPD1R
#define MAP_L1_OC3 RPB5Rbits.RPB5R //USED TO BE F1
#define MAP_L2_OC3 RPD2Rbits.RPD2R
#define TRIS_RD0 TRISDbits.TRISD0
#define TRIS_RD1 TRISDbits.TRISD1
#define TRIS_RB5 TRISBbits.TRISB5 //USED TO BE F1
#define TRIS_RD2 TRISDbits.TRISD2
//CONTROL REGISTERS FOR ENABLES
#define TRIS_RG6 TRISGbits.TRISG6
#define TRIS_RG7 TRISGbits.TRISG7
#define ENABLE_R LATGbits.LATG6
#define ENABLE_L LATGbits.LATG7
//CONTROL REGISTERS FOR MOTOR CONTROL
#define TRIS_RE4 TRISEbits.TRISE4 //USED TO BE RB10
#define TRIS_RB11 TRISBbits.TRISB11
#define TRIS_RE2 TRISEbits.TRISE2 //USED TO BE RB12
#define TRIS_RB13 TRISBbits.TRISB13
#define MCG_R1 LATEbits.LATE4 //LEFT MOTOR GROUND 1 
#define MCG_R2 LATBbits.LATB11 //LEFT MOTOR GROUND 2
#define MCG_L1 LATEbits.LATE2 //RIGHT MOTOR GROUND 1
#define MCG_L2 LATBbits.LATB13 //RIGHT MOTOR GROUND 2

//FOR MOTOR CALIBRATION
#define OFFSET_LEFT 0.965
#define OFFSET_RIGHT 1.00
#define OFFSET_TURN 0.75

//Initializes all relevant motor control registers
void initMotorControl();

//Configures PWM for motor control
void initPWM();

//Adjusts and updates PWM for motor control
void updatePWM(float duty, int motor, int direction);

//Determines duty cycle based on input ADC Value
float getDutyCycle(int ADC_Value, int motor);
#endif	/* PWM_H */

