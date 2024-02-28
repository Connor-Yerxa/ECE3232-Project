#include <xc.h>

int L_sens_pin = ;
int M_sens_pin = ;
int R_sens_pin = ;
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

int read_adc(sensor)
{
	volatile unsigned int ADC_val;
	sensor = 0; 		//set to output
	sensor_latch = 1;	//set pin high
	ADCON0bits.ADGO = 1;
	while(ADCON0bits.ADGO){}
	ADC_val = ADRESH << 8;
        ADC_val += ADRESL;
	sensor = 1; 		//set to input
	return ADC_val;
}


void __interrupt() autonomous_line()
{
	volatile unsigned int l_val, m_val, r_val;

	while(*auto_switch*)
	{
		l_val = read_adc(L_sens_pin);
		m_val = read_adc(M_sens_pin);
		r_val = read_adc(R_sens_pin);
		
		if(l_val == *line colour*)
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
}