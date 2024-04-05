#pragma config WDTE = OFF
#pragma config  FEXTOSC = ECH 
#pragma config RSTOSC = HFINT32 
#pragma config CLKOUTEN = ON 
#pragma config CSWEN = ON 
#pragma config FCMEN = ON 

#define _XTAL_FREQ 400000

void shield_code(void) { //shield code function
	PCLS_info();
    __delay_ms(50);
    shield_flag = 1;
	if(shield_flag){ //non zero if shot
		user_data();
		__delay_ms(50);
		if(swA){//switches
            set_laser(1);
            __delay_ms(100);
            shoot_laser(1);
			transmit_shield_code();
            __delay_ms(100);
            set_laser(0);
            __delay_ms(6000);
		}
	}
}