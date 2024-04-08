/*
 * File:   Master.c
 * Author: cbyer
 *
 * Created on April 4, 2024, 5:03 PM
 */


#include <xc.h>
#include <pic16f18855.h>
#include "Commands.h"
#include "Driving.h"
#include "Line Follower.h"
#include "IR.h"
#include "SheildCode.h"


#pragma config WDTE = OFF
#pragma config  FEXTOSC = ECH 
#pragma config RSTOSC = HFINT32 
#pragma config CLKOUTEN = ON 
#pragma config CSWEN = ON 
#pragma config FCMEN = ON 

#define _XTAL_FREQ 400000
volatile unsigned char edge;
volatile unsigned char serA;

void servos()
{
    int i, j;
    TRISCbits.TRISC3 = 0;     //Claw Move
    TRISCbits.TRISC4 = 0;     //Claw Direction
    
    TRISCbits.TRISC5 = 0;     //Arm Move
    TRISCbits.TRISC0 = 0;     //Arm Direction
    
    TRISAbits.TRISA7 = 0;     //Arduino reset
    
    user_data();
    
    if(potA < 25)
    {
        LATCbits.LATC3 = 1;
        LATCbits.LATC4 = 0;
        i=0;
    }
    else {if(potA > 75)
    {
        LATCbits.LATC3 = 1;
        LATCbits.LATC4 = 1;
        i=0;
    }
    else
    {
        LATCbits.LATC3 = 0;
        i=1;
    }}
    
    if(potB < 25)
    {
        LATCbits.LATC5 = 1;
        LATCbits.LATC0 = 0;
        j=0;
    }
    else {if(potB > 75)
    {
        LATCbits.LATC5 = 1;
        LATCbits.LATC0 = 1;
        j=0;
    }
    else
    {
        LATCbits.LATC5 = 0;
        j=1;
    }}
    
    if(i && j){LATAbits.LATA7=0;}
    else{LATAbits.LATA7=1;}
}

void claw()
{
    user_data();
    
    servo(0, 0, 0, potA);
}



void main(void) {
    UART_init();
    ADC_init();
    user_data();
    while(1){
    user_data();
    
    if(swB)
    {
        line_colour = read_adc(mpin) - 0x100;
        while(swB)
        {
            line_follower();
            user_data();
        }
    }
        if(!swD){
        user_data();
        drive();
        if(swA ==1 && swC ==0){
            set_laser(1);
            __delay_ms(6000);
            set_laser(0);
            __delay_ms(6000);
            ore_type(1);
            __delay_ms(6000);
            set_laser(0);
        }
        else {if(swA==1 && swC==1){
            ore_type(2);
             __delay_ms(6000);
             edge =3;
        }
        }
    }
    if(swC==1 &&swD){
        shield_code();
    }
   while(swC == 2)
    {
        IR_init();
        drive();
        if(swA&& !swD){
            ore_type(3);
            __delay_ms(6000);
        }
       
        while(swD) {IR(); user_data();
        if(swA){
            IR_zone();
            __delay_ms(6000);
        }
        }
    }

    //shield_code();
    
//    if(swC == 1)
//    {
//        user_data();
//        serA = (potA-1000)/10;
//        serA = serA * 2.5;
//        __delay_ms(3000);
//        servo(0,0,0,0);
//        __delay_ms(3000);
//    }
    
    servos();
    
    drive();
}
}