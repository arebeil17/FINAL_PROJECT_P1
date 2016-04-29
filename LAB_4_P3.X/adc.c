/* 
 * File:   adc.c
 * Author: Andres
 *
 * Created on March 9, 2016, 10:19 PM
 */

#include "adc.h"


void initADC(){
    
    TRIS_RB15_POT = INPUT; //WIPER SET AS INPUT FOR PIN RB15
//    S3_AN5_RB5 = INPUT;
    S2_AN3_RB3 = INPUT;
    S1_AN1_RB1 = INPUT;
    
    TRIS_RA7_VRN = INPUT;
    TRIS_RA6_VRP = INPUT;
    
    ANSELA = 0x0060; //set ALL A to digital except for RA6 and RA7
    ANSELB = 0x0000; //set all B to digital except for RB5,RB3,RB1
    ANSELBbits.ANSB15 = 1;
    ANSELBbits.ANSB3 = 1;
    ANSELBbits.ANSB1 = 1;
    
    AD1CON1bits.FORM = 0; // 16 unsigned integer
    AD1CON1bits.SSRC = 7; // Auto-convert mode
    AD1CON1bits.ASAM = 1; // Auto-sampling
    AD1CON2bits.VCFG = 0b010; // Use internal VREF+ and external VREF- reference
    AD1CON2bits.CSCNA = 1; // enable scanning
    AD1CON2bits.SMPI = 14; // Take 15 samples
    AD1CON2bits.ALTS = 0; // Only Mux A
    AD1CON3bits.ADRC = 0; // Use PBCLK
    AD1CON3bits.SAMC = 2; // 2 Tad per sample
    AD1CON3bits.ADCS = 0x01; // 4 times the PBCLK Tpb*2*(ADCS + 1) = Tad
    AD1CHSbits.CH0NA = 0; // Use Vref- as negative reference
    //AD1CHSbits.CH0SA = 0b11100; // positive input IVref
    AD1CSSLbits.CSSL15 = 1; // Select AN5 for input SCAN
    AD1CSSLbits.CSSL3 = 1; // Select AN3 for input SCAN
    AD1CSSLbits.CSSL1 = 1; // Select AN1 for input SCAN
    
    IFS0bits.AD1IF = 0; // Put down ADC flag
    //IPC5bits.AD1IP = 7;
    //IEC0bits.AD1IE = 1;
    AD1CON1bits.ADON = 1; // turn on the ADC
}

