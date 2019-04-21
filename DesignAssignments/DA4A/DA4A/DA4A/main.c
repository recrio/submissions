#define F_CPU 16000000UL
#define MAXSPEED 244
#define CONVERSION .238 // 244/1024

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void PWM_init(void);
void ADC_init(void);

volatile int ADCvalue; // holds the value of ADC
volatile int motor = 0; // State of motor 0 = off, 1 = on
int main(void)
{
    ADC_init(); // initializes ADC
	PWM_init(); // initiales PWM
    while (1) 
    {
		if (motor == 1) {
			ADCSRA |= (1<<ADSC); // Start conversion
			while((ADCSRA&(1<<ADIF))==0); // while not done converting do nothing
			ADCSRA |= (1<<ADIF); // reset converter
			ADCvalue = ADC; // store ADC to ADCvalue for debugging
			OCR0A = CONVERSION*ADCvalue; // 224/1024 is the ratio of the speed so that max speed is 95% of the pwm
		}
		if (motor == 0) {
			OCR0A = 0;
		}
    }
}

ISR(PCINT1_vect) {
	if(!(PINC & (1 << PINC1))) { // if Button pressed
		_delay_ms(100); // debouncing
		while(!(PINC&(1<<PINC1))); // extra debouncing
		motor ^= 1; // Toggle motor
	}
}

void PWM_init(void) {
	DDRD |= (1<<PORTD6); // PD6 as output
	DDRC |= (1<<PORTC1); // PC1 as output for interrupt
	TCCR0A |=
	(1<<WGM01)| // Fast
	(1<<WGM00)| // PWM
	(1<<COM0A1);// Clear OC0A on compare/set OC0A as bottom (non-inverting)
	PCMSK1 |= (1<<PCINT9); // Enable PCINT9
	PCICR |= (1<<PCIE1); // Enable PCMSK1
	TCCR0B |=
	(1<<CS02)| // Prescaler
	(1<<CS00); // 1024 and start pwm
	
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