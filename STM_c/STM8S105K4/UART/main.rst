                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler 
                                      3 ; Version 4.3.0 #14184 (MINGW64)
                                      4 ;--------------------------------------------------------
                                      5 	.module main
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _main
                                     12 	.globl _fputc
                                     13 	.globl _puts
                                     14 ;--------------------------------------------------------
                                     15 ; ram data
                                     16 ;--------------------------------------------------------
                                     17 	.area DATA
                                     18 ;--------------------------------------------------------
                                     19 ; ram data
                                     20 ;--------------------------------------------------------
                                     21 	.area INITIALIZED
                                     22 ;--------------------------------------------------------
                                     23 ; Stack segment in internal ram
                                     24 ;--------------------------------------------------------
                                     25 	.area SSEG
      000001                         26 __start__stack:
      000001                         27 	.ds	1
                                     28 
                                     29 ;--------------------------------------------------------
                                     30 ; absolute external ram data
                                     31 ;--------------------------------------------------------
                                     32 	.area DABS (ABS)
                                     33 
                                     34 ; default segment ordering for linker
                                     35 	.area HOME
                                     36 	.area GSINIT
                                     37 	.area GSFINAL
                                     38 	.area CONST
                                     39 	.area INITIALIZER
                                     40 	.area CODE
                                     41 
                                     42 ;--------------------------------------------------------
                                     43 ; interrupt vector
                                     44 ;--------------------------------------------------------
                                     45 	.area HOME
      008000                         46 __interrupt_vect:
      008000 82 00 80 07             47 	int s_GSINIT ; reset
                                     48 ;--------------------------------------------------------
                                     49 ; global & static initialisations
                                     50 ;--------------------------------------------------------
                                     51 	.area HOME
                                     52 	.area GSINIT
                                     53 	.area GSFINAL
                                     54 	.area GSINIT
      008007 CD 80 8C         [ 4]   55 	call	___sdcc_external_startup
      00800A 4D               [ 1]   56 	tnz	a
      00800B 27 03            [ 1]   57 	jreq	__sdcc_init_data
      00800D CC 80 04         [ 2]   58 	jp	__sdcc_program_startup
      008010                         59 __sdcc_init_data:
                                     60 ; stm8_genXINIT() start
      008010 AE 00 00         [ 2]   61 	ldw x, #l_DATA
      008013 27 07            [ 1]   62 	jreq	00002$
      008015                         63 00001$:
      008015 72 4F 00 00      [ 1]   64 	clr (s_DATA - 1, x)
      008019 5A               [ 2]   65 	decw x
      00801A 26 F9            [ 1]   66 	jrne	00001$
      00801C                         67 00002$:
      00801C AE 00 00         [ 2]   68 	ldw	x, #l_INITIALIZER
      00801F 27 09            [ 1]   69 	jreq	00004$
      008021                         70 00003$:
      008021 D6 80 3A         [ 1]   71 	ld	a, (s_INITIALIZER - 1, x)
      008024 D7 00 00         [ 1]   72 	ld	(s_INITIALIZED - 1, x), a
      008027 5A               [ 2]   73 	decw	x
      008028 26 F7            [ 1]   74 	jrne	00003$
      00802A                         75 00004$:
                                     76 ; stm8_genXINIT() end
                                     77 	.area GSFINAL
      00802A CC 80 04         [ 2]   78 	jp	__sdcc_program_startup
                                     79 ;--------------------------------------------------------
                                     80 ; Home
                                     81 ;--------------------------------------------------------
                                     82 	.area HOME
                                     83 	.area HOME
      008004                         84 __sdcc_program_startup:
      008004 CC 80 4B         [ 2]   85 	jp	_main
                                     86 ;	return from main will return to caller
                                     87 ;--------------------------------------------------------
                                     88 ; code
                                     89 ;--------------------------------------------------------
                                     90 	.area CODE
                                     91 ;	main.c: 20: void fputc(char c) //вывод символа в UART
                                     92 ;	-----------------------------------------
                                     93 ;	 function fputc
                                     94 ;	-----------------------------------------
      00803B                         95 _fputc:
      00803B 88               [ 1]   96 	push	a
      00803C 6B 01            [ 1]   97 	ld	(0x01, sp), a
                                     98 ;	main.c: 22: while(!(UART1_SR & UART_SR_TXE));
      00803E                         99 00101$:
      00803E C6 52 30         [ 1]  100 	ld	a, 0x5230
      008041 2A FB            [ 1]  101 	jrpl	00101$
                                    102 ;	main.c: 23: UART1_DR = c;
      008043 AE 52 31         [ 2]  103 	ldw	x, #0x5231
      008046 7B 01            [ 1]  104 	ld	a, (0x01, sp)
      008048 F7               [ 1]  105 	ld	(x), a
                                    106 ;	main.c: 24: }
      008049 84               [ 1]  107 	pop	a
      00804A 81               [ 4]  108 	ret
                                    109 ;	main.c: 28: void main(void)
                                    110 ;	-----------------------------------------
                                    111 ;	 function main
                                    112 ;	-----------------------------------------
      00804B                        113 _main:
                                    114 ;	main.c: 30: CLK_DIVR = 0x00; //установка тактовой частоты 16 MГц
      00804B 35 00 50 C6      [ 1]  115 	mov	0x50c6+0, #0x00
                                    116 ;	main.c: 31: CLK_PCKENR1 = 0xFF; //включение периферии
      00804F 35 FF 50 C7      [ 1]  117 	mov	0x50c7+0, #0xff
                                    118 ;	main.c: 32: UART1_CR2 = UART_CR2_TEN; //разрешение TX и RX
      008053 35 08 52 35      [ 1]  119 	mov	0x5235+0, #0x08
                                    120 ;	main.c: 33: UART1_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP2); //1 стоп-бит
      008057 C6 52 36         [ 1]  121 	ld	a, 0x5236
      00805A A4 CF            [ 1]  122 	and	a, #0xcf
      00805C C7 52 36         [ 1]  123 	ld	0x5236, a
                                    124 ;	main.c: 34: UART1_BRR2 = 0x03; UART1_BRR1 = 0x68; //9600 бод
      00805F 35 03 52 33      [ 1]  125 	mov	0x5233+0, #0x03
      008063 35 68 52 32      [ 1]  126 	mov	0x5232+0, #0x68
                                    127 ;	main.c: 36: while(1)
      008067                        128 00102$:
                                    129 ;	main.c: 38: printf("Hello,world!\r\n"); //вывод строки в UART
      008067 AE 80 2D         [ 2]  130 	ldw	x, #(___str_1+0)
      00806A CD 80 70         [ 4]  131 	call	_puts
      00806D 20 F8            [ 2]  132 	jra	00102$
                                    133 ;	main.c: 40: }
      00806F 81               [ 4]  134 	ret
                                    135 	.area CODE
                                    136 	.area CONST
                                    137 	.area CONST
      00802D                        138 ___str_1:
      00802D 48 65 6C 6C 6F 2C 77   139 	.ascii "Hello,world!"
             6F 72 6C 64 21
      008039 0D                     140 	.db 0x0d
      00803A 00                     141 	.db 0x00
                                    142 	.area CODE
                                    143 	.area INITIALIZER
                                    144 	.area CABS (ABS)
