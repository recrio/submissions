;
; DA1B.asm
;
; Created: 2/21/2019 11:38:28 AM
; Author : Josh R
;

.ORG 0
.EQU STARTADDS = 0x0200
.EQU DIVISIBLES = 0x0400
.EQU NDIVISIBLE = 0x0600
	
	LDI R23, 0				; Holds Zero
	LDI R20, 99				; Counter = 99
	LDI R25, 11				; First Value = 11
	LDI XL, low(STARTADDS)	; X points to
	LDI XH, high(STARTADDS) ; 0x0200
	LDI YL, low(DIVISIBLES)	; Y points to
	LDI YH, high(DIVISIBLES); 0x0400
	LDI ZL, low(NDIVISIBLE)	; Z points to
	LDI ZH, high(NDIVISIBLE); 0x0600

POPULATE:
	ST X+, R25				; Store R25 into X, increment pointer
	INC R25					; Increment R25
	DEC R20					; Decrement Counter
	BRNE POPULATE			; Keep populating if counter != 0

	LDI XL, low(STARTADDS)	; X points to
	LDI XH, high(STARTADDS) ; 0x0200 again
	LDI R20, 99+1			; Counter = 99+1 because BREQ
							; checks before the operations

DIV3:
	DEC R20					; Decrement Counter
	BREQ DONEDIV			; If counter = 0, parsed through all numbers
	LD R25, X				; R25 = X data
	LD R24, X+				; R24 = X data, increment X pointer
CHECK:
	CPI R25, 0				; Check if R25 = 0
	BREQ DIVIDES			; If 0 then divisible
	SUBI R25, 3				; Subtract R25 by 3
	BRPL CHECK				; If not negative, go back to CHECK
NDIVIDES:
	ST Z+, R24				; store R24 in Z
	RJMP DIV3				; Jump to DIV3
DIVIDES:
	ST Y+, R24				; store R25 in Y
	RJMP DIV3				; Jump to DIV3

DONEDIV:
	LDI R20, 99				; Counter set to 99 again
	LDI YL, low(DIVISIBLES) ; Y points to
	LDI YH, high(DIVISIBLES); 0x0400 again
	LDI ZL, low(NDIVISIBLE) ; Z points to
	LDI ZH, high(NDIVISIBLE); 0x0600 again

SUM:
	LD R21, Y+				; R21 = Y data, increment Y pointer
	LD R22, Z+				; R22 = Z data, increment Z pointer
	ADD R17, R21			; R17 = R17 + R21
	ADC R16, R23			; R16 = R16 + 0 + Carry
	ADD R19, R22			; R19 = R19 + R22
	ADC R18, R23			; R18 = R18 + 0 + Carry
	DEC R20					; Decrement counter
	BRNE SUM				; if counter != 0 keep summing

DONE: RJMP DONE

		