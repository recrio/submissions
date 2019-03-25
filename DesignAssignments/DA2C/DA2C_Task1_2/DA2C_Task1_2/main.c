#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>

int main(void)
{
	DDRB = (1<<DDB2); // Make PB2 Output
	DDRC = (0<<DDC2); // Make PC2 Input
	PORTB = (1<<DDB2); // Turn off LED
	PORTC = (1<<DDC2); // Turn on pull-up transistor
	TCCR0A = 0; // Normal Mode
	TCCR0B = 5; // Set prescaler to 1024
	int ovrflow = 0; // overflow counter
	while (1)
	{
		if (!(PINC & (1 << PINC2))) {	// if button pressed
		PORTB ^= (1<<DDB2); // Turn on LED	
		TCNT0 = 0; // Reset counter
		ovrflow = 0; // Reset overflow counter
		

		// Delay for 1.25 sec (19531 TCNT)
		while (ovrflow < 76) { // Gets to 19456
			while ((TIFR0 & 0x01) == 0) {}
			ovrflow++; // increment ovrflow
			TCNT0 = 0; // reset counter
			TIFR0 = 1; // reset ovf flag
		}
		while (TCNT0 < 75) {} // 19456+75 = 19531
		
		PORTB ^= (1<<DDB2); // Turn off LED

		}
	}
}
