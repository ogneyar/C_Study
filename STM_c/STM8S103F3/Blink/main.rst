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
                                     12 	.globl _TIM1_overflow_Handler
                                     13 	.globl _led
                                     14 ;--------------------------------------------------------
                                     15 ; ram data
                                     16 ;--------------------------------------------------------
                                     17 	.area DATA
                                     18 ;--------------------------------------------------------
                                     19 ; ram data
                                     20 ;--------------------------------------------------------
                                     21 	.area INITIALIZED
      000001                         22 _led::
      000001                         23 	.ds 1
                                     24 ;--------------------------------------------------------
                                     25 ; Stack segment in internal ram
                                     26 ;--------------------------------------------------------
                                     27 	.area SSEG
      000002                         28 __start__stack:
      000002                         29 	.ds	1
                                     30 
                                     31 ;--------------------------------------------------------
                                     32 ; absolute external ram data
                                     33 ;--------------------------------------------------------
                                     34 	.area DABS (ABS)
                                     35 
                                     36 ; default segment ordering for linker
                                     37 	.area HOME
                                     38 	.area GSINIT
                                     39 	.area GSFINAL
                                     40 	.area CONST
                                     41 	.area INITIALIZER
                                     42 	.area CODE
                                     43 
                                     44 ;--------------------------------------------------------
                                     45 ; interrupt vector
                                     46 ;--------------------------------------------------------
                                     47 	.area HOME
      008000                         48 __interrupt_vect:
      008000 82 00 80 3B             49 	int s_GSINIT ; reset
      008004 82 00 00 00             50 	int 0x000000 ; trap
      008008 82 00 00 00             51 	int 0x000000 ; int0
      00800C 82 00 00 00             52 	int 0x000000 ; int1
      008010 82 00 00 00             53 	int 0x000000 ; int2
      008014 82 00 00 00             54 	int 0x000000 ; int3
      008018 82 00 00 00             55 	int 0x000000 ; int4
      00801C 82 00 00 00             56 	int 0x000000 ; int5
      008020 82 00 00 00             57 	int 0x000000 ; int6
      008024 82 00 00 00             58 	int 0x000000 ; int7
      008028 82 00 00 00             59 	int 0x000000 ; int8
      00802C 82 00 00 00             60 	int 0x000000 ; int9
      008030 82 00 00 00             61 	int 0x000000 ; int10
      008034 82 00 80 62             62 	int _TIM1_overflow_Handler ; int11
                                     63 ;--------------------------------------------------------
                                     64 ; global & static initialisations
                                     65 ;--------------------------------------------------------
                                     66 	.area HOME
                                     67 	.area GSINIT
                                     68 	.area GSFINAL
                                     69 	.area GSINIT
      00803B CD 80 B6         [ 4]   70 	call	___sdcc_external_startup
      00803E 4D               [ 1]   71 	tnz	a
      00803F 27 03            [ 1]   72 	jreq	__sdcc_init_data
      008041 CC 80 38         [ 2]   73 	jp	__sdcc_program_startup
      008044                         74 __sdcc_init_data:
                                     75 ; stm8_genXINIT() start
      008044 AE 00 00         [ 2]   76 	ldw x, #l_DATA
      008047 27 07            [ 1]   77 	jreq	00002$
      008049                         78 00001$:
      008049 72 4F 00 00      [ 1]   79 	clr (s_DATA - 1, x)
      00804D 5A               [ 2]   80 	decw x
      00804E 26 F9            [ 1]   81 	jrne	00001$
      008050                         82 00002$:
      008050 AE 00 01         [ 2]   83 	ldw	x, #l_INITIALIZER
      008053 27 09            [ 1]   84 	jreq	00004$
      008055                         85 00003$:
      008055 D6 80 60         [ 1]   86 	ld	a, (s_INITIALIZER - 1, x)
      008058 D7 00 00         [ 1]   87 	ld	(s_INITIALIZED - 1, x), a
      00805B 5A               [ 2]   88 	decw	x
      00805C 26 F7            [ 1]   89 	jrne	00003$
      00805E                         90 00004$:
                                     91 ; stm8_genXINIT() end
                                     92 	.area GSFINAL
      00805E CC 80 38         [ 2]   93 	jp	__sdcc_program_startup
                                     94 ;--------------------------------------------------------
                                     95 ; Home
                                     96 ;--------------------------------------------------------
                                     97 	.area HOME
                                     98 	.area HOME
      008038                         99 __sdcc_program_startup:
      008038 CC 80 8D         [ 2]  100 	jp	_main
                                    101 ;	return from main will return to caller
                                    102 ;--------------------------------------------------------
                                    103 ; code
                                    104 ;--------------------------------------------------------
                                    105 	.area CODE
                                    106 ;	main.c: 36: void TIM1_overflow_Handler() __interrupt(11)
                                    107 ;	-----------------------------------------
                                    108 ;	 function TIM1_overflow_Handler
                                    109 ;	-----------------------------------------
      008062                        110 _TIM1_overflow_Handler:
      008062 88               [ 1]  111 	push	a
                                    112 ;	main.c: 38: TIM1_SR1 &= ~1;
      008063 72 11 52 55      [ 1]  113 	bres	0x5255, #0
                                    114 ;	main.c: 39: if (led == 1) {
      008067 C6 00 01         [ 1]  115 	ld	a, _led+0
      00806A 4A               [ 1]  116 	dec	a
      00806B 26 05            [ 1]  117 	jrne	00112$
      00806D A6 01            [ 1]  118 	ld	a, #0x01
      00806F 6B 01            [ 1]  119 	ld	(0x01, sp), a
      008071 C5                     120 	.byte 0xc5
      008072                        121 00112$:
      008072 0F 01            [ 1]  122 	clr	(0x01, sp)
      008074                        123 00113$:
                                    124 ;	main.c: 40: GPIOB->ODR |= (1 << 5);
      008074 C6 50 05         [ 1]  125 	ld	a, 0x5005
                                    126 ;	main.c: 39: if (led == 1) {
      008077 0D 01            [ 1]  127 	tnz	(0x01, sp)
      008079 27 07            [ 1]  128 	jreq	00102$
                                    129 ;	main.c: 40: GPIOB->ODR |= (1 << 5);
      00807B AA 20            [ 1]  130 	or	a, #0x20
      00807D C7 50 05         [ 1]  131 	ld	0x5005, a
      008080 20 05            [ 2]  132 	jra	00103$
      008082                        133 00102$:
                                    134 ;	main.c: 44: GPIOB->ODR &= ~(1 << 5);  
      008082 A4 DF            [ 1]  135 	and	a, #0xdf
      008084 C7 50 05         [ 1]  136 	ld	0x5005, a
      008087                        137 00103$:
                                    138 ;	main.c: 46: led  ^= 1;
      008087 90 10 00 01      [ 1]  139 	bcpl	_led+0, #0
                                    140 ;	main.c: 47: }
      00808B 84               [ 1]  141 	pop	a
      00808C 80               [11]  142 	iret
                                    143 ;	main.c: 50: void main(void)
                                    144 ;	-----------------------------------------
                                    145 ;	 function main
                                    146 ;	-----------------------------------------
      00808D                        147 _main:
                                    148 ;	main.c: 52: CLK_DIVR = 0x00;
      00808D 35 00 50 C6      [ 1]  149 	mov	0x50c6+0, #0x00
                                    150 ;	main.c: 53: CLK_PCKENR1 = 0xFF;
      008091 35 FF 50 C7      [ 1]  151 	mov	0x50c7+0, #0xff
                                    152 ;	main.c: 55: GPIOB->DDR |= (1 << 5);
      008095 72 1A 50 07      [ 1]  153 	bset	0x5007, #5
                                    154 ;	main.c: 56: GPIOB->ODR |= (1 << 5);
      008099 C6 50 05         [ 1]  155 	ld	a, 0x5005
      00809C AA 20            [ 1]  156 	or	a, #0x20
      00809E C7 50 05         [ 1]  157 	ld	0x5005, a
                                    158 ;	main.c: 58: TIM1_PSCRH = 0x00;
      0080A1 35 00 52 60      [ 1]  159 	mov	0x5260+0, #0x00
                                    160 ;	main.c: 59: TIM1_PSCRL = 0xF4;
      0080A5 35 F4 52 61      [ 1]  161 	mov	0x5261+0, #0xf4
                                    162 ;	main.c: 60: TIM1_CR1 = 0x01;
      0080A9 35 01 52 50      [ 1]  163 	mov	0x5250+0, #0x01
                                    164 ;	main.c: 61: TIM1_IER = 0x01;
      0080AD 35 01 52 54      [ 1]  165 	mov	0x5254+0, #0x01
                                    166 ;	main.c: 62: __asm__ ("rim");
      0080B1 9A               [ 1]  167 	rim
                                    168 ;	main.c: 63: while(1)
      0080B2                        169 00102$:
                                    170 ;	main.c: 65: __asm__ ("WFI");
      0080B2 8F               [10]  171 	WFI
      0080B3 20 FD            [ 2]  172 	jra	00102$
                                    173 ;	main.c: 67: }
      0080B5 81               [ 4]  174 	ret
                                    175 	.area CODE
                                    176 	.area CONST
                                    177 	.area INITIALIZER
      008061                        178 __xinit__led:
      008061 00                     179 	.db #0x00	; 0
                                    180 	.area CABS (ABS)
