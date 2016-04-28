/* 
 * File:   control.c
 * Author: Andres
 *
 * Created on April 13, 2016, 2:23 PM
 */

#include "control.h"
#include "sensor.h"
#include <xc.h>
#include "leds.h"

#define GOOD 1
#define FAIL 0
#define LEFT_S1 1
#define MIDDLE_S2 2
#define RIGHT_S3 3

int assessLinePosition(unsigned int scanResult){
    
    int command = IDLE;
    
    if(scanResult == 0b000) command = OFF_LINE;
    else if(scanResult == 0b001) command = OFF_LEFT;
    else if(scanResult == 0b010) command = ON_LINE;
    else if(scanResult == 0b011) command = ALMOST_OFF_LEFT;
    else if(scanResult == 0b100) command = OFF_RIGHT;
    else if(scanResult == 0b110) command = ALMOST_OFF_RIGHT;
    else if(scanResult == 0b111) command = ALL_ON;
    else command = UNDEFINED;
    
    return command;
}

void driveCommand(int command, float throttle, float duration, int off_cnt, int on_cnt){
    
    int direction = IDLE;
    float turnDelay = 0.600;
    int iterations = 0;
    int result = 7; //0b111
    
    if(duration > 0){
        if((on_cnt < 55) && (off_cnt < 100)){
            if(command == OFF_LEFT) direction = FLIP_CW; //WAS RIGHT
            else if(command == ON_LINE) direction = FORWARD;
            else if(command == ALMOST_OFF_LEFT) direction = FORWARD;  //RIGHT
            else if(command == OFF_RIGHT) direction = FLIP_CCW;//WAS LEFT
            else if(command == ALMOST_OFF_RIGHT) direction = FORWARD; //LEFT
            else if(command == ALL_ON) direction = FORWARD;
            else if(command == UNDEFINED) direction = FORWARD;
            else direction = IDLE;
            //Executes specified drive command
            updatePWM(throttle, SYNC, direction);
            delaySec(duration);
    }else if(off_cnt == 100) {
        while(result != 2){ //0b010
            result = scanLineSensors(result, 0);
            turnOnLED(result);
            if(iterations == 0) {
                updatePWM(SLOW, SYNC, FLIP_CW);
                delaySec(0.550);
            }
            iterations++; delayMs(1);
            if(iterations > 100) break;
       }
            updatePWM(CRUISE, SYNC, IDLE);
    }else if(on_cnt == 55){
        
        while(result != 2){ //0b010
            result = scanLineSensors(result, 0);
            turnOnLED(result);
            if(iterations == 0) {
                updatePWM(SLOW, SYNC, FLIP_CW);
                delaySec(0.550);
            }
            iterations++; delayMs(1);
            if(iterations > 100) break;
       }
            updatePWM(CRUISE, SYNC, IDLE);
    }
        
    }
}

int avoidanceProtocol(int sonarResult, int command, int INITIAL_DETECTION){
    
    int newResult = 0;
    int initScenario = BLOCKED_FRONT;
    int resolve = BLOCKED_FRONT;
    int currentDist = 0;  //current distance
    
    if(INITIAL_DETECTION){
        
        if((sonarResult == BLOCKED_FAR) || (sonarResult == BLOCKED_FRONT)) 
        {   command = FLIP_CCW; 
            resolve = BLOCKED_RIGHT;
        }else if(sonarResult == BLOCKED_LAF) {
            command = FLIP_CW;
            resolve = BLOCKED_LEFT;
        }else return FAIL;
        
        while(newResult != resolve){
            updatePWM(SLOW, SYNC, command);
            delayMs(1);
            newResult = sonarSweep(1);
        }
        updatePWM(SLOW, SYNC, IDLE);
        return GOOD;
    }else{
        if(sonarResult == BLOCKED_RIGHT){
            currentDist = getDistance(RIGHT_S3);
            if( currentDist <= (CRITICAL_RIGHT - BUFFER_REGION)) command = LEFT;
            else if(currentDist >= (CRITICAL_RIGHT + BUFFER_REGION)) command = RIGHT;
            else command = FORWARD;
            
        }else if(sonarResult == BLOCKED_LEFT){
            currentDist = getDistance(LEFT_S1);
            if( currentDist <= (CRITICAL_LEFT - BUFFER_REGION)) command = RIGHT;
            else if(currentDist >= (CRITICAL_LEFT + BUFFER_REGION)) command = LEFT;
            else command = FORWARD;
        }else{
            
        }
        
    }
    
}




