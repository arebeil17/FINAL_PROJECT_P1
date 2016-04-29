/* 
 * File:   sensor.c
 * Author: Andres
 *
 * Created on April 13, 2016, 2:23 PM
 */

#include "sensor.h"
#include "lcd.h"
#include <xc.h>
#include <math.h>

int scanLineSensors(int result, int display){
    
    int ADC_V1 = 0, ADC_V2 = 0, ADC_V3 = 0;
    int SR1 = 0, SR2 = 0, SR3 = 0;
    float v_k1 = 0, v_k2 = 0, v_k3 = 0;
    
    if(IFS0bits.AD1IF == 1) 
    {   
        ADC_V1 = floor((ADC1BUF0 + ADC1BUF3 + ADC1BUF6 + ADC1BUF9 + ADC1BUFC)/5.0); 
        ADC_V2 = floor((ADC1BUF1 + ADC1BUF4 + ADC1BUF7 + ADC1BUFA + ADC1BUFD)/5.0);
        ADC_V3 = floor((ADC1BUF2 + ADC1BUF5 + ADC1BUF8 + ADC1BUFB + ADC1BUFE)/5.0);
        IFS0bits.AD1IF = 0;
        
        if(display) displayLineSensors(ADC_V1, ADC_V2, ADC_V3);
    
        v_k1 = (3.3/1023.0)*ADC_V1;
        v_k2 = (3.3/1023.0)*ADC_V2;
        v_k3 = (3.3/1023.0)*ADC_V3;

        //CHECK IF SENSORS HAVE REACHED LINE DETECTION LIMIT
        if(v_k1 < DETECTION_LIMIT_1) SR1 = 1;
        if(v_k2 < DETECTION_LIMIT_2) SR2 = 1;
        if(v_k3 < DETECTION_LIMIT_3) SR3 = 1;

        result = 4*(SR1) + 2*(SR2) + 1*(SR3);
    }     
    return result;
}

void displayLineSensors(int s1, int s2, int s3){
    
    char *string1[19],*string2[19];
    float v_k1 = 0.0, v_k2 = 0.0, v_k3 = 0.0;;
    int L1 = 0, L2 = 0, L3 = 0;
    
    v_k1 = (3.3/1023.0)*s1;
    v_k2 = (3.3/1023.0)*s2;
    v_k3 = (3.3/1023.0)*s3;
    moveCursorLCD(1,1);
    sprintf(string1,"%4.2f  %4.2f  %4.2f",v_k1, v_k2, v_k3);
    printStringLCD(string1);
    
    moveCursorLCD(1,2);
    if(v_k1 <= DETECTION_LIMIT_1) L1 = 1;
    if(v_k2 <= DETECTION_LIMIT_2) L2 = 1;
    if(v_k3 <= DETECTION_LIMIT_3) L3 = 1;
    
    sprintf(string2," %i   %i   %i",L1, L2, L3);

    printStringLCD(string2);
    
}


