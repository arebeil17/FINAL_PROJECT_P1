/*
 * File:   timer.c
 * Authors: Andres D. Rebeil
 *
 * Created on Feb 16, 2016
 */

#include <xc.h>
#include "timer.h"
#include <math.h>
#define prescalar_8_T2 3
#define prescalar_8_T1 1

void initTimer1(){
    TMR1 = 0;
    T1CONbits.ON = 0;   //TURN TIMER ON
    T1CONbits.TCKPS = prescalar_8_T1;
    T1CONbits.TCS = 0;
    IFS0bits.T1IF = 0;
}

void initTimer2(){
    TMR2 = 0;
    T2CONbits.ON = 0;   //TURN TIMER ON
    //T2CONbits.TCKPS = prescalar_8_T2;
    T2CONbits.TCS = 0;
    IFS0bits.T2IF = 0;
}

void delayMs(unsigned int delay){
    IFS0bits.T1IF = 0;         // SETS FLAG
    TMR1 = 0;                  //RESET TIME
    PR1 = delay*1249;           
    
    T1CONbits.ON = 1;          //ENABLE CN
    while(IFS0bits.T1IF == 0); //DELAY TILL FLAG IS RAISED
    T1CONbits.ON = 0;          //DISABLE CN
}

void delayUs(unsigned int delay){
    
    IFS0bits.T1IF = 0;         // SETS FLAG
    TMR1 = 0;                  //RESET TIME
    PR1 = delay;           //SET PRESCALAR
   
    T1CONbits.ON = 1;          //ENABLE CN
    while(IFS0bits.T1IF == 0); //DELAY TILL FLAG IS RAISED
    T1CONbits.ON = 0;          //DISABLE CN
}

void delaySec(float delay){
    
    int limit = floor(20*delay);
    int i = 0;
    
    if(delay < 0.050){
        
        delayMs(1000*delay);
        
    } else if(delay > 0){
        for(i = 0; i < limit; i++){
            delayMs(50);
        }
    }
    
}

