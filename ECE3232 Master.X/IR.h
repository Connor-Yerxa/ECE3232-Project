#pragma config WDTE = OFF
#pragma config  FEXTOSC = ECH 
#pragma config RSTOSC = HFINT32 
#pragma config CLKOUTEN = ON 
#pragma config CSWEN = ON 
#pragma config FCMEN = ON 

#define _XTAL_FREQ 400000

volatile unsigned int IR_base=0xB200;

void IR_init()
{
    IR_base = read_adc(0b1100) - 0x40;
}

void IR()
{
    volatile unsigned int ADC_val;
    
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA3 = 0;
    
    ADC_val = read_adc(0b1100);
    if (ADC_val > IR_base)
    {
        LATAbits.LATA1 = 1;
        LATAbits.LATA1 = 1;
        LATAbits.LATA2 = 1;
        LATAbits.LATA3 = 1;
    }
    else
    {
        LATAbits.LATA0 = 0;
        LATAbits.LATA1 = 0;
        LATAbits.LATA2 = 0;
        LATAbits.LATA3 = 0;
        __delay_ms(3000);
    }
}