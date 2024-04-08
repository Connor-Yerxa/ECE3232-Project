/*
 * COMMANDS:
 * PCLS_info():                             This updates  the PCLS info.
 * user_data():                             This updates the RC values.
 * motor(dirA, pwmA, dirB, pwmB):           This Sets the motor speeds and directions.
 * servo(s1, s2, s3, s4):                   This sets the servo angles.
 * set_laser(en):                           This is to enable the laser scope.
 * shoot_laser(en):                         This sets the shot type. (not needed maybe)
 * transmit_shield_code():
 * request_repair():
 * transmit_repair():
 * ore_type(type):
 * 
 * custom_laser(size_lower, size_upper, message[]):
 * surface_exploration(taskID_l, taskID_u, value_l, value_u):
 */

#include <xc.h>

#pragma config WDTE = OFF
#pragma config  FEXTOSC = ECH 
#pragma config RSTOSC = HFINT32 
#pragma config CLKOUTEN = ON 
#pragma config CSWEN = ON 
#pragma config FCMEN = ON 

#define _XTAL_FREQ 400000

//User Data
volatile unsigned int joy_rx, joy_ry, joy_ly, joy_lx;
volatile unsigned int swA, swB, swC, swD, potA, potB;
//PCLS Info
volatile unsigned char teamID, playerID, shield_flag, repair_flag;
volatile unsigned int health;

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

volatile unsigned char UART_send(volatile unsigned char message[], unsigned int size)
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
    swA = swA > 1500;
    swB = r[16];
    swB += r[17] << 8;
    swB = swB > 1500;
    swC = r[18];
    swC += r[19] << 8;
    if(swC == 1000){swC = 0;}
    else{if(swC == 1500){swC = 1;}
    else {swC = 2;}}
    swD = r[20];
    swD += r[21] << 8;
    swD = swD > 1500;

    potA = r[22];
    potA += r[23] << 8;
    potA = (potA - 1000)/10;
    potB = r[24];
    potB += r[25] << 8;
    potB = (potB - 1000)/10;
    return 1;
}

void user_data()
{
    user_data_send();
    while(!user_data_receive())
    {
        user_data_send();
    }
    __delay_ms(100);
}

void motor(unsigned char dirA, unsigned char pwmA, unsigned char dirB, unsigned char pwmB)
{
    while (!UART_send((volatile unsigned char []) {0xFE, 0x19, 0x01, 0x06, 0x04, 0x00, dirA, pwmA, dirB, pwmB}, 10)){}
}

void servo(unsigned char s1, unsigned char s2, unsigned char s3, unsigned char s4)
{
    while(!UART_send((volatile unsigned char []){0xFE, 0x19, 0x01, 0x07, 0x04, 0x00, s1, s2, s3, s4}, 10)){}
}

void set_laser(unsigned char en)
{
    while(!UART_send((volatile unsigned char []){0xFE, 0x19, 0x01, 0x08, 0x01, 0x00, en}, 7)){}
}

void shoot_laser(unsigned char en)
{
    while(!UART_send((volatile unsigned char []){0xFE, 0x19, 0x01, 0x09, 0x01, 0x00, en}, 7)){}
}

void transmit_shield_code()
{
    while(!UART_send((volatile unsigned char []){0xFE, 0x19, 0x02, 0x09, 0x00, 0x00}, 6)){}
}

void request_repair()
{
    while(!UART_send((volatile unsigned char []){0xFE, 0x19, 0x03, 0x09, 0x00, 0x00}, 6)){}
}

void transmit_repair()
{
    while(!UART_send((volatile unsigned char []){0xFE, 0x19, 0x01, 0x09, 0x00, 0x00}, 6)){}
}

void custom_laser(unsigned char size_lower, unsigned char size_upper, volatile unsigned char message[])
{
    int size=size_lower;
    size += size_upper << 8;
    while(!UART_send((volatile unsigned char []){0xFE, 0x19, 0x81, 0x09, size_lower, size_upper}, 6)){}
    while(!UART_send(message, size)){}
}

void surface_exploration(unsigned char taskID_l, unsigned char taskID_u, unsigned char value_l, unsigned char value_u)
{
    while(!UART_send((volatile unsigned char []){0xFE, 0x19, 0x01, 0x0A, 0x04, 0x00, taskID_l, taskID_u, value_l, value_u}, 10)){}
}

void ore_type(unsigned char type)
{
    while(!UART_send((volatile unsigned char []){0xFE, 0x19, 0x03, 0x0A, 0x01, 0x00, type}, 7)){}
}


//void main(void) {
//    UART_init();
//    PCLS_info();
//    while(1)
//    {
//        user_data();
//        motor(1, joy_ly, 1, 1);
//    }
//    return;
//}


