/* 
 * File:   pwm.c
 * Author: Andres
 *
 * Created on March 9, 2016, 11:02 PM
 */

#include "pwm.h"

void initMotorControl(){
    
    ANSELG = 0x0000;
    ANSELE = 0x0000;
            
    TRIS_RG6 = OUTPUT; 
    TRIS_RG7 = OUTPUT; 
    ENABLE_L = 0; ENABLE_R = 0;
    TRIS_RE4 = OUTPUT;
    TRIS_RB11 = OUTPUT; 
    TRIS_RE2 = OUTPUT; 
    TRIS_RB13 = OUTPUT; 
    MCG_L1 = 0; MCG_L2 = 0; MCG_R1 = 0; MCG_R2 = 0;
    ODCEbits.ODCE4 = ENABLED; 
    ODCBbits.ODCB11 = ENABLED; 
    ODCEbits.ODCE2 = ENABLED;  
    ODCBbits.ODCB13 = ENABLED; 
}

void initPWM(){
    
    initMotorControl();
    ANSELD = 0x0000;
    ANSELF = 0x0000;
    
    //-------------------------------------
    //FOR MOTOR 1 LEFT
    //-------------------------------------
    OC1CON = 0x0000; //Turn off OC1 while doing setup
    
    T2CONbits.TCKPS = 0;
    //PERIOD CONTROL
    PR2 = 1023;
    TMR2 = 0;
    //PULSE WIDTH CONTROL OCxRS / PR2 = duty cycle
    OC1RS = 255;    //REAL-TIME UPDATE
    OC1R = 0;       //UPDATED BY OCxRS EVERY CLOCK CYCLE
    
    T2CONbits.TON = 1; //ENABLE CN
    
    OC1CONbits.OCTSEL = 0;
    OC1CONbits.OCM = 6; //6
    OC1CONbits.ON = 1;
    OC1CONSET = 0x8000; //Enable OC1
    
    //-------------------------------------
    //FOR MOTOR 2 RIGHT
    //-------------------------------------
    OC3CON = 0x0000; //Turn off OC3 while doing setup
    
    T3CONbits.TCKPS = 0;
    //PERIOD CONTROL
    PR3 = 1023;
    TMR3 = 0;
    //PULSE WIDTH CONTROL OCxRS / PR2 = duty cycle
    OC3RS = 255;    //REAL-TIME UPDATE
    OC3R = 0;       //UPDATED BY OCxRS EVERY CLOCK CYCLE
    
    T3CONbits.TON = 1; //ENABLE CN
    
    OC3CONbits.OCTSEL = 1; //selects Timer to be used
    OC3CONbits.OCM = 6; //6
    OC3CONbits.ON = 1;
    OC3CONSET = 0x8000; //Enable OC3
}
//Updates PWM and pin mapping for motor control
void updatePWM(float duty, int motor, int direction){
    
    TMR2 = 0; TMR3 = 0; //RESET TIMERS
    if(motor == 1){
        OC1RS = floor(duty*1023);
    }else if(motor == 2){
        OC3RS = floor(duty*1023);
    }else if(motor == 0){
        OC1RS = floor(duty*1023); //RIGHT MOTOR
        OC3RS = (floor(duty*1023)*OFFSET_LEFT); //LEFT MOTOR   SABOTAGE 2
    }
    if(direction == BACKWARD){
        ENABLE_L = 1; ENABLE_R = 1;
        MCG_L1 = ENABLE_ODC;  MCG_R1 = ENABLE_ODC;
        MCG_L2 = DISABLE_ODC; MCG_R2 = DISABLE_ODC;
        MAP_R1_OC1 = 0;
        MAP_R2_OC1 = OC1;
        MAP_L1_OC3 = 0;
        MAP_L2_OC3 = OC3;
    }else if(direction == FORWARD){
        ENABLE_L = 1; ENABLE_R = 1;
        MCG_L1 = DISABLE_ODC; MCG_R1 = DISABLE_ODC;
        MCG_L2 = ENABLE_ODC;  MCG_R2 = ENABLE_ODC;
        MAP_R1_OC1 = OC1;//fix
        MAP_R2_OC1 = 0;
        MAP_L1_OC3 = OC3;//fix
        MAP_L2_OC3 = 0;
    }else if(direction == LEFT){
        OC3RS = (floor(duty*1023)*OFFSET_TURN); //LEFT MOTOR  
        ENABLE_L = 1; ENABLE_R = 1;                     // SABOTAGE 4
        MCG_L1 = DISABLE_ODC; MCG_R1 = DISABLE_ODC;
        MCG_L2 = ENABLE_ODC;  MCG_R2 = ENABLE_ODC;
        MAP_R1_OC1 = OC1;
        MAP_R2_OC1 = 0;
        MAP_L1_OC3 = OC3;
        MAP_L2_OC3 = 0;
    }else if(direction == RIGHT){
        OC1RS = floor(duty*1023*OFFSET_TURN); 
        ENABLE_L = 1; ENABLE_R = 1;
        MCG_L1 = DISABLE_ODC; MCG_R1 = DISABLE_ODC;
        MCG_L2 = ENABLE_ODC;  MCG_R2 = ENABLE_ODC;
        MAP_R1_OC1 = OC1;
        MAP_R2_OC1 = 0;
        MAP_L1_OC3 = OC3;
        MAP_L2_OC3 = 0;
    }else if(direction == FLIP_CCW){
        ENABLE_L = 1; ENABLE_R = 1;
        MCG_L1 = DISABLE_ODC; MCG_R1 = ENABLE_ODC;
        MCG_L2 = ENABLE_ODC;  MCG_R2 = DISABLE_ODC;
        MAP_R1_OC1 = OC1;//fix
        MAP_R2_OC1 = 0;
        MAP_L1_OC3 = 0;//fix
        MAP_L2_OC3 = OC3;
    }else if(direction == FLIP_CW){
        ENABLE_L = 1; ENABLE_R = 1;
        MCG_L1 = ENABLE_ODC; MCG_R1 = DISABLE_ODC;
        MCG_L2 = DISABLE_ODC;  MCG_R2 = ENABLE_ODC;
        MAP_R1_OC1 = 0;//fix
        MAP_R2_OC1 = OC1;
        MAP_L1_OC3 = OC3;//fix
        MAP_L2_OC3 = 0;
    }else{
        ENABLE_L = 0; ENABLE_R = 0;
        MCG_L1 = ENABLE_ODC; MCG_R1 = ENABLE_ODC;
        MCG_L2 = ENABLE_ODC;  MCG_R2 = ENABLE_ODC;
        MAP_R1_OC1 = 0;
        MAP_R2_OC1 = 0;
        MAP_L1_OC3 = 0;
        MAP_L2_OC3 = 0;
    }
}

//Determines duty cycle for specified motor based on given ADC Value
float getDutyCycle(int ADC_Value, int motor){
    
    float duty = 0.0;
    int diff = abs(ADC_Value - ADC_MIDPOINT); //ADC_MIDPOINT = 511
    
    if(motor == LEFT){
        if(ADC_Value <= ADC_MIDPOINT) duty = 1.0; //ADC_RESOLUTION = 1024
        else duty = (ADC_RESOLUTION - 2*diff)/(ADC_RESOLUTION * 1.0);
    }else if(motor == RIGHT){
        if(ADC_Value >= ADC_MIDPOINT) duty = 1.0;
        else duty = (ADC_RESOLUTION - 2*diff)/(ADC_RESOLUTION * 1.0);
    }
    return duty;
}