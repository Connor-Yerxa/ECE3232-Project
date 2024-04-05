/*
 * File:   Master.c
 * Author: cbyer
 *
 * Created on April 4, 2024, 5:03 PM
 */


#include <xc.h>
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

volatile unsigned char serA;

void claw()
{
    user_data();
    
    servo(0, 0, 0, potA);
}



void main(void) {
    UART_init();
    user_data();
    ADC_init();
    
    if(swB)
    {
        line_colour = read_adc(mpin) - 0x100;
        while(swB)
        {
            line_follower();
            user_data();
        }
    }
    
    while(swC == 2)
    {
        IR_init();
        drive();
        while(swD) {IR(); user_data();}
    }
    
    shield_code();
    
//    if(swC == 1)
//    {
//        user_data();
//        serA = (potA-1000)/10;
//        serA = serA * 2.5;
//        __delay_ms(3000);
//        servo(0,0,0,0);
//        __delay_ms(3000);
//    }
    drive();
    return;
}
