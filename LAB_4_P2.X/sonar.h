/* 
 * File:   sonar.h
 * Author: Andres
 *
 * Created on April 25, 2016, 12:20 PM
 */
#ifndef SONAR_H
#define	SONAR_H

#include "lcd.h"
#include "timer.h"
#include <xc.h>


#define DIGITAL_RE6 TRISEbits.TRISE6
#define DIGITAL_RD4 TRISDbits.TRISD4 
#define DIGITAL_RD10 TRISDbits.TRISD10 
#define DIGITAL_RC1 TRISCbits.TRISC1
#define DIGITAL_RA0 TRISAbits.TRISA0 
#define DIGITAL_RF1 TRISFbits.TRISF1 

#define TRIGGER_S1 LATEbits.LATE6
#define TRIGGER_S2 LATDbits.LATD4
#define TRIGGER_S3 LATDbits.LATD10
#define ECHO_1 PORTCbits.RC1
#define ECHO_2 PORTFbits.RF1
#define ECHO_3 PORTAbits.RA0

#define ENABLED 1
#define DISABLED 0

#define BUFFER_TIME 0.005    //seconds
#define TRIGGER_TIME 12       //micro-seconds

//SONAR RESULT DEFINES FORMAT 1 1XXX
#define CLEAR 24                //1 1000
#define BLOCKED_RIGHT 25        //1 1001
#define BLOCKED_FRONT 26        //1 1010
#define BLOCKED_FAR 27          //1 1011
#define BLOCKED_LEFT 28         //1 1100
#define BLOCKED_LAR 29          //1 1101
#define BLOCKED_LAF 30          //1 1110
#define SURROUNDED 31           //1 1111

void initSonar();

int sonarSweep();

int performEcho(int sensor);

int sonarAssess(int t1, int t2, int t3);

int proximityCheck(int sonarResult, int display);


#endif	/* SONAR_H */

