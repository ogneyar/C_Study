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
      008000 82 00 80 07             49 	int s_GSINIT ; reset
                                     50 ;--------------------------------------------------------
                                     51 ; global & static initialisations
                                     52 ;--------------------------------------------------------
                                     53 	.area HOME
                                     54 	.area GSINIT
                                     55 	.area GSFINAL
                                     56 	.area GSINIT
      008007 CD 80 82         [ 4]   57 	call	___sdcc_external_startup
      00800A 4D               [ 1]   58 	tnz	a
      00800B 27 03            [ 1]   59 	jreq	__sdcc_init_data
      00800D CC 80 04         [ 2]   60 	jp	__sdcc_program_startup
      008010                         61 __sdcc_init_data:
                                     62 ; stm8_genXINIT() start
      008010 AE 00 00         [ 2]   63 	ldw x, #l_DATA
      008013 27 07            [ 1]   64 	jreq	00002$
      008015                         65 00001$:
      008015 72 4F 00 00      [ 1]   66 	clr (s_DATA - 1, x)
      008019 5A               [ 2]   67 	decw x
      00801A 26 F9            [ 1]   68 	jrne	00001$
      00801C                         69 00002$:
      00801C AE 00 01         [ 2]   70 	ldw	x, #l_INITIALIZER
      00801F 27 09            [ 1]   71 	jreq	00004$
      008021                         72 00003$:
      008021 D6 80 2C         [ 1]   73 	ld	a, (s_INITIALIZER - 1, x)
      008024 D7 00 00         [ 1]   74 	ld	(s_INITIALIZED - 1, x), a
      008027 5A               [ 2]   75 	decw	x
      008028 26 F7            [ 1]   76 	jrne	00003$
      00802A                         77 00004$:
                                     78 ; stm8_genXINIT() end
                                     79 	.area GSFINAL
      00802A CC 80 04         [ 2]   80 	jp	__sdcc_program_startup
                                     81 ;--------------------------------------------------------
                                     82 ; Home
                                     83 ;--------------------------------------------------------
                                     84 	.area HOME
                                     85 	.area HOME
      008004                         86 __sdcc_program_startup:
      008004 CC 80 59         [ 2]   87 	jp	_main
                                     88 ;	return from main will return to caller
                                     89 ;--------------------------------------------------------
                                     90 ; code
                                     91 ;--------------------------------------------------------
                                     92 	.area CODE
                                     93 ;	main.c: 38: void TIM1_overflow_Handler() //__interrupt(11)
                                     94 ;	-----------------------------------------
                                     95 ;	 function TIM1_overflow_Handler
                                     96 ;	-----------------------------------------
      00802E                         97 _TIM1_overflow_Handler:
      00802E 88               [ 1]   98 	push	a
                                     99 ;	main.c: 40: TIM1_SR1 &= ~1;
      00802F 72 11 52 55      [ 1]  100 	bres	0x5255, #0
                                    101 ;	main.c: 41: if (led == 1) {
      008033 C6 00 01         [ 1]  102 	ld	a, _led+0
      008036 4A               [ 1]  103 	dec	a
      008037 26 05            [ 1]  104 	jrne	00112$
      008039 A6 01            [ 1]  105 	ld	a, #0x01
      00803B 6B 01            [ 1]  106 	ld	(0x01, sp), a
      00803D C5                     107 	.byte 0xc5
      00803E                        108 00112$:
      00803E 0F 01            [ 1]  109 	clr	(0x01, sp)
      008040                        110 00113$:
                                    111 ;	main.c: 42: GPIOE->ODR |= (1 << 5);
      008040 C6 50 14         [ 1]  112 	ld	a, 0x5014
                                    113 ;	main.c: 41: if (led == 1) {
      008043 0D 01            [ 1]  114 	tnz	(0x01, sp)
      008045 27 07            [ 1]  115 	jreq	00102$
                                    116 ;	main.c: 42: GPIOE->ODR |= (1 << 5);
      008047 AA 20            [ 1]  117 	or	a, #0x20
      008049 C7 50 14         [ 1]  118 	ld	0x5014, a
      00804C 20 05            [ 2]  119 	jra	00103$
      00804E                        120 00102$:
                                    121 ;	main.c: 46: GPIOE->ODR &= ~(1 << 5);  
      00804E A4 DF            [ 1]  122 	and	a, #0xdf
      008050 C7 50 14         [ 1]  123 	ld	0x5014, a
      008053                        124 00103$:
                                    125 ;	main.c: 48: led  ^= 1;
      008053 90 10 00 01      [ 1]  126 	bcpl	_led+0, #0
                                    127 ;	main.c: 49: }
      008057 84               [ 1]  128 	pop	a
      008058 81               [ 4]  129 	ret
                                    130 ;	main.c: 52: void main(void)
                                    131 ;	-----------------------------------------
                                    132 ;	 function main
                                    133 ;	-----------------------------------------
      008059                        134 _main:
                                    135 ;	main.c: 54: CLK_DIVR = 0x00;
      008059 35 00 50 C6      [ 1]  136 	mov	0x50c6+0, #0x00
                                    137 ;	main.c: 55: CLK_PCKENR1 = 0xFF;
      00805D 35 FF 50 C7      [ 1]  138 	mov	0x50c7+0, #0xff
                                    139 ;	main.c: 57: GPIOE->DDR |= (1 << 5);
      008061 72 1A 50 16      [ 1]  140 	bset	0x5016, #5
                                    141 ;	main.c: 58: GPIOE->ODR |= (1 << 5);
      008065 C6 50 14         [ 1]  142 	ld	a, 0x5014
      008068 AA 20            [ 1]  143 	or	a, #0x20
      00806A C7 50 14         [ 1]  144 	ld	0x5014, a
                                    145 ;	main.c: 60: TIM1_PSCRH = 0x00;
      00806D 35 00 52 60      [ 1]  146 	mov	0x5260+0, #0x00
                                    147 ;	main.c: 61: TIM1_PSCRL = 0xF4;
      008071 35 F4 52 61      [ 1]  148 	mov	0x5261+0, #0xf4
                                    149 ;	main.c: 62: TIM1_CR1 = 0x01;
      008075 35 01 52 50      [ 1]  150 	mov	0x5250+0, #0x01
                                    151 ;	main.c: 63: TIM1_IER = 0x01;
      008079 35 01 52 54      [ 1]  152 	mov	0x5254+0, #0x01
                                    153 ;	main.c: 64: __asm__ ("rim");
      00807D 9A               [ 1]  154 	rim
                                    155 ;	main.c: 65: while(1)
      00807E                        156 00102$:
                                    157 ;	main.c: 67: __asm__ ("WFI");
      00807E 8F               [10]  158 	WFI
      00807F 20 FD            [ 2]  159 	jra	00102$
                                    160 ;	main.c: 69: }
      008081 81               [ 4]  161 	ret
                                    162 	.area CODE
                                    163 	.area CONST
                                    164 	.area INITIALIZER
      00802D                        165 __xinit__led:
      00802D 00                     166 	.db #0x00	; 0
                                    167 	.area CABS (ABS)
