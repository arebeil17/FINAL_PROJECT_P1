/* 
 * File:   timer.h
 * Authors: Andres D. Rebeil
 *
 * Created on Feb 16, 2016
 */

#ifndef INITTIMER_H
#define	INITTIMER_H

//Used for long delays in seconds
void initTimer1();

//User for short delays micro and milli-seconds
void initTimer2();

//Delay of sec. >=0 per call multiples of 5
void delaySec(float delay);

//Maximum of 50 ms per call
void delayMs(unsigned int delay);

//Maximum of 50 ms per call
void delayUs(unsigned int delay);

void startTimer();

int stopTimer();


#endif	/* INITTIMER_H */

