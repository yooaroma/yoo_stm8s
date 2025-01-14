                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler 
                                      3 ; Version 4.4.1 #14792 (MINGW64)
                                      4 ;--------------------------------------------------------
                                      5 	.module stm8s_blink_main
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _main
                                     12 ;--------------------------------------------------------
                                     13 ; ram data
                                     14 ;--------------------------------------------------------
                                     15 	.area DATA
                                     16 ;--------------------------------------------------------
                                     17 ; ram data
                                     18 ;--------------------------------------------------------
                                     19 	.area INITIALIZED
                                     20 ;--------------------------------------------------------
                                     21 ; Stack segment in internal ram
                                     22 ;--------------------------------------------------------
                                     23 	.area SSEG
      000001                         24 __start__stack:
      000001                         25 	.ds	1
                                     26 
                                     27 ;--------------------------------------------------------
                                     28 ; absolute external ram data
                                     29 ;--------------------------------------------------------
                                     30 	.area DABS (ABS)
                                     31 
                                     32 ; default segment ordering for linker
                                     33 	.area HOME
                                     34 	.area GSINIT
                                     35 	.area GSFINAL
                                     36 	.area CONST
                                     37 	.area INITIALIZER
                                     38 	.area CODE
                                     39 
                                     40 ;--------------------------------------------------------
                                     41 ; interrupt vector
                                     42 ;--------------------------------------------------------
                                     43 	.area HOME
      008000                         44 __interrupt_vect:
      008000 82 00 80 07             45 	int s_GSINIT ; reset
                                     46 ;--------------------------------------------------------
                                     47 ; global & static initialisations
                                     48 ;--------------------------------------------------------
                                     49 	.area HOME
                                     50 	.area GSINIT
                                     51 	.area GSFINAL
                                     52 	.area GSINIT
      008007 CD 80 69         [ 4]   53 	call	___sdcc_external_startup
      00800A 4D               [ 1]   54 	tnz	a
      00800B 27 03            [ 1]   55 	jreq	__sdcc_init_data
      00800D CC 80 04         [ 2]   56 	jp	__sdcc_program_startup
      008010                         57 __sdcc_init_data:
                                     58 ; stm8_genXINIT() start
      008010 AE 00 00         [ 2]   59 	ldw x, #l_DATA
      008013 27 07            [ 1]   60 	jreq	00002$
      008015                         61 00001$:
      008015 72 4F 00 00      [ 1]   62 	clr (s_DATA - 1, x)
      008019 5A               [ 2]   63 	decw x
      00801A 26 F9            [ 1]   64 	jrne	00001$
      00801C                         65 00002$:
      00801C AE 00 00         [ 2]   66 	ldw	x, #l_INITIALIZER
      00801F 27 09            [ 1]   67 	jreq	00004$
      008021                         68 00003$:
      008021 D6 80 2C         [ 1]   69 	ld	a, (s_INITIALIZER - 1, x)
      008024 D7 00 00         [ 1]   70 	ld	(s_INITIALIZED - 1, x), a
      008027 5A               [ 2]   71 	decw	x
      008028 26 F7            [ 1]   72 	jrne	00003$
      00802A                         73 00004$:
                                     74 ; stm8_genXINIT() end
                                     75 	.area GSFINAL
      00802A CC 80 04         [ 2]   76 	jp	__sdcc_program_startup
                                     77 ;--------------------------------------------------------
                                     78 ; Home
                                     79 ;--------------------------------------------------------
                                     80 	.area HOME
                                     81 	.area HOME
      008004                         82 __sdcc_program_startup:
      008004 CC 80 2D         [ 2]   83 	jp	_main
                                     84 ;	return from main will return to caller
                                     85 ;--------------------------------------------------------
                                     86 ; code
                                     87 ;--------------------------------------------------------
                                     88 	.area CODE
                                     89 ;	stm8s_blink_main.c: 57: void main(void)
                                     90 ;	-----------------------------------------
                                     91 ;	 function main
                                     92 ;	-----------------------------------------
      00802D                         93 _main:
                                     94 ;	stm8s_blink_main.c: 63: CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);    
      00802D C6 50 C6         [ 1]   95 	ld	a, 0x50c6
      008030 A4 E7            [ 1]   96 	and	a, #0xe7
      008032 C7 50 C6         [ 1]   97 	ld	0x50c6, a
                                     98 ;	stm8s_blink_main.c: 65: CLK->CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV1;
      008035 55 50 C6 50 C6   [ 1]   99 	mov	0x50c6, 0x50c6
                                    100 ;	stm8s_blink_main.c: 74: UCOM_LED1_GPIO->ODR &= (uint8_t)(~(UCOM_LED1_PIN)); /* Output mode  Low level */
      00803A 72 11 50 0F      [ 1]  101 	bres	0x500f, #0
                                    102 ;	stm8s_blink_main.c: 75: UCOM_LED1_GPIO->DDR |= (uint8_t)UCOM_LED1_PIN; /* Set Output mode */
      00803E 72 10 50 11      [ 1]  103 	bset	0x5011, #0
                                    104 ;	stm8s_blink_main.c: 76: UCOM_LED1_GPIO->CR1 |= (uint8_t)UCOM_LED1_PIN; /* Push-Pull */
      008042 72 10 50 12      [ 1]  105 	bset	0x5012, #0
                                    106 ;	stm8s_blink_main.c: 78: UCOM_LED1_GPIO->CR2 |= (uint8_t)UCOM_LED1_PIN; /* 10MHz */
      008046 72 10 50 13      [ 1]  107 	bset	0x5013, #0
                                    108 ;	stm8s_blink_main.c: 83: uint32_t vdwDelay = 0;
      00804A 5F               [ 1]  109 	clrw	x
      00804B 90 5F            [ 1]  110 	clrw	y
                                    111 ;	stm8s_blink_main.c: 84: do {
      00804D                        112 00104$:
                                    113 ;	stm8s_blink_main.c: 85: if(vdwDelay)
      00804D 5D               [ 2]  114 	tnzw	x
      00804E 26 04            [ 1]  115 	jrne	00123$
      008050 90 5D            [ 2]  116 	tnzw	y
      008052 27 09            [ 1]  117 	jreq	00102$
      008054                        118 00123$:
                                    119 ;	stm8s_blink_main.c: 87: vdwDelay--;
      008054 1D 00 01         [ 2]  120 	subw	x, #0x0001
      008057 24 F4            [ 1]  121 	jrnc	00104$
      008059 90 5A            [ 2]  122 	decw	y
      00805B 20 F0            [ 2]  123 	jra	00104$
      00805D                        124 00102$:
                                    125 ;	stm8s_blink_main.c: 91: vdwDelay = 0x20000*5;
      00805D 5F               [ 1]  126 	clrw	x
      00805E 90 AE 00 0A      [ 2]  127 	ldw	y, #0x000a
                                    128 ;	stm8s_blink_main.c: 93: UCOM_LED1_GPIO->ODR ^= (uint8_t)UCOM_LED1_PIN;
      008062 90 10 50 0F      [ 1]  129 	bcpl	0x500f, #0
                                    130 ;	stm8s_blink_main.c: 95: } while(1);
      008066 20 E5            [ 2]  131 	jra	00104$
                                    132 ;	stm8s_blink_main.c: 97: }
      008068 81               [ 4]  133 	ret
                                    134 	.area CODE
                                    135 	.area CONST
                                    136 	.area INITIALIZER
                                    137 	.area CABS (ABS)
