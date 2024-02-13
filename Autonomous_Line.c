#include <xc.h>

int L_sens_pin = ;
int M_sens_pin = ;
int R_sens_pin = ;
int auto_pin = ;
int left_vec, right_vec;

void __interrupt() autonomous_line()
{
	int l_val, m_val, r_val;

	while(*auto_switch*)
	{
		l_val = *PORTREAD*;
		m_val = *PORTREAD*;
		r_val = *PORTREAD*;
		
		switch(m_val)
		{
			case *sees_line*:
				switch(l_val)
				{
					case *sees_line*:
						break;
					case *!sees_line*
				}
				
				switch(r_val)
				{
				}
				break;
			case *!sees_line*:
				break;
		}
	}

}

void main()
{
	
}