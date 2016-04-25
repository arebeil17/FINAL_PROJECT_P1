
/*
* File:   leds.c
* Author: Andres D. Rebeil
*
* Created on December 27, 2014, 1:31 PM
*/

#include <xc.h>
#include "leds.h"
#define INPUT 1
#define OUTPUT 0
#define OFF 0;
#define ON 1;
#define OFF_E 1 //external LEDS
#define ON_E 0  //external LEDS


void initLEDs(){
    //TODO: Initialize the appropriate pins to work with the LEDs
    TRISGbits.TRISG12 = OUTPUT;
    TRISGbits.TRISG14 = OUTPUT;
    TRISEbits.TRISE0 = OUTPUT;
    ODCGbits.ODCG12 = 1;
    ODCGbits.ODCG14 = 1; 
    ODCEbits.ODCE0 = 1;
}
void turnOnLED(int led){
    
    //TODO: You may choose to write this function
    // as a matter of convenience
    if( led == 1 ) //001
    {   
        LATGbits.LATG14 = OFF_E;
        LATEbits.LATE0 = OFF_E;
        LATGbits.LATG12 = ON_E;
    }else if( led == 2 ) //010
    {
        LATGbits.LATG12 = OFF_E;
        LATEbits.LATE0 = ON_E;
        LATGbits.LATG14 = OFF_E;
        
    }else if( led == 3 ) //011
    {
        LATGbits.LATG14 = OFF_E;
        LATEbits.LATE0 = ON_E;
        LATGbits.LATG12 = ON_E;
        
    }else if( led == 4 ) //100
    {  
        LATGbits.LATG14 = ON_E;
        LATEbits.LATE0 = OFF_E;
        LATGbits.LATG12 = OFF_E;
        
    }else if( led == 5 ) //101
    {  
        LATGbits.LATG14 = ON_E;
        LATEbits.LATE0 = OFF_E;
        LATGbits.LATG12 = ON_E;
    }else if(led == 6) //110
    {
        LATGbits.LATG14 = ON_E;
        LATEbits.LATE0 = ON_E;
        LATGbits.LATG12 = OFF_E;  
    }else if(led == 7) //111
    {
        LATGbits.LATG14 = ON_E;
        LATEbits.LATE0 = ON_E;
        LATGbits.LATG12 = ON_E;  
    }else
    {
        LATGbits.LATG12 = OFF_E;
        LATGbits.LATG14 = OFF_E;
        LATEbits.LATE0 = OFF_E;
    }
}