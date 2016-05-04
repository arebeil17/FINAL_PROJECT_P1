// ******************************************************************************************* //
//
// File:         LAB3_PART2
// Date:         April 25th, 2016
// Authors:      Andres D. Rebeil
//
// Description: 
// ******************************************************************************************* //
#include <xc.h>
#include <sys/attribs.h>
#include "lcd.h"
#include "timer.h"
#include "switch.h"
#include "leds.h"
#include "config.h"
#include "interrupt.h"
#include "adc.h"
#include "sensor.h"
#include "sonar.h"
#include "control.h"
#include "pwm.h"
#include <math.h>

#define pressed 0
#define released 1
#define SW1 PORTAbits.RA7 


typedef enum stateTypeEnum{
    INITIAL, WAIT, WAIT_2, STOP, SCAN, ASSESS, FOLLOW, AVOID
} stateType;

void initialProcess();

//Volatile variable declarations
volatile stateType state = INITIAL;
volatile unsigned int SW1_pressed = 0, SW1_toggle = 0;
volatile unsigned int ADC_Val1 = 0, ADC_Val2 = 0, ADC_Val3 = 0;

// ******************************************************************************************* //

int main(void)
{
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    
    //CALL INITIALIZATION FUNCTION
    initialProcess();
   
    int TEMP = 0, result = 0, sonarResult = CLEAR, command = IDLE;
    int position = OFF_LINE;
    int off_cnt = 0, on_cnt = 0;
    int prev_position = OFF_LINE;
    int steps = 0;
    int BLOCKED = 0, INITIAL_DETECTION = 0;
    int avoid_cnt = 0, status = FAIL, prev_status = FAIL;
    int initSonarResult = CLEAR;

    delaySec(1);
    while(1)
    {   
        switch(state){
            case INITIAL:
                turnOnLED(0);
                delaySec(1);
                state = WAIT;
            break;
            case WAIT:
                turnOnLED(result);
                result = scanLineSensors(result, 1); //Scans ADC and displays sensor data
                if(SW1_toggle){ state = WAIT_2; SW1_toggle = 0; delayMs(10);}
            break;
            case WAIT_2:
                turnOnLED(result);
                result = scanLineSensors(result, 0); //Scans ADC and displays sensor data
                sonarResult = sonarSweep(BLOCKED);
                if(SW1_toggle){ state = SCAN; SW1_toggle = 0; delayMs(10);}
            break;
            case SCAN:
                if((steps == 250) || BLOCKED) {
                    sonarResult = sonarSweep(BLOCKED); steps = 0;}
                turnOnLED(result);
                result = scanLineSensors(result, 0); //Scans ADC and do not display sensor data
                if(SW1_toggle){ state = STOP; SW1_toggle = 0; delayMs(10);}
                else state = ASSESS;
            break;
            case ASSESS:
                position = assessLinePosition(result);
                if((sonarResult != CLEAR) || BLOCKED) {
                    if(!avoid_cnt) {
                        INITIAL_DETECTION = 1; 
                        initSonarResult = sonarResult;
                        updatePWM(SLOW, SYNC, IDLE);
                    }else INITIAL_DETECTION = 0;
                    BLOCKED = 1; avoid_cnt++;
                }else BLOCKED = 0;
                
                if(SW1_toggle){ state = STOP; SW1_toggle = 0; delayMs(10);}
                else if(BLOCKED) state = AVOID;
                else state = FOLLOW;
            break;
            case FOLLOW:
                steps++;
                if(position == OFF_LINE){ 
                    off_cnt++;
                    driveCommand(prev_position, CRUISE, 0.001, off_cnt, on_cnt);
                }else if(position == ALL_ON){
                    on_cnt++;
                    driveCommand(position, CRUISE, 0.001, off_cnt, on_cnt);
                }else { 
                    off_cnt = 0; on_cnt = 0;
                    if(position != ON_LINE) prev_position = position;
                    driveCommand(position, CRUISE, 0.001, off_cnt, on_cnt);
                }
                if(SW1_toggle){ state = STOP; SW1_toggle = 0; delayMs(10);}
                else state = SCAN;
            break;
            case AVOID:
               status = avoidanceProtocol(sonarResult, position, INITIAL_DETECTION, initSonarResult);
               if(status == GOOD) { state = SCAN; BLOCKED = 1; sonarResult = sonarSweep(BLOCKED);}
               else if(status == FAIL) { state = STOP; BLOCKED = 0;}
               else if(status == SUCCESS) 
               { state = SCAN; BLOCKED = 0; steps = 0; sonarResult = CLEAR;}
            break;
            case STOP:
                turnOnLED(0);
                off_cnt = 0; on_cnt = 0; avoid_cnt = 0; INITIAL_DETECTION = 0;
                steps = 0;
                BLOCKED = 0;
                driveCommand(IDLE, SLOW, 0.001, 0, 0);
                clearLCD(); moveCursorLCD(1,1);
                printStringLCD("STOP");
                delaySec(5);
                clearLCD();
                state = WAIT;
            break;
        } 
    }        
    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void){
    //----------------------------------------
    int j;
    PORTA;
    
    j = SW1; //RA7
    
    //CHECK SW2 TOGGLE
    if(SW1 == pressed){
        SW1_pressed = 1;
    }else if((SW1 == released) && (SW1_pressed == 1)){
        SW1_pressed = 0;
        SW1_toggle = 1;
        
    }
    //RESET FLAGS
    IFS1bits.CNAIF = 0; //FLAG DOWN

}

void initialProcess(){
    initLEDs();
    initTimer1();
    initTimer2();
    initLCD();
    delayMs(1);
    initSW();
    initADC();
    initPWM();
    initSonar();
    int i = 0;
    turnOnLED(0);
    for(i = 0; i < 20; i++) delayMs(50);
    turnOnLED(7);
    for(i = 0; i < 20; i++) delayMs(50);
    turnOnLED(0);
    for(i = 0; i < 20; i++) delayMs(50);
    turnOnLED(7);
    clearLCD();
    moveCursorLCD(1,1);
}
