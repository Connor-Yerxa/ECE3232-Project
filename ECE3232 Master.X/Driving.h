#pragma config WDTE = OFF
#pragma config  FEXTOSC = ECH 
#pragma config RSTOSC = HFINT32 
#pragma config CLKOUTEN = ON 
#pragma config CSWEN = ON 
#pragma config FCMEN = ON 

#define _XTAL_FREQ 400000

void Vdrive(int y, int x)
{
    volatile int l_motor, r_motor;
    volatile unsigned char left_dir, right_dir;
    
    if(x > 0 && y > 0)
    {
        l_motor = y - (x*2);
        r_motor = y + x;
    }
    else{if(x < 0 && y > 0)
    {
        l_motor = y - x;
        r_motor = y + (x*2);
    }
    else
    {
        l_motor = y - x;
        r_motor = y + x;
    }}
    
    
    if (l_motor > 100 || l_motor < -100)
    {
        l_motor = (l_motor/abs(l_motor)) * 100;
    }
    if (r_motor > 100 || r_motor < -100)
    {
        r_motor = (r_motor/abs(r_motor)) * 100;
    }
    
    if (l_motor > 0)
    {
        left_dir = 1;
    }
    else
    {
        left_dir = 2;
    }
    
    if (r_motor > 0)
    {
        right_dir = 1;
    }
    else
    {
        right_dir = 2;
    }
    
    motor(left_dir, abs(l_motor), right_dir, abs(r_motor));
}

void drive()
{
    volatile int dir;
    volatile int x, y;
    
    user_data();
    
    if(joy_ry > 25){dir = 1;}
    else{dir = -1;}
    
    x = (joy_rx - 50)*2;
    y = dir * (joy_ly);
    
    Vdrive(y, x);
}