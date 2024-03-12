/*
 * File:   Commands.c
 * Author: cbyer
 *
 * Created on March 7, 2024, 7:21 PM
 */

#include <xc.h>

#pragma config  FEXTOSC = ECH 
#pragma config RSTOSC = HFINT32 
#pragma config CLKOUTEN = ON 
#pragma config CSWEN = ON 
#pragma config FCMEN = ON 

char fault_check(unsigned char expected_command)
{
    volatile unsigned char x, y, z;
    
    while(!PIR3bits.RCIF){}
    z = RCREG;
    if(z == 0xFE)
    {
        while(!PIR3bits.RCIF){}
        z = RCREG;
        if(z == 0x19)
        {
            while(!PIR3bits.RCIF){}
            z = RCREG;
            if(z == 0x02)
            {
                while(!PIR3bits.RCIF){}
                z = RCREG;
                if(z == expected_command)
                {
                    
                }
            }
        }
    }
}

void UART_init()
{
    ANSELCbits.ANSC6 = 0;           //Set Tx to Digital
    ANSELCbits.ANSC7 = 0;           //Set Rx to Digital
    
    TX1STAbits.SYNC = 0;            //Set EUSART to asyncronous
    TX1STAbits.TXEN = 1;            //Transmit enabled
    RC1STAbits.SPEN = 1;            //Serial port enabled
    RC1STAbits.CREN = 1;            //Enables continuous receive until enable bit CREN is cleared
    
    TX1STAbits.BRGH = 1;            //High Baud Rate 
    
    BAUD1CONbits.BRG16 = 1;         //Use 16 bits for baurd rate
    SP1BRGLbits.SP1BRGL = 0x44;     //Baud Rate
    
    RC6PPS = 0x10;                  //Set Pin as Tx
}

//PCLS Info
volatile unsigned char teamID, playerID, shield_flag, repair_flag;
volatile unsigned int health;
void PCLS_info()
{
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

    //Sync bits
    while(!PIR3bits.RCIF){}
    teamID = RCREG;
    while(!PIR3bits.RCIF){}
    teamID = RCREG;
    while(!PIR3bits.RCIF){}
    teamID = RCREG;
    while(!PIR3bits.RCIF){}
    teamID = RCREG;
    while(!PIR3bits.RCIF){}
    teamID = RCREG;
    while(!PIR3bits.RCIF){}
    teamID = RCREG;

    //Info bits
    while(!PIR3bits.RCIF){}
    teamID = RCREG;
    while(!PIR3bits.RCIF){}
    playerID = RCREG;
    while(!PIR3bits.RCIF){}
    health = RCREG;
    while(!PIR3bits.RCIF){}
    health += RCREG << 8;
    while(!PIR3bits.RCIF){}
    shield_flag = RCREG;
    while(!PIR3bits.RCIF){}
    repair_flag = RCREG;
}

//User Data
volatile unsigned int joy_rx, joy_ry, joy_ly, joy_lx;
volatile unsigned int swA, swB, swC, swD, potA, potB;
void user_data()
{
    while(!PIR3bits.TXIF){}
    TXREG = 0xFE;
    while(!PIR3bits.TXIF){}
    TXREG = 0x19;
    while(!PIR3bits.TXIF){}
    TXREG = 0x01;
    while(!PIR3bits.TXIF){}
    TXREG = 0x05;
    while(!PIR3bits.TXIF){}
    TXREG = 0x00;
    while(!PIR3bits.TXIF){}
    TXREG = 0x00;
    
    while(!PIR3bits.RCIF){}
    joy_rx = RCREG;
    while(!PIR3bits.RCIF){}
    joy_rx = RCREG;
    while(!PIR3bits.RCIF){}
    joy_rx = RCREG;
    while(!PIR3bits.RCIF){}
    joy_rx = RCREG;
    while(!PIR3bits.RCIF){}
    joy_rx = RCREG;
    while(!PIR3bits.RCIF){}
    joy_rx = RCREG;
    
    while(!PIR3bits.RCIF){}
    joy_rx = RCREG;
    while(!PIR3bits.RCIF){}
    joy_rx += RCREG << 8;
    while(!PIR3bits.RCIF){}
    joy_ry = RCREG;
    while(!PIR3bits.RCIF){}
    joy_ry += RCREG << 8;
    while(!PIR3bits.RCIF){}
    
    joy_ly = RCREG;
    while(!PIR3bits.RCIF){}
    joy_ly += RCREG << 8;
    while(!PIR3bits.RCIF){}
    joy_lx = RCREG;
    while(!PIR3bits.RCIF){}
    joy_lx += RCREG << 8;
    
    while(!PIR3bits.RCIF){}
    swA = RCREG;
    while(!PIR3bits.RCIF){}
    swA += RCREG << 8;
    while(!PIR3bits.RCIF){}
    swB = RCREG;
    while(!PIR3bits.RCIF){}
    swB += RCREG << 8;
    while(!PIR3bits.RCIF){}
    swC = RCREG;
    while(!PIR3bits.RCIF){}
    swC += RCREG << 8;
    while(!PIR3bits.RCIF){}
    swD = RCREG;
    while(!PIR3bits.RCIF){}
    swD += RCREG << 8;
    
    while(!PIR3bits.RCIF){}
    potA = RCREG;
    while(!PIR3bits.RCIF){}
    potA += RCREG << 8;
    while(!PIR3bits.RCIF){}
    potB = RCREG;
    while(!PIR3bits.RCIF){}
    potB += RCREG << 8;
}

void motor(unsigned char dirA, unsigned char pwmA, unsigned char dirB, unsigned char pwmB)
{
    while(!PIR3bits.TXIF){}
    TXREG = 0xFE;
    while(!PIR3bits.TXIF){}
    TXREG = 0x19;
    while(!PIR3bits.TXIF){}
    TXREG = 0x01;
    while(!PIR3bits.TXIF){}
    TXREG = 0x06;
    while(!PIR3bits.TXIF){}
    TXREG = 0x04;
    while(!PIR3bits.TXIF){}
    TXREG = 0x00;
    
    while(!PIR3bits.TXIF){}
    TXREG = dirA;
    while(!PIR3bits.TXIF){}
    TXREG = pwmA;
    while(!PIR3bits.TXIF){}
    TXREG = dirB;
    while(!PIR3bits.TXIF){}
    TXREG = pwmB;
}

void main(void) {
    UART_init();
    
    PCLS_info();
    return;
}
