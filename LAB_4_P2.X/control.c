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
            result = scanLineSensors(result, 0, 0);
            turnOnLED(result);
            if(iterations == 0) {
                updatePWM(SLOW, SYNC, FLIP_CW);
                delaySec(0.550);
            }
            iterations++;
       }
            updatePWM(CRUISE, SYNC, IDLE);
    }else if(on_cnt == 55){
        
        while(result != 2){ //0b010
            result = scanLineSensors(result, 0, 0);
            turnOnLED(result);
            if(iterations == 0) {
                updatePWM(SLOW, SYNC, FLIP_CW);
                delaySec(0.550);
            }
            iterations++;
       }
            updatePWM(CRUISE, SYNC, IDLE);
    }
        
    }
}

void dLoopProtocol(int command, float throttle, int off_cnt, int on_cnt, int turn_cnt){
    
    int direction = IDLE;
    float turnDelay = 0.600;
    int iterations = 0;
    int result = 7; //0b111
    
    if((on_cnt < 15) && (off_cnt < 75)){
            if(command == OFF_LEFT) direction = FLIP_CW; //WAS RIGHT
            else if(command == ON_LINE) direction = FORWARD;
            else if(command == ALMOST_OFF_LEFT) direction = FLIP_CW;  //RIGHT
            else if(command == OFF_RIGHT) direction = FLIP_CCW;//WAS LEFT
            else if(command == ALMOST_OFF_RIGHT) direction = FLIP_CCW; //LEFT
            else if(command == ALL_ON) direction = FORWARD;
            else if(command == UNDEFINED) direction = FORWARD;
            else direction = IDLE;

            //Executes specified drive command
            updatePWM(throttle, SYNC, direction);
            delaySec(0.001);
    }else if(off_cnt == 75) {
        
//        if(turn_cnt == 2) { 
//            updatePWM(CRUISE, SYNC, FORWARD); delaySec(0.100);
//            direction = FLIP_CCW;
//        }else if(turn_cnt <= 3) direction = FLIP_CCW;
//        else if(turn_cnt <= 5) direction = FLIP_CW;
//        else direction = FLIP_CCW;

        while(result != 2){ //0b010
            result = scanLineSensors(result, 1, turn_cnt);
            turnOnLED(result);
            if(iterations == 0) {
                updatePWM(SLOW, SYNC, FLIP_CW);
                delaySec(0.550);
            }
            iterations++;
       }
            updatePWM(CRUISE, SYNC, IDLE);
    }else if(on_cnt == 15){
        
//        if(turn_cnt == 2) { 
//            updatePWM(CRUISE, SYNC, FORWARD); delaySec(0.250);
//            direction = FLIP_CCW;
//        }else if(turn_cnt <= 3) {
//            updatePWM(CRUISE, SYNC, FORWARD); delaySec(0.250);
//            direction = FLIP_CCW;
//        }else if(turn_cnt <= 5) direction = FLIP_CCW;
//        else direction = FLIP_CCW;
        
        updatePWM(CRUISE, SYNC, FORWARD); delaySec(0.250);
        direction = FLIP_CCW;
        
        while(result != 2){ //0b010
            result = scanLineSensors(result, 1, turn_cnt);
            turnOnLED(result);
            if(iterations == 0) {
                updatePWM(SLOW, SYNC, direction);
                delaySec(0.550);
            }
            iterations++;
       }
            updatePWM(CRUISE, SYNC, IDLE);
    }
    
}

void turnCommand(int rotation, float throttle, int degrees){
    
//    float duration = 0.05*degrees; //Each degree corresponds to 50ms
//                                   //of drive command duration
//    
//    if(rotation == LEFT){
//        driveCommand(LEFT, CRUISE, duration, 0);
//    }else if(rotation == RIGHT){
//        driveCommand(RIGHT, CRUISE, duration, 0);
//    }
}

