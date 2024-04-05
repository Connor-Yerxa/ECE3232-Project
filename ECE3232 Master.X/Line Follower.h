#pragma config WDTE = OFF
#pragma config  FEXTOSC = ECH 
#pragma config RSTOSC = HFINT32 
#pragma config CLKOUTEN = ON 
#pragma config CSWEN = ON 
#pragma config FCMEN = ON 

#define _XTAL_FREQ 400000

volatile unsigned int y_vec, x_vec;
volatile unsigned char auto_switch, starting=1;

unsigned char lpin=0b1011, mpin=0b1001, rpin=0b1010;

volatile unsigned int line_colour=0x8EC0;
volatile unsigned int fast=100, slow=50, speed_inc=25, speed_dec=25;

void ADC_init()
{
	TRISAbits.TRISA1 = 0;
	ANSELAbits.ANSA0 = 1;
    
	ADCON0 = 0b10010001;
	ADCLK = 0xFF;	//Clock divisions
	ADREF = 0;
    ADPRE = 0;
	ADACQ = 0x3F;	//Measuring speed
    
    ADPCH = 0b1011;
}

int read_adc(char sensor)
{
	volatile unsigned int ADC_val;
    
    ADPCH = sensor;
	ADCON0bits.ADGO = 1;
    __delay_ms(100);
    
	while(ADCON0bits.ADGO){}
    ADC_val = ADRESH << 8;
    ADC_val += ADRESL;
	return ADC_val;
}

void line_follower()
{
    volatile unsigned int l_val, m_val, r_val;
    
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;

    ADCON0bits.ADGO = 1;
    while(ADCON0bits.ADGO){}
    l_val = read_adc(lpin);
    ADCON0bits.ADGO = 1;
    while(ADCON0bits.ADGO){}
    m_val = read_adc(mpin);
    ADCON0bits.ADGO = 1;
    while(ADCON0bits.ADGO){}
    r_val = read_adc(rpin);

    if(m_val < line_colour)
    {
			y_vec = fast;
        LATAbits.LATA1 = 1;
    }
    else
    {
			y_vec = slow;
        LATAbits.LATA1 = 0;
    }

    if(l_val < line_colour)
    {
			y_vec -= speed_inc;
        LATAbits.LATA2 = 1;
    }
    else {LATAbits.LATA2 = 0;}

    if(r_val < line_colour)
    {
			y_vec += speed_inc;
        LATAbits.LATA0 = 1;
    }
    else {LATAbits.LATA0 = 0;}

//    Vdrive(y_vec, x_vec);
    PIR1bits.ADIF = 0;
    ADCON0bits.ADGO = 1;
}