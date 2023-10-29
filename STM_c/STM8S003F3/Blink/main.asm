;--------------------------------------------------------
; File Created by SDCC : free open source ISO C Compiler 
; Version 4.3.0 #14184 (MINGW64)
;--------------------------------------------------------
	.module main
	.optsdcc -mstm8
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _TIM1_overflow_Handler
	.globl _led
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
_led::
	.ds 1
;--------------------------------------------------------
; Stack segment in internal ram
;--------------------------------------------------------
	.area SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area DABS (ABS)

; default segment ordering for linker
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area CONST
	.area INITIALIZER
	.area CODE

;--------------------------------------------------------
; interrupt vector
;--------------------------------------------------------
	.area HOME
__interrupt_vect:
	int s_GSINIT ; reset
	int 0x000000 ; trap
	int 0x000000 ; int0
	int 0x000000 ; int1
	int 0x000000 ; int2
	int 0x000000 ; int3
	int 0x000000 ; int4
	int 0x000000 ; int5
	int 0x000000 ; int6
	int 0x000000 ; int7
	int 0x000000 ; int8
	int 0x000000 ; int9
	int 0x000000 ; int10
	int _TIM1_overflow_Handler ; int11
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area GSINIT
	call	___sdcc_external_startup
	tnz	a
	jreq	__sdcc_init_data
	jp	__sdcc_program_startup
__sdcc_init_data:
; stm8_genXINIT() start
	ldw x, #l_DATA
	jreq	00002$
00001$:
	clr (s_DATA - 1, x)
	decw x
	jrne	00001$
00002$:
	ldw	x, #l_INITIALIZER
	jreq	00004$
00003$:
	ld	a, (s_INITIALIZER - 1, x)
	ld	(s_INITIALIZED - 1, x), a
	decw	x
	jrne	00003$
00004$:
; stm8_genXINIT() end
	.area GSFINAL
	jp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
__sdcc_program_startup:
	jp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;	main.c: 44: void TIM1_overflow_Handler() __interrupt(11)
;	-----------------------------------------
;	 function TIM1_overflow_Handler
;	-----------------------------------------
_TIM1_overflow_Handler:
	push	a
;	main.c: 46: TIM1_SR1 &= ~1;
	bres	0x5255, #0
;	main.c: 47: if (led == 1) {
	ld	a, _led+0
	dec	a
	jrne	00112$
	ld	a, #0x01
	ld	(0x01, sp), a
	.byte 0xc5
00112$:
	clr	(0x01, sp)
00113$:
;	main.c: 48: GPIO->ODR |= (1 << LED);
	ld	a, 0x500a
;	main.c: 47: if (led == 1) {
	tnz	(0x01, sp)
	jreq	00102$
;	main.c: 48: GPIO->ODR |= (1 << LED);
	or	a, #0x08
	ld	0x500a, a
	jra	00103$
00102$:
;	main.c: 52: GPIO->ODR &= ~(1 << LED);  
	and	a, #0xf7
	ld	0x500a, a
00103$:
;	main.c: 54: led  ^= 1;
	bcpl	_led+0, #0
;	main.c: 55: }
	pop	a
	iret
;	main.c: 58: void main(void)
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	main.c: 60: CLK_DIVR = 0x00;
	mov	0x50c6+0, #0x00
;	main.c: 61: CLK_PCKENR1 = 0xFF;
	mov	0x50c7+0, #0xff
;	main.c: 64: __asm__ ("bset	0x500C, #3");	
	bset	0x500C, #3
;	main.c: 66: TIM1_PSCRH = 0x00;
	mov	0x5260+0, #0x00
;	main.c: 67: TIM1_PSCRL = 0xF4;
	mov	0x5261+0, #0xf4
;	main.c: 68: TIM1_CR1 = 0x01;
	mov	0x5250+0, #0x01
;	main.c: 69: TIM1_IER = 0x01;
	mov	0x5254+0, #0x01
;	main.c: 70: __asm__ ("rim");
	rim
;	main.c: 71: while(1)
00102$:
;	main.c: 73: __asm__ ("WFI");
	WFI
	jra	00102$
;	main.c: 75: }
	ret
	.area CODE
	.area CONST
	.area INITIALIZER
__xinit__led:
	.db #0x00	; 0
	.area CABS (ABS)
