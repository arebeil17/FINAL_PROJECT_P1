/* 
 * File:   sonar.c
 * Author: Andres
 *
 * Created on April 25, 2016, 12:20 PM
 */
#include "sonar.h"

void initSonar(){
    
    DIGITAL_RE6 = OUTPUT;  //PIN 6 J10
    DIGITAL_RD4 = OUTPUT;  //PIN 14 J10
    DIGITAL_RD10 = OUTPUT; //PIN 16 J10

    
    //FOR ECHO_1 INPUT PIN 18 ON J10 
    DIGITAL_RC1 = INPUT;                // Configure PIN as input
    CNCONCbits.ON = 1;                  // Enable overall interrupt
    CNENCbits.CNIEC1 = ENABLED;         // Enable pin CN
    CNPUCbits.CNPUC1 = DISABLED;        // Enable pull-up resistor
    IFS1bits.CNCIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 7;                  // Configure interrupt priority
    IPC8bits.CNIS = 3;                  // Configure the interrupt sub-priority
    IEC1bits.CNCIE = DISABLED;           // Enable interrupt for C pins
    
    //FOR ECHO_2 INPUT PIN 8 ON J11
    DIGITAL_RF1 = INPUT;                // Configure PIN as input
    CNCONFbits.ON = 1;                  // Enable overall interrupt
    CNENFbits.CNIEF1 = ENABLED;         // Enable pin CN
    CNPUFbits.CNPUF1 = DISABLED;        // Enable pull-up resistor
    IFS1bits.CNFIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 7;                  // Configure interrupt priority
    IPC8bits.CNIS = 3;                  // Configure the interrupt sub-priority
    IEC1bits.CNFIE = DISABLED;           // Enable interrupt for A pins
    
    //FOR ECHO_3 INPUT PIN 7 ON J5
    DIGITAL_RA0 = INPUT;                // Configure PIN as input
    CNCONAbits.ON = 1;                  // Enable overall interrupt
    CNENAbits.CNIEA0 = ENABLED;         // Enable pin CN
    CNPUAbits.CNPUA0 = DISABLED;        // Enable pull-up resistor
    IFS1bits.CNAIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 7;                  // Configure interrupt priority
    IPC8bits.CNIS = 3;                  // Configure the interrupt sub-priority
    IEC1bits.CNAIE = DISABLED;           // Enable interrupt for A pins

}

int sonarSweep(){
    
    int t1 = 0, t2 = 0, t3 = 0;
    int sonarResult = 0;
    int proximityResult = CLEAR;
    
    t1 = performEcho(1); delaySec(BUFFER_TIME);
    t2 = performEcho(2); delaySec(BUFFER_TIME);
    t3 = performEcho(3); delaySec(BUFFER_TIME);
    
    sonarResult = sonarAssess(t1, t2, t3);
    
    proximityResult = proximityCheck(sonarResult, 1);
    
    return proximityResult;
}

