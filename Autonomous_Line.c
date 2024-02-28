#include <xc.h>

int auto_pin = ;
volatile unsigned int left_vec, right_vec;

void ADC_init()
{
	TRISCbits.TRISC7 = 1;
	TRISAbits.TRISA1 = 0;
	ANSELAbits.ANSA0 = 1;
    
	ADCON0 = 0b10010001;
	ADCLK = 0x00;	//Clock divisions
	ADREF = 0;
	ADPRE = 0;
	ADACQ = 0x3F;	//Measuring speed
	ADPCH = 0;	//ADC channel
}

void init_uart(void)
{
 SPBRG = 0x19; // 9600 baud @ 4 MHz
 TXEN = 1; // enable transmitter
 BRGH = 1; // select high baud rate
 SPEN = 1; // enable serial port
 CREN = 1; // enable continuous operation
}

int read_adc(sensor)
{
	volatile unsigned int ADC_val;
	ADCON0bits.ADGO = 1;
	while(ADCON0bits.ADGO){}
	ADC_val = ADRESH << 8;
        ADC_val += ADRESL;
	return ADC_val;
}


void __interrupt() myinterrupts()
{
	volatile unsigned int l_val, m_val, r_val;

	while(*auto_switch*)
	{
		
		TRISXbits.L_pin = 0; 		//set to output
		LATXbits.L_pin = 1;		//set pin high
		l_val = read_adc(L_sens_pin);
		TRISXbits.L_pin = 1; 		//set to input

		TRISXbits.M_pin = 0; 		//set to output
		LATXbits.M_pin = 1;		//set pin high
		m_val = read_adc(M_sens_pin);
		TRISXbits.M_pin = 1; 		//set to input

		TRISXbits.R_pin = 0; 		//set to output
		LATXbits.R_pin = 1;		//set pin high
		r_val = read_adc(R_sens_pin);
		TRISXbits.R_pin = 1; 		//set to input
		
		if(m_val == *line colour*)
		{
			left_vec = *Fast*;
			right_vec = *Fast*;
		}
		else
		{
			left_vec = *Slow*;
			right_vec = *Slow*;
		}
		
		if(l_val == *line colour*)
		{
			left_vec -= *speed decreases*;
			right_vec += *speed increases*;
		}
		if(r_val == *line colour*)
		{
			left_vec += *speed increases*;
			right_vec -= *speed decreases*;
		}

		send_vectors_to_motors();
	}

}

void main()
{
	ADC_init();
	init_uart();

	while(1){}
}