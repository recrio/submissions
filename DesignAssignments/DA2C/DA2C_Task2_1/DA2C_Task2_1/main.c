#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

int ovrflow = 0; // global ovrflow counter

int main(void)
{
	DDRB = (1<<DDB2); // Make PB2 Output
	PORTB = (0<<DDB2); // Turn on LED
	TIMSK0 |= (1<<TOIE0); // Set up interrupt
	TCCR0A = 0; // Normal Mode
	sei(); // interrupt enable
	TCCR0B = 5; // Set prescaler to 1024
	
	while (1)
	{
		
	}
}

ISR (TIMER0_OVF_vect) {
	ovrflow++; //increment ovrflow
	if (ovrflow == 26) { // delay for .435s
		TCNT0 = 0;
		while (TCNT0 < 140) {}
		PORTB ^= (1<<DDB2); // Turn OFF
		TCNT0 = 0; // reset counter
	}
	else if (ovrflow == 43) { // delay for .29s
		TCNT0 = 0;
		while (TCNT0 < 179) {}
		PORTB ^= (1<<DDB2); // Turn ON
		ovrflow = 0; // reset ovrflow
		TCNT0 = 0; // reset counter
	}
	TCNT0 = 0;
}