/* 
 * File:   sensor.h
 * Author: Andres
 *
 * Created on April 13, 2016, 2:22 PM
 */

#ifndef SENSOR_H
#define	SENSOR_H

#define DETECTION_LIMIT_1 2.30
#define DETECTION_LIMIT_2 2.30
#define DETECTION_LIMIT_3 2.40

int scanLineSensors(int result, int d_mode, int turnCount);

void displayLineSensors(int s1, int s2, int s3, int d_mode, int turnCount);

#endif	/* SENSOR_H */

