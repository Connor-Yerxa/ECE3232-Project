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

volatile unsigned char tx_wait()
{
    volatile unsigned int count;
    for (count=1000; count > 0; count--)
    {
        if (PIR3bits.TXIF)
        {
            return 1;
        }
    }
    return 0;
}

volatile unsigned char rx_wait()
{
    volatile unsigned int count;
    for (count=1000; count > 0; count--)
    {
        if (PIR3bits.RCIF)
        {
            return 1;
        }
    }
    return 0;
}

volatile unsigned char UART_send(volatile unsigned char message[], unsigned char size)
{
    volatile unsigned char i;
    
    for (i=0; i < size; i++)
    {
        if(!tx_wait())
        {
            return 0;
        }
        TXREG = message[i];
    }
    
    return 1;
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
void PCLS_info_send()
{
    while (!UART_send((volatile unsigned char []){0xFE, 0x19, 0x01, 0x04, 0x00, 0x00}, 6)) {}
}

volatile unsigned char PCLS_info_receive()
{
    volatile char s=12, j, r[12];
    
    for (j=0; j < s; j++)
    {
        if (rx_wait())
        {
            r[j] = RCREG;
        }
        else
        {
            return 0;
        }
    }
    
    teamID = r[6];
    playerID = r[7];
    health = r[8];
    health += r[9] << 8;
    shield_flag = r[10];
    repair_flag = r[11];
    return 1;
}

void PCLS_info()
{
    PCLS_info_send();
    while(!PCLS_info_receive())
    {
        PCLS_info_send();
    }
}



//User Data
volatile unsigned int joy_rx, joy_ry, joy_ly, joy_lx;
volatile unsigned int swA, swB, swC, swD, potA, potB;
void user_data_send()
{
    while(!UART_send((volatile unsigned char []) {0xFE, 0x19, 0x01, 0x05, 0x00, 0x00}, 6)){}
}

volatile unsigned char user_data_receive()
{
    volatile char s=26, j, r[26];
    
    for (j=0; j < s; j++)
    {
        if (rx_wait())
        {
            r[j] = RCREG;
        }
        else
        {
            return 0;
        }
    }
        
    joy_rx = r[6];
    joy_rx += r[7] << 8;
    joy_rx -= 1000;
    joy_rx /= 10;
    
    joy_ry = r[8];
    joy_ry += r[9] << 8;
    joy_ry -= 1000;
    joy_ry /= 10;

    joy_ly = r[10];
    joy_ly += r[11] << 8;
    joy_ly -= 1000;
    joy_ly /= 10;
    
    joy_lx = r[12];
    joy_lx += r[13] << 8;
    joy_lx -= 1000;
    joy_lx /= 10;

    swA = r[14];
    swA += r[15] << 8;
    swB = r[16];
    swB += r[17] << 8;
    swC = r[18];
    swC += r[19] << 8;
    swD = r[20];
    swD += r[21] << 8;

    potA = r[22];
    potA += r[23] << 8;
    potB = r[24];
    potB += r[25] << 8;
    return 1;
}

void user_data()
{
    user_data_send();
    while(!user_data_receive())
    {
        user_data_send();
    }
}


void motor(unsigned char dirA, unsigned char pwmA, unsigned char dirB, unsigned char pwmB)
{
    while (!UART_send((volatile unsigned char []) {0xFE, 0x19, 0x01, 0x06, 0x04, 0x00, dirA, pwmA, dirB, pwmB}, 10)){}
}

void main(void) {
    UART_init();
    PCLS_info();
    while(1)
    {
        user_data();
        motor(1, joy_ly, 1, 1);
    }
    return;
}
