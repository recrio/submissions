.include <m328pdef.inc>
.ORG 0	; main code location
JMP MAIN
.ORG 0x02 ; interrupt 0 code location
JMP EX0_ISR

MAIN:
	// Initialize stack pointer
	LDI R20, HIGH(RAMEND) 
	OUT SPH, R20
	LDI R20, LOW(RAMEND)
	OUT SPL, R20

	LDI R22, (1<<2) ; for toggling PB2
	
	SBI PORTD, 2	; Pull-UP activated
	LDI R20, 0x00	; setting to make
	STS EICRA, R20	; INT0 low generate an interrupt
	SBI DDRB, 2		; PORTB.2 set to output
	LDI R20, 1<<INT0; enable
	OUT EIMSK, R20	; INT0
	SEI				; enable global interrupt
HERE:
	OUT PORTB, R22	; Turn off LED 
	JMP HERE		; infinite loop to catch interrupts

EX0_ISR:
	IN R21, PORTB	; Take in status of PORTB
	EOR R21, R22	; Toggle it
	OUT PORTB, R21	; Output to LED
	RCALL DELAY		; Call a delay of 1.25s
	RETI			; Return with interrupt flag on

DELAY:				; Delay adds up to 20 million clock cycles
    LDI  r18, 102	
    LDI  r19, 118	
    LDI  r20, 194	
L1: DEC  r20		
    BRNE L1			
    DEC  r19		
    BRNE L1			
    DEC  r18	
    BRNE L1			
	RET				
