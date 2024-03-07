#include <xc.h>

#pragma config WDTE = OFF

volatile unsigned int left_vec, right_vec, auto_switch;

unsigned char lpin=0b1011, mpin=0b1001, rpin=0b1010;

volatile unsigned int line_colour=0xF600;
volatile unsigned int fast=100, slow=50, speed_inc=25, speed_dec=25;

void ADC_init()
{
	TRISCbits.TRISC7 = 1;
	TRISAbits.TRISA1 = 0;
	ANSELAbits.ANSA0 = 1;
    
	ADCON0 = 0b10010001;
	ADCLK = 0xFF;	//Clock divisions
	ADREF = 0;
	ADACQ = 0x3F;	//Measuring speed
    
    ADPCH = 0b1011;
    
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
}

void init_uart(void)
{
    SPBRG = 0x19; // 9600 baud @ 4 MHz
    TXEN = 1; // enable transmitter
    BRGH = 1; // select high baud rate
    SPEN = 1; // enable serial port
    CREN = 1; // enable continuous operation
}

int read_adc(int sensor)
{
	volatile unsigned int ADC_val;
    
    if (sensor == 1)        //Left (Out1)
    {
        ADPCH = lpin;
    }
    else {if(sensor == 2)   //Right (Out2)
    {
        ADPCH = rpin;
    }
    else {if(sensor == 3)    //Middle (Out3)
    {
        ADPCH = mpin;
    }}}
	ADCON0bits.ADGO = 1;
    
	while(ADCON0bits.ADGO){}
    ADC_val = ADRESH << 8;
    ADC_val += ADRESL;
	return ADC_val;
}

void send_vectors_to_motors() {}


void __interrupt() myinterrupts()
{
	volatile unsigned int l_val, m_val, r_val;

	if(auto_switch)
	{
        ADCON0bits.ADGO = 1;
        while(ADCON0bits.ADGO){}
		l_val = read_adc(1);
        ADCON0bits.ADGO = 1;
        while(ADCON0bits.ADGO){}
		m_val = read_adc(3);
        ADCON0bits.ADGO = 1;
        while(ADCON0bits.ADGO){}
		r_val = read_adc(2);
		
        if(m_val < line_colour)
		{
//			left_vec = fast;
//			right_vec = fast;
            LATAbits.LATA1 = 1;
		}
		else
		{
//			left_vec = slow;
//			right_vec = slow;
            LATAbits.LATA1 = 0;
		}
		
		if(l_val < line_colour)
		{
//			left_vec -= speed_dec;
//			right_vec += speed_inc;
            LATAbits.LATA2 = 1;
		}
        else {LATAbits.LATA2 = 0;}
        
		if(r_val < line_colour)
		{
//			left_vec += speed_inc;
//			right_vec -= speed_dec;
            LATAbits.LATA0 = 1;
		}
        else {LATAbits.LATA0 = 0;}

		send_vectors_to_motors();
        PIR1bits.ADIF = 0;
        ADCON0bits.ADGO = 1;
	}

}

void main()
{
    auto_switch=1;
	ADC_init();
	init_uart();
    
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    
    TRISAbits.TRISA5 = 1;
    ANSELAbits.ANSA5 = 0;
    TRISAbits.TRISA3 = 0;
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    ADCON0bits.ADGO = 1;
    
	while(1)
    {
        if(PORTAbits.RA5 == 0)
        {
            LATAbits.LATA3 = 1;
        }
        else
        {
            LATAbits.LATA3 = 0;
        }
    }
}