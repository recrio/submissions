//	Set clock frequency
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//	Set up UART for printf();
#ifndef BAUD
#define BAUD 9600
#endif
#include "inc\STDIO_UART.c"

//	Include nRF24L01+ library
#include "inc\nrf24l01.c"
#include "inc\nrf24l01-mnemonics.h"
#include "inc\spi.c"
void print_config(void);

//	Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;

void ADC_init (void);
volatile unsigned adc_temp;
volatile char temp[20];

int main(void)
{
	//	Set cliche message to send (message cannot exceed 32 characters)
	char tx_message[32];				// Define string array
	strcpy(tx_message,"It's working!");	// Copy string into array
	
	//	Initialize UART
	uart_init();
	
	//  Initialize ADC
	ADC_init();
	
	//	Initialize nRF24L01+ and print configuration info
	nrf24_init();
	print_config();
	
	//	Start listening to incoming messages
	nrf24_start_listening();
	
	strcpy(tx_message,"Initializing Chat Room...");	// Copy string into array
	nrf24_send_message(tx_message);
	
	while (1)
	{		
		if (message_received)
		{
			//	Message received, print it
			message_received = false;
			printf("Received Temperature: %s\n\n",nrf24_read_message());
			ADCSRA |= (1<<ADSC); // start conversion
			while ((ADCSRA&(1<<ADIF))==0){} // Wait for conversion
			ADCSRA |= (1<<ADIF); // Clear Interrupt Flag
	
			adc_temp = ADCL; // take in lower bits first
			adc_temp = adc_temp | (ADCH<<8); // take in upper bits
			adc_temp = (adc_temp/1024.0) * 5000/10;
			adc_temp = adc_temp*1.8 + 32;
			snprintf(temp,sizeof(temp),"%d\r\n",adc_temp); // print to the buffer 

			//	Send message as response
			_delay_ms(500);
			status = nrf24_send_message(temp);
			if (status == true) printf("Temperature Successfully Sent\n\n");
		}
	}
}

//	Interrupt on IRQ pin
ISR(INT0_vect)
{
	message_received = true;
}


//  Prints configuration
void print_config(void)
{
	uint8_t data;
	printf("Startup successful\n\n nRF24L01+ configured as:\n");
	printf("-------------------------------------------\n");
	nrf24_read(CONFIG,&data,1);
	printf("CONFIG		0x%x\n",data);
	nrf24_read(EN_AA,&data,1);
	printf("EN_AA			0x%x\n",data);
	nrf24_read(EN_RXADDR,&data,1);
	printf("EN_RXADDR		0x%x\n",data);
	nrf24_read(SETUP_RETR,&data,1);
	printf("SETUP_RETR		0x%x\n",data);
	nrf24_read(RF_CH,&data,1);
	printf("RF_CH			0x%x\n",data);
	nrf24_read(RF_SETUP,&data,1);
	printf("RF_SETUP		0x%x\n",data);
	nrf24_read(STATUS,&data,1);
	printf("STATUS		0x%x\n",data);
	nrf24_read(FEATURE,&data,1);
	printf("FEATURE		0x%x\n",data);
	printf("-------------------------------------------\n\n");
}

void ADC_init(void) {
	ADMUX |= (1<<REFS0)| // Selected AVcc
			 (1<<MUX2) ;			 
	ADCSRA |= (1<<ADEN)| // Enable ADC
			  (0<<ADSC)| // Do not start conversion
			  (0<<ADATE)|// Auto Trigger Disabled
			  (0<<ADIF)| // Interrupt Flag Cleared
			  (0<<ADIE)| // Interrupt Disabled
			  (1<<ADPS2)| // ADC Prescaler Select
			  (0<<ADPS1)| // Set to
			  (1<<ADPS0); // 32
}