int performEcho(int sensor){
    
    int time = 0, elapsed = 0;
    int echoTrack1 = 0, echoTrack2 = 0, echoTrack3 = 0;
    
    //TRIGGER SPECIFIED SENSOR
    if(sensor == 1) {
        IFS1bits.CNCIF = 0;          //reset flag
        TRIGGER_S1 = ENABLED; delayUs(TRIGGER_TIME); TRIGGER_S1 = DISABLED;
        while(IFS1bits.CNCIF == 0);  //wait for echo
        echoTrack1 = ECHO_1;
        IFS1bits.CNCIF = 0;         //reset flag
    }else if(sensor == 2){
        IFS1bits.CNFIF = 0;          //reset flag
        TRIGGER_S2 = ENABLED; delayUs(TRIGGER_TIME); TRIGGER_S2 = DISABLED;
        while(IFS1bits.CNFIF == 0);  //wait for echo
        echoTrack2 = ECHO_2;
        IFS1bits.CNFIF = 0; //reset flag
    }else if(sensor == 3){
        IFS1bits.CNAIF = 0;           //reset flag
        TRIGGER_S3 = ENABLED; delayUs(TRIGGER_TIME); TRIGGER_S3 = DISABLED;
        while(IFS1bits.CNAIF == 0);  //wait for echo
        echoTrack3 = ECHO_3;
        IFS1bits.CNAIF = 0; //reset flag
    }
    
    //START TIMING HIGH TIME OF ECHO
    startTimer(); 
   
    if(sensor == 1 ){
        while((IFS1bits.CNCIF == 0) || ECHO_1){
            if(IFS0bits.T1IF == 1){
                elapsed++;
                TMR1 = 0;
                IFS0bits.T1IF = 0;
            }
            if(elapsed >= 2) break;
        }; //time until echo pulse ends
    }else if(sensor == 2 ){
        while((IFS1bits.CNFIF == 0) || ECHO_2){
            if(IFS0bits.T1IF == 1){
                elapsed++;
                TMR1 = 0;
                IFS0bits.T1IF = 0;
            }
            if(elapsed >= 2) break;
        }; //time until echo pulse ends
    }else {
         while((IFS1bits.CNAIF == 0) || ECHO_3){
            if(IFS0bits.T1IF == 1){
                elapsed++;
                TMR1 = 0;
                IFS0bits.T1IF = 0;
            }
            if(elapsed >= 2) break;
        }; //time until echo pulse ends
    }
    //ECHO FINISHED STOP TIMER AND RECORD HIGH TIME
    time = stopTimer();
    
    if(elapsed > 0) time = time + (65535*elapsed)/10;
    
    IFS1bits.CNCIF = 0; //reset flag
    IFS1bits.CNFIF = 0; //reset flag
    IFS1bits.CNAIF = 0; //reset flag
    
    return time; //returns time in micro-seconds
}


int sonarAssess(int t1, int t2, int t3){
    
    char *string1[19];
    float dist_1 = 0.0, dist_2 = 0.0, dist_3 = 0.0;
    int sonarResult = 0;
    
    dist_1 = (((t1/1000000.0)*340)/2)*100.0; //calculate distance in cm
    dist_2 = (((t2/1000000.0)*340)/2)*100.0; //calculate distance in cm
    dist_3 = (((t3/1000000.0)*340)/2)*100.0; //calculate distance in cm
    
    //clearLCD();
    moveCursorLCD(1,1);
    sprintf(string1,"%4.0f %4.0f %4.0f", dist_1, dist_2, dist_3);
    printStringLCD(string1);
    
    if(dist_1 <= 20) sonarResult = sonarResult + 4;
    if(dist_2 <= 20) sonarResult = sonarResult + 2;
    if(dist_3 <= 20) sonarResult = sonarResult + 1;
    
    return sonarResult;
}

int proximityCheck(int sonarResult, int display){
    
    int proximityStatus = CLEAR;
    int L = 0, F = 0, R = 0;
    char *string[19];
    
    if(sonarResult == 0) proximityStatus = CLEAR;              //000
    else if(sonarResult == 1) proximityStatus = BLOCKED_RIGHT; //001
    else if(sonarResult == 2) proximityStatus = BLOCKED_FRONT; //010
    else if(sonarResult == 3) proximityStatus = BLOCKED_FAR;   //011
    else if(sonarResult == 4) proximityStatus = BLOCKED_LEFT;  //100
    else if(sonarResult == 5) proximityStatus = BLOCKED_LAR;   //101
    else if(sonarResult == 6) proximityStatus = BLOCKED_LAF;   //110
    else if(sonarResult == 7) proximityStatus = SURROUNDED;    //111
 
    if(display){
        moveCursorLCD(1,2);
        if(sonarResult >= 4) L = 1;
        if(sonarResult%2) R = 1;
        if(sonarResult > 5 || ((sonarResult < 4) && (sonarResult > 1))) F = 1;
        sprintf(string,"L:%2i F:%2i R:%2i",L,F,R );
        printStringLCD(string);
    }
    
    return proximityStatus;
}
