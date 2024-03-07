/*
 * File:   uart.c
 * Author: cbyer
 *
 * Created on March 6, 2024, 7:43 PM
 */


#include <xc.h>

#pragma config  FEXTOSC = ECH 
#pragma config RSTOSC = HFINT32 
#pragma config CLKOUTEN = ON 
#pragma config CSWEN = ON 
#pragma config FCMEN = ON 

volatile unsigned char read[12];

void main(void) {
    TRISAbits.TRISA3 = 0;
    TRISAbits.TRISA5 = 1;
    ANSELAbits.ANSA5 = 0;
    
    ANSELCbits.ANSC6 = 0;
    ANSELCbits.ANSC7 = 0;
    
    TX1STAbits.SYNC = 0;
    TX1STAbits.TXEN = 1;
    RC1STAbits.SPEN = 1;
    RC1STAbits.CREN = 1;
    
    TX1STAbits.BRGH = 1;
    
    BAUD1CONbits.BRG16 = 1;
    SP1BRGLbits.SP1BRGL = 0x44;
    SP1BRGHbits.SP1BRGH = 0x00;
    
//    RXPPS = 0x17;
//    TXPPS = 0x16;
    
    RC6PPS = 0x10;
    
    
    while(1)
    {
        read[1] = 0;
        read[0] = 0;
        if(PORTAbits.RA5 == 0)
        {
            LATAbits.LATA3 = 1;
        }
        else
        {
            LATAbits.LATA3 = 0;
        }
        
        while(!PIR3bits.TXIF){}
        TXREG = 0xFE;
        while(!PIR3bits.TXIF){}
        TXREG = 0x19;
        while(!PIR3bits.TXIF){}
        TXREG = 0x01;
        while(!PIR3bits.TXIF){}
        TXREG = 0x04;
        while(!PIR3bits.TXIF){}
        TXREG = 0x00;
        while(!PIR3bits.TXIF){}
        TXREG = 0x00;
        
        while(!PIR3bits.RCIF){}
        read[0] = RCREG;
        while(!PIR3bits.RCIF){}
        read[1] = RCREG;
        while(!PIR3bits.RCIF){}
        read[2] = RCREG;
        while(!PIR3bits.RCIF){}
        read[3] = RCREG;
        while(!PIR3bits.RCIF){}
        read[4] = RCREG;
        while(!PIR3bits.RCIF){}
        read[5] = RCREG;
        while(!PIR3bits.RCIF){}
        read[6] = RCREG;
        while(!PIR3bits.RCIF){}
        read[7] = RCREG;
        while(!PIR3bits.RCIF){}
        read[8] = RCREG;
        while(!PIR3bits.RCIF){}
        read[9] = RCREG;
        while(!PIR3bits.RCIF){}
        read[10] = RCREG;
        while(!PIR3bits.RCIF){}
        read[11] = RCREG;
        
    }
    return;
}
