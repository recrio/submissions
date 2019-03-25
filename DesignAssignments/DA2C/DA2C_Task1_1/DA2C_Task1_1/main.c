#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>

int main(void)
{
	DDRB = (1<<DDB2); // Make PB2 Output
	PORTB = (0<<DDB2); // Turn on LED
	TCCR0A = 0; // Normal Mode
	TCCR0B = 5; // Set prescaler to 1024
	int ovrflow = 0; // overflow counter
    while (1) 
    {
		TCNT0 = 0; // Reset counter
		ovrflow = 0; // Reset overflow counter
		
		// Delay for 6796
		while (ovrflow < 26) { // Gets to 6656
			while ((TIFR0 & 0x01) == 0) {}
			ovrflow++; // increment ovrflow
			TCNT0 = 0; // reset counter
			TIFR0 = 1; // reset ovf flag
		}
		while (TCNT0 < 140) {} // 6656+140 = 6796
		
		PORTB ^= (1<<DDB2); // Turn off LED
		TCNT0 = 0;	// reset counter
		ovrflow = 0; // reset ovrflow counter
		
		// Delay for 4531
		while (ovrflow < 17) { // 4352
			while ((TIFR0 & 0x01) == 0) {}
			ovrflow++; // increment ovrflow
			TCNT0 = 0; // reset counter
			TIFR0 = 1; // reset ovf flag
		}
		while (TCNT0 < 179) {} // 4352+179 = 4531
		PORTB ^= (1<<DDB2); // Turn on LED
    }
}

