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
	.globl _fputc
	.globl _puts
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
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
;	main.c: 20: void fputc(char c) //вывод символа в UART
;	-----------------------------------------
;	 function fputc
;	-----------------------------------------
_fputc:
	push	a
	ld	(0x01, sp), a
;	main.c: 22: while(!(UART1_SR & UART_SR_TXE));
00101$:
	ld	a, 0x5230
	jrpl	00101$
;	main.c: 23: UART1_DR = c;
	ldw	x, #0x5231
	ld	a, (0x01, sp)
	ld	(x), a
;	main.c: 24: }
	pop	a
	ret
;	main.c: 28: void main(void)
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	main.c: 30: CLK_DIVR = 0x00; //установка тактовой частоты 16 MГц
	mov	0x50c6+0, #0x00
;	main.c: 31: CLK_PCKENR1 = 0xFF; //включение периферии
	mov	0x50c7+0, #0xff
;	main.c: 32: UART1_CR2 = UART_CR2_TEN; //разрешение TX и RX
	mov	0x5235+0, #0x08
;	main.c: 33: UART1_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP2); //1 стоп-бит
	ld	a, 0x5236
	and	a, #0xcf
	ld	0x5236, a
;	main.c: 34: UART1_BRR2 = 0x03; UART1_BRR1 = 0x68; //9600 бод
	mov	0x5233+0, #0x03
	mov	0x5232+0, #0x68
;	main.c: 36: while(1)
00102$:
;	main.c: 38: printf("Hello,world!\r\n"); //вывод строки в UART
	ldw	x, #(___str_1+0)
	call	_puts
	jra	00102$
;	main.c: 40: }
	ret
	.area CODE
	.area CONST
	.area CONST
___str_1:
	.ascii "Hello,world!"
	.db 0x0d
	.db 0x00
	.area CODE
	.area INITIALIZER
	.area CABS (ABS)
