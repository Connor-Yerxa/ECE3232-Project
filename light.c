/*
 * File:   light.c
 * Author: cbyer
 *
 * Created on February 12, 2024, 7:17 PM
 */


#include <xc.h> 

void init_uart(void)
{
 SPBRG = 0x19; // 9600 baud @ 4 MHz
 TXEN = 1; // enable transmitter
 BRGH = 1; // select high baud rate
 SPEN = 1; // enable serial port
 CREN = 1; // enable continuous operation
}

void main(void)
{
    TRISCbits.TRISC7 = 1;
    TRISAbits.TRISA1 = 0;
    ANSELAbits.ANSA0 = 1;
    
    ADCON0 = 0b10010001;
    ADCLK = 0x00;
    ADREF = 0;
    ADPRE = 0;
    ADACQ = 1;
    ADPCH = 0;
    
    // init_uart();
    
    while(1)
    {
        while(ADCON0bits.ADGO == 1){}
        if (ADRESL > 128)
        {
            LATAbits.LATA1 = 1;
        }
        else
        {
            LATAbits.LATA1 = 0;
        }
        ADCON0bits.ADGO = 1;
    }
    return;
}