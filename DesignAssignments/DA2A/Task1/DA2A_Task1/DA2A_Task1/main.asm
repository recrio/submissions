.ORG 0x0000

init:
	LDI R16, 4			; 0b000100 or PB2
	OUT DDRB, R16		; PB2 set as an output
	LDI R17, 0			; Used for toggling PB2 
	LDI R20, 5			; 0b101 for 1024 prescaler
	STS TCCR1B, R20		; prescaler set
	LDI R20, 0			; to be used for TCNT1 reset

main:
	RCALL TCNT1_RESET	; reset TCNT1

	RCALL delay_ON		; call for .435 second delay
	EOR R17, R16		; XOR toggles the LED
	OUT PORTB, R17		; should turn off the LED

	RCALL TCNT1_RESET	; reset TCNT1

	RCALL delay_OFF		; call for .290 second delay
	EOR R17, R16		; toggle LED again
	OUT PORTB, R17		; should turn on the LED

	RJMP main			; repeat

TCNT1_RESET:
	STS TCNT1H, R20		; timer/counter
	STS TCNT1L, R20		; reset
	RET

check_high_ON:
	CPI R27, 0x1A		; check if high byte of timer matches 0x1A
	BRLT delay_ON		; if not, recheck timer
	RET					; return to main

delay_OFF:				; TCNT should be 4531 or 0x11B3
	LDS R27, TCNT1H		; R27 = high byte of timer/counter
	LDS R26, TCNT1L		; R26 = low byte of timer/counter

	CPI R26, 0xB3		; check if low byte of timer matches 0x8C
	BRSH check_high_OFF	; if low bytes match, check high byte next
	
	RJMP delay_OFF		; repeat until matching

check_high_OFF:
	CPI R27, 0x11		; check if high byte of timer matches 0x1A
	BRLT delay_OFF		; if not, recheck timer
	RET					; return to main

delay_ON:				; TCNT should be 6796 or 0x1A8C
	LDS R27, TCNT1H		; R27 = high byte of timer/counter
	LDS R26, TCNT1L		; R26 = low byte of timer/counter

	CPI R26, 0x8C		; check if low byte of timer matches 0x8C
	BRSH check_high_ON	; if low bytes match, check high byte next
	
	RJMP delay_ON		; repeat until matching