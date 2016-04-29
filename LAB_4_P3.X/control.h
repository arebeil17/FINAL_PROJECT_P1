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
#define CRUISE 0.75        
#define SLOW 0.65

#define OFF_LINE 10         //000
#define OFF_LEFT 11         //001
#define ON_LINE 12          //010
#define ALMOST_OFF_LEFT 13  //011
#define OFF_RIGHT 14        //100
#define UNDEFINED 15        //101
#define ALMOST_OFF_RIGHT 16 //110
#define ALL_ON 17           //111

#define ON_LIMIT 500        // was 65
#define OFF_LIMIT 50

int assessLinePosition(unsigned int scanResult);

//This function controls the direction and speed of the robot 
//and the duration for which this command will be executed.
//Duration is in seconds and should typically be between 0.050
//and 0.500 seconds
void driveCommand(int command, float throttle, float duration, int off_cnt, int on_cnt);


#endif	/* CONTROL_H */

