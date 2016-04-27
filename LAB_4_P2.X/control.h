/* 
 * File:   control.h
 * Author: Andres
 *
 * Created on April 13, 2016, 2:51 PM
 */

#ifndef CONTROL_H
#define	CONTROL_H

#include "pwm.h"  

#define MAX 1.00
#define FAST 0.90
#define CRUISE 0.70
#define SLOW 0.55

#define OFF_LINE 10         //000
#define OFF_LEFT 11         //001
#define ON_LINE 12          //010
#define ALMOST_OFF_LEFT 13  //011
#define OFF_RIGHT 14        //100
#define UNDEFINED 15        //101
#define ALMOST_OFF_RIGHT 16 //110
#define ALL_ON 17           //111

int assessLinePosition(unsigned int scanResult);

//This function controls the direction and speed of the robot 
//and the duration for which this command will be executed.
//Duration is in seconds and should typically be between 0.050
//and 0.500 seconds
void driveCommand(int command, float throttle, float duration, int off_cnt, int on_cnt);

void dLoopProtocol(int command, float throttle, int off_cnt, int on_cnt, int turn_cnt);

void turnCommand(int rotation, float throttle, int degrees);

#endif	/* CONTROL_H */

