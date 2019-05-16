#define F_CPU 16000000UL
#define CONVERSION 10

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void TIMER_init(void);
void ADC_init(void);

volatile int ADCvalue; // holds the value of ADC
int main(void)
{
	DDRB = 0x0F; // PORTB[3:0] set to output
	PORTB = 0; // Initially set output to 0
	ADC_init(); // initializes ADC
	TIMER_init(); // initializes TIMER
	
	while (1)
	{
		ADCSRA |= (1<<ADSC); // Start conversion
		while((ADCSRA&(1<<ADIF))==0); // while not done converting do nothing
		ADCSRA |= (1<<ADIF); // reset converter
		ADCvalue = ADC; // store ADC to ADCvalue for debugging
		OCR1A = CONVERSION*ADCvalue; // 224/1024 is the ratio of the speed so that max speed is 95% of the pwm
		
		// Sequence
		PORTB = 0x09;					
		while(!(TIFR1 & (1<<OCF1A)));   // Delays using CTC
		TIFR1 |= (1 << OCF1A);			// Resets
		PORTB = 0x03;
		while(!(TIFR1 & (1<<OCF1A)));
		TIFR1 |= (1 << OCF1A);	
		PORTB = 0x06;
		while(!(TIFR1 & (1<<OCF1A)));
		TIFR1 |= (1 << OCF1A);	
		PORTB = 0x0C;
		while(!(TIFR1 & (1<<OCF1A)));
		TIFR1 |= (1 << OCF1A);

	}
}

void TIMER_init(void) {
	TCCR1B = (1<<WGM12) | // CTC mode
			 (1<<CS11); // Prescaler 8
}

void ADC_init(void) {
	DDRC &= (0<<PORTC0); // PC0 as input
	PORTC |= (1<<PORTC1); // Pull up resistor
	ADMUX |= (1<<REFS0); // REFERENCE VOLTAGE AT AREF
	ADCSRA |=
	(1<<ADEN)| // ADC enable
	(1<<ADPS2)| // ADC
	(1<<ADPS1)| // Prescaler
	(1<<ADPS0); // 128
	sei();
}