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
        if((on_cnt < ON_LIMIT) && (off_cnt < OFF_LIMIT)){
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
    }else if(off_cnt == OFF_LIMIT) {
        if((command == ALMOST_OFF_RIGHT) || (command == OFF_RIGHT)) direction = FLIP_CCW;
        else if((command == ALMOST_OFF_LEFT) || (command == OFF_LEFT)) direction = FLIP_CW;
        
        while(result != 2){ //0b010
            result = scanLineSensors(result, 0);
            turnOnLED(result);
            if(iterations == 0) {
                updatePWM(SLOW, SYNC, FLIP_CW);
            }
            delayMs(1); iterations++;
            if(iterations > 2000) break;
       }
            updatePWM(CRUISE, SYNC, IDLE);
    }else if(on_cnt == ON_LIMIT){
        
        while(result != 2){ //0b010
            result = scanLineSensors(result, 0);
            turnOnLED(result);
            if(iterations == 0) {
                updatePWM(SLOW, SYNC, FLIP_CW);
            }
            delayMs(1); iterations++;
            if(iterations > 2000) break;
       }
            updatePWM(CRUISE, SYNC, IDLE);
    }
        
    }
}

int avoidanceProtocol(int sonarResult, int position, int INITIAL_DETECTION, int initResult){
    
    int newResult = 0;
    int initScenario = BLOCKED_FRONT;
    int resolve = BLOCKED_FRONT;
    int currentDist = 0;  //current distance
    int command = IDLE;
    int newPosition = 7; //0b111
    int iterations = 0;
    
    if(INITIAL_DETECTION){               //PERFORM INITIAL DETECTION ROTATION
        
        if((sonarResult == BLOCKED_FAR) || (sonarResult == BLOCKED_FRONT)) 
        {   command = FLIP_CCW; 
            resolve = BLOCKED_RIGHT;
        }else if(sonarResult == BLOCKED_LAF) {
            command = FLIP_CW;
            resolve = BLOCKED_LEFT;
        }else return FAIL;
        
        while(newResult != resolve){          //ROTATE UNTIL SIDE SENSOR DETECTS
            updatePWM(SLOW, SYNC, command);   //OBJECT OR IF MAXIMUM TIME
            delayMs(1); iterations++;         //EXCEEDED
            newResult = sonarSweep(1);
            if(iterations >= 1500) break;
        }
        updatePWM(SLOW, SYNC, IDLE);          //STOP
        
        if(iterations < 1500) return GOOD;   //RETURN GOOD IF TURN ENDED WELL
        else return FAIL;                    //ELSE RETURN FAIL STATUS
        
    }else if(position != OFF_LINE){     //CHECK IF BACK ON LINE
        
        if((initResult == BLOCKED_FAR) || (initResult == BLOCKED_FRONT))
        {   command = FLIP_CCW;
        }else command = FLIP_CW;
        
        while(newPosition != 0){                       //MOVE UNTIL OFF LINE
            newPosition = scanLineSensors(newPosition, 0);
            turnOnLED(newPosition);
            updatePWM(CRUISE, SYNC, FORWARD);
            delayMs(1); iterations++;
            if(iterations >= 200) break;
        }
        updatePWM(SLOW, SYNC, IDLE);                    //STOP
        if(iterations >= 200) return FAIL;              //CHECK FAIL CONDITION
        else iterations = 0;                            //ELSE RESET & CONTINUE
                                                        //TO FINAL TURN
        newPosition = scanLineSensors(newPosition, 0);
        
        while(newPosition != 2){ //0b010               //TURN UNTIL BACK ON LINE
            newPosition = scanLineSensors(newPosition, 0);
            turnOnLED(newPosition);
            if(iterations == 0) {
                updatePWM(SLOW, SYNC, command);
            }
            delayMs(1); iterations++;
            if(iterations >= 1500) break;
       }
        updatePWM(SLOW, SYNC, IDLE);                  //STOP
        if(iterations >= 1500) return FAIL;           //CHECK FAIL CONDITION
        else return SUCCESS;                          //ELSE SUCCESS -> READY
                                                      //TO RESUME LINE FOLLOWING
    }else{ //ELSE MAINTAIN OBJECT WITHIN BUFFER REGION
                
        if(sonarResult == BLOCKED_RIGHT){
            currentDist = getDistance(RIGHT_S3);
            if( currentDist <= (CRITICAL_RIGHT - BUFFER_REGION)) command = LEFT;
            else if(currentDist >= (CRITICAL_RIGHT + BUFFER_REGION)) command = RIGHT;
            else command = FORWARD;
            updatePWM(CRUISE, SYNC, command); delayMs(1);
        }else if(sonarResult == BLOCKED_LEFT){
            currentDist = getDistance(LEFT_S1);
            if( currentDist <= (CRITICAL_LEFT - BUFFER_REGION)) command = RIGHT;
            else if(currentDist >= (CRITICAL_LEFT + BUFFER_REGION)) command = LEFT;
            else command = FORWARD;
            updatePWM(CRUISE, SYNC, command); delayMs(1);
        }else{
            //OBJECT WAS LOST ATTEMPT TO FIND AGAIN
            if((initResult == BLOCKED_FAR) || (initResult == BLOCKED_FRONT)){   
                command = FLIP_CW; resolve = BLOCKED_RIGHT;
            }else{
                command = FLIP_CCW; resolve = BLOCKED_LEFT;
            }
             while(newResult != resolve){
                updatePWM(SLOW, SYNC, command);
                delayMs(1); iterations++;
                newResult = sonarSweep(1);
                if(iterations >= 1500) break;
            }
            updatePWM(SLOW, SYNC, IDLE);
        
            if(iterations < 1500) return GOOD; //OBJECT FOUND AGAIN RETURN GOOD
            else return FAIL;                  //OBJECT NOT FOUND RETURN FAIL
        }
        return GOOD; //COMMAND EXECUTED RETURN GOOD STATUS
    }
    
}




