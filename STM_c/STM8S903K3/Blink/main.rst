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
                                     12 	.globl _led
                                     13 ;--------------------------------------------------------
                                     14 ; ram data
                                     15 ;--------------------------------------------------------
                                     16 	.area DATA
                                     17 ;--------------------------------------------------------
                                     18 ; ram data
                                     19 ;--------------------------------------------------------
                                     20 	.area INITIALIZED
      000001                         21 _led::
      000001                         22 	.ds 1
                                     23 ;--------------------------------------------------------
                                     24 ; Stack segment in internal ram
                                     25 ;--------------------------------------------------------
                                     26 	.area SSEG
      000002                         27 __start__stack:
      000002                         28 	.ds	1
                                     29 
                                     30 ;--------------------------------------------------------
                                     31 ; absolute external ram data
                                     32 ;--------------------------------------------------------
                                     33 	.area DABS (ABS)
                                     34 
                                     35 ; default segment ordering for linker
                                     36 	.area HOME
                                     37 	.area GSINIT
                                     38 	.area GSFINAL
                                     39 	.area CONST
                                     40 	.area INITIALIZER
                                     41 	.area CODE
                                     42 
                                     43 ;--------------------------------------------------------
                                     44 ; interrupt vector
                                     45 ;--------------------------------------------------------
                                     46 	.area HOME
      008000                         47 __interrupt_vect:
      008000 82 00 80 07             48 	int s_GSINIT ; reset
                                     49 ;--------------------------------------------------------
                                     50 ; global & static initialisations
                                     51 ;--------------------------------------------------------
                                     52 	.area HOME
                                     53 	.area GSINIT
                                     54 	.area GSFINAL
                                     55 	.area GSINIT
      008007 CD 80 3E         [ 4]   56 	call	___sdcc_external_startup
      00800A 4D               [ 1]   57 	tnz	a
      00800B 27 03            [ 1]   58 	jreq	__sdcc_init_data
      00800D CC 80 04         [ 2]   59 	jp	__sdcc_program_startup
      008010                         60 __sdcc_init_data:
                                     61 ; stm8_genXINIT() start
      008010 AE 00 00         [ 2]   62 	ldw x, #l_DATA
      008013 27 07            [ 1]   63 	jreq	00002$
      008015                         64 00001$:
      008015 72 4F 00 00      [ 1]   65 	clr (s_DATA - 1, x)
      008019 5A               [ 2]   66 	decw x
      00801A 26 F9            [ 1]   67 	jrne	00001$
      00801C                         68 00002$:
      00801C AE 00 01         [ 2]   69 	ldw	x, #l_INITIALIZER
      00801F 27 09            [ 1]   70 	jreq	00004$
      008021                         71 00003$:
      008021 D6 80 2C         [ 1]   72 	ld	a, (s_INITIALIZER - 1, x)
      008024 D7 00 00         [ 1]   73 	ld	(s_INITIALIZED - 1, x), a
      008027 5A               [ 2]   74 	decw	x
      008028 26 F7            [ 1]   75 	jrne	00003$
      00802A                         76 00004$:
                                     77 ; stm8_genXINIT() end
                                     78 	.area GSFINAL
      00802A CC 80 04         [ 2]   79 	jp	__sdcc_program_startup
                                     80 ;--------------------------------------------------------
                                     81 ; Home
                                     82 ;--------------------------------------------------------
                                     83 	.area HOME
                                     84 	.area HOME
      008004                         85 __sdcc_program_startup:
      008004 CC 80 2E         [ 2]   86 	jp	_main
                                     87 ;	return from main will return to caller
                                     88 ;--------------------------------------------------------
                                     89 ; code
                                     90 ;--------------------------------------------------------
                                     91 	.area CODE
                                     92 ;	main.c: 50: void main(void)
                                     93 ;	-----------------------------------------
                                     94 ;	 function main
                                     95 ;	-----------------------------------------
      00802E                         96 _main:
                                     97 ;	main.c: 55: GPIOE->DDR |= (1 << 5);
      00802E 72 1A 50 16      [ 1]   98 	bset	0x5016, #5
                                     99 ;	main.c: 56: GPIOE->ODR |= (1 << 5);
      008032 C6 50 14         [ 1]  100 	ld	a, 0x5014
      008035 AA 20            [ 1]  101 	or	a, #0x20
      008037 C7 50 14         [ 1]  102 	ld	0x5014, a
                                    103 ;	main.c: 63: while(1)
      00803A                        104 00102$:
                                    105 ;	main.c: 65: __asm__ ("WFI");
      00803A 8F               [10]  106 	WFI
      00803B 20 FD            [ 2]  107 	jra	00102$
                                    108 ;	main.c: 67: }
      00803D 81               [ 4]  109 	ret
                                    110 	.area CODE
                                    111 	.area CONST
                                    112 	.area INITIALIZER
      00802D                        113 __xinit__led:
      00802D 00                     114 	.db #0x00	; 0
                                    115 	.area CABS (ABS)
