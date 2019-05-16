#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void TIMER_init(void);
void ADC_init(void);

int main(void)
{
	ADC_init(); // initialize ADC settings
	TIMER_init(); // initialize TIMER1
	
	
	while(1)
	{
		ADCSRA |= (1 << ADSC);	// Starts conversion
		while((ADCSRA&(1<<ADIF))==0); // wait for conversion
		ADCSRA |= (1<<ADIF); // reset converter
		ICR1 = 4999; 	// fPWM = 50Hz
		DDRB |= (1<<PB1);	// Set PB1 as output
		OCR1A = ADC;	// Adjusts pulse width range
		_delay_ms(100);	// Short Delay
	}
}

void TIMER_init(void)
{
	//Timer1
	TCCR1A = (1<<COM1A1) | // Clear OC1A on Compare non-inverting
			 (1<<COM1B1) | //
			 (1<<WGM11)  ; // Fast
	TCCR1B = (1<<WGM13) |  // PWM
			 (1<<WGM12) |  // TOP = ICR1
			 (1<<CS11) |  // Prescaler
			 (1<<CS10);	  // 64
}

void ADC_init (void)
{
	ADMUX = (1<<REFS0);											// Set Aref
	
	// right-justified data
	ADCSRA |= (1<<ADEN) |	// ADC enable
			  (1<<ADSC) |	// AD start conversion
			  (1<<ADPS2)|	// ADC
			  (1<<ADPS1)|	// Prescaler
			  (1<<ADPS0);	// 128
}
