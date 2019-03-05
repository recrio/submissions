.ORG 0
init:
 CBI DDRC, 2		; PC2 as input, Clears DDRC2
 LDI R16, 4			; PB2 
 OUT DDRB, R16		; SET PB2 as output
 LDI R17,0			; toggle for PC2
 LDI R20,5			; 0b101 for prescaler 1024
 STS TCCR1B, R20	; Set prescaler to 1024
 LDI R20, 0			; For counter/timer reset
 EOR R17, R16		; Toggle with XOR so that it starts as OFF
	
main:
 OUT PORTB, R17		; Turn off LED
 SBIC PINC, 2		; Skip if PC2 is pressed 
 RJMP keep_checking	; if not then jump

 EOR R17, R16		; Toggle again
 OUT PORTB, R17		; Turn on LED
 EOR R17, R16		; Toggle back
 STS TCNT1H, R20	; Reset
 STS TCNT1L, R20	; Timer/Counter

 RCALL delay		; call delay of 1.25s

 RJMP main			; Loop again

keep_checking:
 SBI PORTB, 2		; PB2 = 1, set it high so it will not light up
 RJMP main			; Loop back to check for button press

delay:				; TCNT1 should be 19,531 or 0x4C4B
 LDS R27, TCNT1H	; high byte of timer/counter
 LDS R26, TCNT1L	; low byte of timer/counter
 CPI R26,0x4B		; check if low byte matches
 
 BRSH check_high	; if matching, then check high byte
 
 RJMP delay			; else keep checking

check_high:
 CPI R27,0x4C		; check high byte
 BRLT delay			; if not matching , keep checking
 RET				; if timer/counter is finally matching, return