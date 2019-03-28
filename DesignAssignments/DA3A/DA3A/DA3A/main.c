#define F_CPU 16000000UL
#define PRESCALAR 1024
#define BAUDRATE 9600
#define BAUD_PRESCALAR (((F_CPU / (BAUDRATE * 16UL))) - 1)
#define ONESEC (0xFFFF - ((F_CPU/PRESCALAR)*1) - 26)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

void USART_init(void); // initializes USART settings
void USART_sendChar(char ch); // sends a character
void USART_sendString(char* str); // sends a string
void TIMER_init(void); // initializes timer sequence for interrupts

char num = '7'; // random number
char string[] = "Hello World!"; // basic string
float floating = 12.345; // random float value
char fl[20]; // character buffer for float value


int main(void)
{
	snprintf(fl,sizeof(fl),"%f\r\n",floating); // converts floating value into string
	USART_init(); // initialize USART
	TIMER_init(); // initialize Timer/Interrupt
	
    while (1) // Loop forever
    {
    }
}

void USART_init( void )
{
	UBRR0H = 0; // not needed
	UBRR0L = BAUD_PRESCALAR; // Baud Prescaler
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */
}