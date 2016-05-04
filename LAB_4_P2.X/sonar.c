/* 
 * File:   sonar.c
 * Author: Andres
 *
 * Created on April 25, 2016, 12:20 PM
 */
#include "sonar.h"

void initSonar(){
    //FOR TRIGGER PINS
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
    IEC1bits.CNFIE = DISABLED;           // Enable interrupt for F pins
    
    //FOR ECHO_3 INPUT PIN 5 RG1 ON J11
    DIGITAL_RG1 = INPUT;                // Configure PIN as input
    CNCONGbits.ON = 1;                  // Enable overall interrupt
    CNENGbits.CNIEG1 = ENABLED;         // Enable pin CN
    CNPUGbits.CNPUG1 = DISABLED;        // Enable pull-up resistor
    IFS1bits.CNGIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 7;                  // Configure interrupt priority
    IPC8bits.CNIS = 3;                  // Configure the interrupt sub-priority
    IEC1bits.CNGIE = DISABLED;           // Enable interrupt for G pins
    
    TRIGGER_S1 = DISABLED;
    TRIGGER_S2 = DISABLED;
    TRIGGER_S3 = DISABLED;
}

int sonarSweep(int AVOID){
    
    int t1 = 0, t2 = 0, t3 = 0;
    int temp1 = 0, temp2 = 0;
    int sonarResult = 0;
    int proximityResult = CLEAR;
    
    while(t1 == 0) {t1 = getTime(1); }
    while(t2 == 0) {t2 = getTime(2); }
    while(t3 == 0) {t3 = getTime(3); }
    
    sonarResult = sonarAssess(t1, t2, t3, AVOID);
    
    proximityResult = proximityCheck(sonarResult, 1);
    
    if((t1 + t2 + t3) > TOTAL_ELAPSED_TIME) delayMs(5);
    
    return proximityResult;
}

int getTime(int sensor){
    
    int time1 = 0;
    int time2 = 0;
    int validTime = 0;
    
    while(time1 == 0){
        time1 = performEcho(sensor);
        delayUs(300);
    }
    
    while(time2 == 0){
        time2 = performEcho(sensor);
        delayUs(300);
    }
    
    if(abs(time1 - time2) < CHANGE) validTime = (time1 + time2)/2;
    
    return validTime;
}
int performEcho(int sensor){
    
    int time = 0, elapsed = 0;
    int echoTrack1 = 0, echoTrack2 = 0, echoTrack3 = 0;
    int delay = 0;
    
    //TRIGGER SPECIFIED SENSOR
    if(sensor == 1) { IFS1bits.CNCIF = 0;
        if(ECHO_1){
            IFS1bits.CNCIF = 0;  return 0;} 
        TRIGGER_S1 = ENABLED; delayUs(TRIGGER_TIME); TRIGGER_S1 = DISABLED;
        while(IFS1bits.CNCIF == 0 || !ECHO_1){ //wait for echo
            delayUs(CHECK_TIME); delay++; if(delay > 100) return 0; 
        }; //check for no echo
        echoTrack1 = ECHO_1; IFS1bits.CNCIF = 0;         //reset flag
    }else if(sensor == 2){ IFS1bits.CNFIF = 0; 
        if(ECHO_2){ 
            IFS1bits.CNFIF = 0;  return 0;}        //reset flag
        TRIGGER_S2 = ENABLED; delayUs(TRIGGER_TIME); TRIGGER_S2 = DISABLED;
        while(IFS1bits.CNFIF == 0 || !ECHO_2){//wait for echo
            delayUs(CHECK_TIME); delay++; if(delay > 100) return 0; 
        }; //check for no echo
        echoTrack2 = ECHO_2; IFS1bits.CNFIF = 0; //reset flag
    }else if(sensor == 3){ IFS1bits.CNGIF = 0;
        if(ECHO_3){
            IFS1bits.CNGIF = 0;  return 0;}        //reset flag 
        TRIGGER_S3 = ENABLED; delayUs(TRIGGER_TIME); TRIGGER_S3 = DISABLED;
        while(IFS1bits.CNGIF == 0 || !ECHO_3){//wait for echo
            delayUs(CHECK_TIME); delay++; if(delay > 100) return 0; 
        }; //check for no echo
        echoTrack3 = ECHO_3; IFS1bits.CNGIF = 0;         //reset flag
    }else return 0;
    
    //START TIMING HIGH TIME OF ECHO
    startTimer(); 
   
    if(sensor == 1 ){
        while(ECHO_1){
            if(IFS0bits.T1IF == 1){
                elapsed++;
                TMR1 = 0;
                IFS0bits.T1IF = 0;
            }
            if(elapsed >= ELAPSED_CNT) break;
        }; //time until echo pulse ends
    }else if(sensor == 2 ){
        while(ECHO_2){
            if(IFS0bits.T1IF == 1){
                elapsed++;
                TMR1 = 0;
                IFS0bits.T1IF = 0;
            }
           if(elapsed >= ELAPSED_CNT) break;
        }; //time until echo pulse ends
    }else if(sensor == 3) {
         while(ECHO_3){
            if(IFS0bits.T1IF == 1){
                elapsed++;
                TMR1 = 0;
                IFS0bits.T1IF = 0;
            }
            if(elapsed >= ELAPSED_CNT) break;  
        }; //time until echo pulse ends
    }
    //ECHO FINISHED STOP TIMER AND RECORD HIGH TIME
    time = stopTimer();
    
    if(elapsed > 0) time = time + (65535*elapsed)/10;
    
    //if(elapsed >= ELAPSED_CNT) delayMs(5);
    
    IFS1bits.CNCIF = 0; //reset flag
    IFS1bits.CNFIF = 0; //reset flag
    IFS1bits.CNGIF = 0; //reset flag
    
    return time; //returns time in micro-seconds
}


