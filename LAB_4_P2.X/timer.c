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
#define prescalar_1_T1 0
#define OVERFLOW 65535

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

void startTimer(){
    TMR1 = 0;
    PR1 = OVERFLOW;
    T1CONbits.TCKPS = prescalar_1_T1;
    IFS0bits.T1IF = 0;
    T1CONbits.ON = 1;          //TIMER ENABLED
}

int stopTimer(){
    
    int time = 0;
  
    T1CONbits.ON = 0;       //TIMER DISABLED
    time = TMR1;            //STORE ELAPSED CURRENT TIME
    T1CONbits.TCKPS = prescalar_8_T1; //reset pre-scalar back to 8
    IFS0bits.T1IF = 0;
    
    time = time/10;
    
    return time;            //RETURN ELAPSED TIME
}

