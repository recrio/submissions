#include <stdio.h>
#include <avr/io.h>

int main(void)
{
    DDRB = (1<<2); //Make PB2 Output
	TCCR1B = 5;	// set prescaler to 1024
	
    while (1) 
    {
		TCNT1 = 0; // set timer/counter to 0
		while (TCNT1 != 6796) {
		//do nothing
		}
		PORTB ^= (1<<2); // toggle PB2 using xor
		TCNT1 = 0; // reset again
		while (TCNT1 != 4531) {
		// do nothing	
		}
		PORTB ^= (1<<2); // toggle PB2 using xor
    }
}

