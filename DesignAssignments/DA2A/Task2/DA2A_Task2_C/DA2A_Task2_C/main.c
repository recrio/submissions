#define F_CPU 16000000UL //Change frequency to 16MHz
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1<<2);		//PB2 is an output, XXXX X1XX
	PORTB |= (1<<2);	//PB2 set as high to turn off LED, XXXX X1XX
	DDRC &= (0<<2);		//PC2 is an input, XXXX X0XX 
	PORTC |= (0<<2);	//PC2 set as low or unpressed, XXXX X0XX
	while (1) {
		if (!(PINC & (1 << PINC2))){ // If button is pressed
		PORTB &= ~(1<<2); // turn on LED 
		_delay_ms(1250); // delay for 1250ms
		}
	    else { // if button is not pressed
		    PORTB |= (1<<2); // set PB2 to high or LED off
		}
    }
}

