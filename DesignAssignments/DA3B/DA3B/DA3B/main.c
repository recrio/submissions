#define F_CPU 16000000UL
#define PRESCALAR 1024
#define BAUDRATE 9600
#define BAUD_PRESCALAR (((F_CPU / (BAUDRATE * 16UL))) - 1)
#define ONESEC (0xFFFF - ((F_CPU/PRESCALAR)*1) - 60)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

void USART_init(void); // initializes USART settings
void USART_sendChar(char ch); // sends a character
void USART_sendString(char* str); // sends a string
void TIMER_init(void); // initializes timer sequence for interrupts
void ADC_init(void); // initializes ADC settings

volatile int adc_temp;

int main(void)
{
	USART_init(); // initialize USART
	TIMER_init(); // initialize Timer/Interrupt
	ADC_init();
	
	while (1) // Loop forever
	{
	}
}

ISR (TIMER1_OVF_vect) {
	TCNT1 = ONESEC; // set the timer back
	ADCSRA |= (1<<ADSC); // start conversion
	while ((ADCSRA&(1<<ADIF))==0){} // Wait for conversion
	ADCSRA |= (1<<ADIF); // Clear Interrupt Flag
	
	adc_temp = ADCL; // take in lower bits first
	adc_temp = adc_temp | (ADCH<<8); // take in upper bits
	char temp[20]; // buffer
	snprintf(temp,sizeof(temp),"%d\r\n",adc_temp); // print to the buffer 
	USART_sendString(temp); // send the temp out
}

void USART_init( void )
{
	UBRR0H = 0; // not needed
	UBRR0L = BAUD_PRESCALAR; // Baud Prescaler
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */
}

void USART_sendChar(char ch) {
	while (!(UCSR0A & (1<<UDRE0))); // while data reg is not empty: hold
	UDR0 = ch; // place character into reg
}

void USART_sendString(char* str) {
	while ((*str != '\0')) { // while not the end of the string
		while (!(UCSR0A & (1<<UDRE0))); // while data reg is not empty: hold
		USART_sendChar(*str); //take in character to reg
		str++; // next character
	}
}

void TIMER_init(void) {
	TCNT1 = ONESEC; // ONESEC is the number to count up to 0xFFFF for 1 sec delay
	TIMSK1 |= (1 << TOIE0); // Enable Interrupt for Timer1
	sei(); // Enable Global Interrupt
	TCCR1B |= (1<<CS12)|(1<<CS10); // Start timer 1 and set prescaler to 1024
}

void ADC_init(void) {
	ADMUX |= (0<<REFS1)| // Reference Select
			 (1<<REFS0)| // Selected AVcc
			 (0<<ADLAR)| // Left Adjust Result OFF
			 (1<<MUX2) | // Analog Channel Select
			 (0<<MUX1) | // 1 0 1
			 (1<<MUX0) ; // Channel 5 or PC5
	ADCSRA |= (1<<ADEN)| // Enable ADC
			  (0<<ADSC)| // Do not start conversion
			  (0<<ADATE)|// Auto Trigger Disabled
			  (0<<ADIF)| // Interrupt Flag Cleared
			  (0<<ADIE)| // Interrupt Disabled
			  (1<<ADPS2)| // ADC Prescaler Select
			  (0<<ADPS1)| // Set to
			  (1<<ADPS0); // 32
}