int sonarAssess(int t1, int t2, int t3, int AVOID){
    
    char *string1[19];
    float dist_1 = 0.0, dist_2 = 0.0, dist_3 = 0.0;
    int sonarResult = 0;
    
    dist_1 = (((t1/1000000.0)*340)/2)*100.0; //calculate distance in cm
    dist_2 = (((t2/1000000.0)*340)/2)*100.0; //calculate distance in cm
    dist_3 = (((t3/1000000.0)*340)/2)*100.0; //calculate distance in cm
    
    clearLCD();
    moveCursorLCD(1,1);
    sprintf(string1,"%4.0f %4.0f %4.0f", dist_1, dist_2, dist_3);
    printStringLCD(string1);
    
    if(!AVOID){
        if(dist_1 <= CRITICAL_LEFT) sonarResult = sonarResult + 4;
        if(dist_2 <= CRITICAL_FRONT) sonarResult = sonarResult + 2;
        if(dist_3 <= CRITICAL_RIGHT) sonarResult = sonarResult + 1;
    }else{
        if((dist_1 <= (CRITICAL_LEFT + BUFFER_REGION)) 
           && (dist_1 >= (CRITICAL_LEFT - BUFFER_REGION))){ 
            sonarResult = sonarResult + 4;
        }
        if(dist_2 <= CRITICAL_FRONT){
            sonarResult = sonarResult + 2;
        }
        if((dist_3 <= (CRITICAL_RIGHT + BUFFER_REGION)) 
           && (dist_3 >= (CRITICAL_RIGHT - BUFFER_REGION))) {
            sonarResult = sonarResult + 1;
        }
    }
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

//Get distance of single from a single sensor
int getDistance(int sensor){
    
    int time = 0;
    int distance = 0; //cm
    
    //perform trigger and echo process until valid time returned
    while(time == 0){ time = getTime(sensor);}
    
    distance = (((time/1000000.0)*340)/2)*100.0; //calculate distance in cm
    
    return distance;
}
