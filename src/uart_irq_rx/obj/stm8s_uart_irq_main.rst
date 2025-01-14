                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler 
                                      3 ; Version 4.4.0 #14620 (MINGW32)
                                      4 ;--------------------------------------------------------
                                      5 	.module stm8s_uart_irq_main
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _main
                                     12 	.globl _MibReadDebugByte
                                     13 	.globl _MibWriteDebugString
                                     14 	.globl _MibWriteDebugByte
                                     15 	.globl _MibDebugInit
                                     16 	.globl _GPIO_WriteReverse
                                     17 	.globl _GPIO_Init
                                     18 	.globl _GPIO_DeInit
                                     19 	.globl _CLK_HSIPrescalerConfig
                                     20 	.globl _CLK_Configuration
                                     21 	.globl _GPIO_Configuration
                                     22 	.globl _Toggle
                                     23 ;--------------------------------------------------------
                                     24 ; ram data
                                     25 ;--------------------------------------------------------
                                     26 	.area DATA
                                     27 ;--------------------------------------------------------
                                     28 ; ram data
                                     29 ;--------------------------------------------------------
                                     30 	.area INITIALIZED
                                     31 ;--------------------------------------------------------
                                     32 ; Stack segment in internal ram
                                     33 ;--------------------------------------------------------
                                     34 	.area SSEG
      000045                         35 __start__stack:
      000045                         36 	.ds	1
                                     37 
                                     38 ;--------------------------------------------------------
                                     39 ; absolute external ram data
                                     40 ;--------------------------------------------------------
                                     41 	.area DABS (ABS)
                                     42 
                                     43 ; default segment ordering for linker
                                     44 	.area HOME
                                     45 	.area GSINIT
                                     46 	.area GSFINAL
                                     47 	.area CONST
                                     48 	.area INITIALIZER
                                     49 	.area CODE
                                     50 
                                     51 ;--------------------------------------------------------
                                     52 ; interrupt vector
                                     53 ;--------------------------------------------------------
                                     54 	.area HOME
      008000                         55 __interrupt_vect:
      008000 82 00 80 6F             56 	int s_GSINIT ; reset
      008004 82 00 87 7B             57 	int _TRAP_IRQHandler ; trap
      008008 82 00 87 7C             58 	int _TLI_IRQHandler ; int0
      00800C 82 00 87 7D             59 	int _AWU_IRQHandler ; int1
      008010 82 00 87 7E             60 	int _CLK_IRQHandler ; int2
      008014 82 00 87 7F             61 	int _EXTI_PORTA_IRQHandler ; int3
      008018 82 00 87 80             62 	int _EXTI_PORTB_IRQHandler ; int4
      00801C 82 00 87 81             63 	int _EXTI_PORTC_IRQHandler ; int5
      008020 82 00 87 82             64 	int _EXTI_PORTD_IRQHandler ; int6
      008024 82 00 87 83             65 	int _EXTI_PORTE_IRQHandler ; int7
      008028 82 00 00 00             66 	int 0x000000 ; int8
      00802C 82 00 00 00             67 	int 0x000000 ; int9
      008030 82 00 87 84             68 	int _SPI_IRQHandler ; int10
      008034 82 00 87 85             69 	int _TIM1_UPD_OVF_TRG_BRK_IRQHandler ; int11
      008038 82 00 87 86             70 	int _TIM1_CAP_COM_IRQHandler ; int12
      00803C 82 00 87 87             71 	int _TIM2_UPD_OVF_BRK_IRQHandler ; int13
      008040 82 00 87 88             72 	int _TIM2_CAP_COM_IRQHandler ; int14
      008044 82 00 87 89             73 	int _TIM3_UPD_OVF_BRK_IRQHandler ; int15
      008048 82 00 87 8A             74 	int _TIM3_CAP_COM_IRQHandler ; int16
      00804C 82 00 00 00             75 	int 0x000000 ; int17
      008050 82 00 00 00             76 	int 0x000000 ; int18
      008054 82 00 87 8B             77 	int _I2C_IRQHandler ; int19
      008058 82 00 87 8C             78 	int _UART2_TX_IRQHandler ; int20
      00805C 82 00 87 8D             79 	int _UART2_RX_IRQHandler ; int21
      008060 82 00 87 93             80 	int _ADC1_IRQHandler ; int22
      008064 82 00 87 94             81 	int _TIM4_UPD_OVF_IRQHandler ; int23
      008068 82 00 87 95             82 	int _EEPROM_EEC_IRQHandler ; int24
                                     83 ;--------------------------------------------------------
                                     84 ; global & static initialisations
                                     85 ;--------------------------------------------------------
                                     86 	.area HOME
                                     87 	.area GSINIT
                                     88 	.area GSFINAL
                                     89 	.area GSINIT
      00806F CD 8A 1B         [ 4]   90 	call	___sdcc_external_startup
      008072 4D               [ 1]   91 	tnz	a
      008073 27 03            [ 1]   92 	jreq	__sdcc_init_data
      008075 CC 80 6C         [ 2]   93 	jp	__sdcc_program_startup
      008078                         94 __sdcc_init_data:
                                     95 ; stm8_genXINIT() start
      008078 AE 00 42         [ 2]   96 	ldw x, #l_DATA
      00807B 27 07            [ 1]   97 	jreq	00002$
      00807D                         98 00001$:
      00807D 72 4F 00 00      [ 1]   99 	clr (s_DATA - 1, x)
      008081 5A               [ 2]  100 	decw x
      008082 26 F9            [ 1]  101 	jrne	00001$
      008084                        102 00002$:
      008084 AE 00 02         [ 2]  103 	ldw	x, #l_INITIALIZER
      008087 27 09            [ 1]  104 	jreq	00004$
      008089                        105 00003$:
      008089 D6 81 6E         [ 1]  106 	ld	a, (s_INITIALIZER - 1, x)
      00808C D7 00 42         [ 1]  107 	ld	(s_INITIALIZED - 1, x), a
      00808F 5A               [ 2]  108 	decw	x
      008090 26 F7            [ 1]  109 	jrne	00003$
      008092                        110 00004$:
                                    111 ; stm8_genXINIT() end
                                    112 	.area GSFINAL
      008092 CC 80 6C         [ 2]  113 	jp	__sdcc_program_startup
                                    114 ;--------------------------------------------------------
                                    115 ; Home
                                    116 ;--------------------------------------------------------
                                    117 	.area HOME
                                    118 	.area HOME
      00806C                        119 __sdcc_program_startup:
      00806C CC 81 71         [ 2]  120 	jp	_main
                                    121 ;	return from main will return to caller
                                    122 ;--------------------------------------------------------
                                    123 ; code
                                    124 ;--------------------------------------------------------
                                    125 	.area CODE
                                    126 ;	stm8s_uart_irq_main.c: 46: void main(void)
                                    127 ;	-----------------------------------------
                                    128 ;	 function main
                                    129 ;	-----------------------------------------
      008171                        130 _main:
      008171 52 04            [ 2]  131 	sub	sp, #4
                                    132 ;	stm8s_uart_irq_main.c: 50: CLK_Configuration();
      008173 CD 81 E8         [ 4]  133 	call	_CLK_Configuration
                                    134 ;	stm8s_uart_irq_main.c: 53: GPIO_Configuration();
      008176 CD 81 EC         [ 4]  135 	call	_GPIO_Configuration
                                    136 ;	stm8s_uart_irq_main.c: 56: MibDebugInit(9600);
      008179 4B 80            [ 1]  137 	push	#0x80
      00817B 4B 25            [ 1]  138 	push	#0x25
      00817D 5F               [ 1]  139 	clrw	x
      00817E 89               [ 2]  140 	pushw	x
      00817F CD 87 F7         [ 4]  141 	call	_MibDebugInit
                                    142 ;	stm8s_uart_irq_main.c: 59: enableInterrupts();
      008182 9A               [ 1]  143 	rim
                                    144 ;	stm8s_uart_irq_main.c: 62: MibWriteDebugStringCheck(1,"\r\n ###################################\r\n");
      008183 AE 80 95         [ 2]  145 	ldw	x, #(__str_0+0)
      008186 CD 87 B3         [ 4]  146 	call	_MibWriteDebugString
                                    147 ;	stm8s_uart_irq_main.c: 63: MibWriteDebugStringCheck(1," file name : " __FILE__ "\r\n");
      008189 AE 80 BE         [ 2]  148 	ldw	x, #(__str_1+0)
      00818C CD 87 B3         [ 4]  149 	call	_MibWriteDebugString
                                    150 ;	stm8s_uart_irq_main.c: 64: MibWriteDebugStringCheck(1," date :  "__DATE__"  :  "__TIME__"\r\n");
      00818F AE 80 E3         [ 2]  151 	ldw	x, #(__str_2+0)
      008192 CD 87 B3         [ 4]  152 	call	_MibWriteDebugString
                                    153 ;	stm8s_uart_irq_main.c: 65: MibWriteDebugStringCheck(1," webgpio.com by MYMEDIA Co., Ltd.\r\n");
      008195 AE 81 07         [ 2]  154 	ldw	x, #(__str_3+0)
      008198 CD 87 B3         [ 4]  155 	call	_MibWriteDebugString
                                    156 ;	stm8s_uart_irq_main.c: 66: MibWriteDebugStringCheck(1," ###################################\r\n");
      00819B AE 81 2B         [ 2]  157 	ldw	x, #(__str_4+0)
      00819E CD 87 B3         [ 4]  158 	call	_MibWriteDebugString
                                    159 ;	stm8s_uart_irq_main.c: 70: uint32_t vdwDelay = 0;
      0081A1 5F               [ 1]  160 	clrw	x
      0081A2 1F 03            [ 2]  161 	ldw	(0x03, sp), x
      0081A4 1F 01            [ 2]  162 	ldw	(0x01, sp), x
                                    163 ;	stm8s_uart_irq_main.c: 71: do {
      0081A6                        164 00109$:
                                    165 ;	stm8s_uart_irq_main.c: 72: if((UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN)==0)
      0081A6 72 0A 50 06 06   [ 2]  166 	btjt	0x5006, #5, 00105$
                                    167 ;	stm8s_uart_irq_main.c: 75: UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR | UCOM_LED1_PIN; // LED off
      0081AB 72 10 50 0F      [ 1]  168 	bset	0x500f, #0
      0081AF 20 25            [ 2]  169 	jra	00106$
      0081B1                        170 00105$:
                                    171 ;	stm8s_uart_irq_main.c: 79: if(vdwDelay)
      0081B1 1E 03            [ 2]  172 	ldw	x, (0x03, sp)
      0081B3 26 04            [ 1]  173 	jrne	00143$
      0081B5 1E 01            [ 2]  174 	ldw	x, (0x01, sp)
      0081B7 27 10            [ 1]  175 	jreq	00102$
      0081B9                        176 00143$:
                                    177 ;	stm8s_uart_irq_main.c: 81: vdwDelay--;
      0081B9 1E 03            [ 2]  178 	ldw	x, (0x03, sp)
      0081BB 1D 00 01         [ 2]  179 	subw	x, #0x0001
      0081BE 1F 03            [ 2]  180 	ldw	(0x03, sp), x
      0081C0 1E 01            [ 2]  181 	ldw	x, (0x01, sp)
      0081C2 24 01            [ 1]  182 	jrnc	00144$
      0081C4 5A               [ 2]  183 	decw	x
      0081C5                        184 00144$:
      0081C5 1F 01            [ 2]  185 	ldw	(0x01, sp), x
      0081C7 20 0D            [ 2]  186 	jra	00106$
      0081C9                        187 00102$:
                                    188 ;	stm8s_uart_irq_main.c: 85: vdwDelay = 320000;
      0081C9 AE E2 00         [ 2]  189 	ldw	x, #0xe200
      0081CC 1F 03            [ 2]  190 	ldw	(0x03, sp), x
      0081CE AE 00 04         [ 2]  191 	ldw	x, #0x0004
      0081D1 1F 01            [ 2]  192 	ldw	(0x01, sp), x
                                    193 ;	stm8s_uart_irq_main.c: 86: Toggle();
      0081D3 CD 82 0D         [ 4]  194 	call	_Toggle
      0081D6                        195 00106$:
                                    196 ;	stm8s_uart_irq_main.c: 89: vwCh = MibReadDebugByte();
      0081D6 CD 87 D4         [ 4]  197 	call	_MibReadDebugByte
                                    198 ;	stm8s_uart_irq_main.c: 90: if(vwCh!=MIB_DEBUG_READ_NODATA)
      0081D9 90 93            [ 1]  199 	ldw	y, x
      0081DB 90 5C            [ 1]  200 	incw	y
      0081DD 27 C7            [ 1]  201 	jreq	00109$
                                    202 ;	stm8s_uart_irq_main.c: 92: MibWriteDebugByte(vwCh);
      0081DF 9F               [ 1]  203 	ld	a, xl
      0081E0 CD 87 96         [ 4]  204 	call	_MibWriteDebugByte
                                    205 ;	stm8s_uart_irq_main.c: 94: } while(1);
      0081E3 20 C1            [ 2]  206 	jra	00109$
                                    207 ;	stm8s_uart_irq_main.c: 96: }
      0081E5 5B 04            [ 2]  208 	addw	sp, #4
      0081E7 81               [ 4]  209 	ret
                                    210 ;	stm8s_uart_irq_main.c: 108: void CLK_Configuration(void)
                                    211 ;	-----------------------------------------
                                    212 ;	 function CLK_Configuration
                                    213 ;	-----------------------------------------
      0081E8                        214 _CLK_Configuration:
                                    215 ;	stm8s_uart_irq_main.c: 112: CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
      0081E8 4F               [ 1]  216 	clr	a
                                    217 ;	stm8s_uart_irq_main.c: 114: }
      0081E9 CC 85 D0         [ 2]  218 	jp	_CLK_HSIPrescalerConfig
                                    219 ;	stm8s_uart_irq_main.c: 126: void GPIO_Configuration(void)
                                    220 ;	-----------------------------------------
                                    221 ;	 function GPIO_Configuration
                                    222 ;	-----------------------------------------
      0081EC                        223 _GPIO_Configuration:
                                    224 ;	stm8s_uart_irq_main.c: 129: GPIO_DeInit(UCOM_LED1_GPIO);
      0081EC AE 50 0F         [ 2]  225 	ldw	x, #0x500f
      0081EF CD 82 15         [ 4]  226 	call	_GPIO_DeInit
                                    227 ;	stm8s_uart_irq_main.c: 131: GPIO_Init(UCOM_LED1_GPIO, UCOM_LED1_PIN, UCOM_LED1_MODE);
      0081F2 4B E0            [ 1]  228 	push	#0xe0
      0081F4 A6 01            [ 1]  229 	ld	a, #0x01
      0081F6 AE 50 0F         [ 2]  230 	ldw	x, #0x500f
      0081F9 CD 82 23         [ 4]  231 	call	_GPIO_Init
                                    232 ;	stm8s_uart_irq_main.c: 133: GPIO_DeInit(UCOM_KEY1_GPIO);
      0081FC AE 50 05         [ 2]  233 	ldw	x, #0x5005
      0081FF CD 82 15         [ 4]  234 	call	_GPIO_DeInit
                                    235 ;	stm8s_uart_irq_main.c: 135: GPIO_Init(UCOM_KEY1_GPIO, UCOM_KEY1_PIN, UCOM_KEY1_MODE);
      008202 4B 40            [ 1]  236 	push	#0x40
      008204 A6 20            [ 1]  237 	ld	a, #0x20
      008206 AE 50 05         [ 2]  238 	ldw	x, #0x5005
      008209 CD 82 23         [ 4]  239 	call	_GPIO_Init
                                    240 ;	stm8s_uart_irq_main.c: 136: }
      00820C 81               [ 4]  241 	ret
                                    242 ;	stm8s_uart_irq_main.c: 148: void Toggle(void)
                                    243 ;	-----------------------------------------
                                    244 ;	 function Toggle
                                    245 ;	-----------------------------------------
      00820D                        246 _Toggle:
                                    247 ;	stm8s_uart_irq_main.c: 150: GPIO_WriteReverse(UCOM_LED1_GPIO, UCOM_LED1_PIN);
      00820D A6 01            [ 1]  248 	ld	a, #0x01
      00820F AE 50 0F         [ 2]  249 	ldw	x, #0x500f
                                    250 ;	stm8s_uart_irq_main.c: 151: }
      008212 CC 82 B2         [ 2]  251 	jp	_GPIO_WriteReverse
                                    252 	.area CODE
                                    253 	.area CONST
                                    254 	.area CONST
      008095                        255 __str_0:
      008095 0D                     256 	.db 0x0d
      008096 0A                     257 	.db 0x0a
      008097 20 23 23 23 23 23 23   258 	.ascii " ###################################"
             23 23 23 23 23 23 23
             23 23 23 23 23 23 23
             23 23 23 23 23 23 23
             23 23 23 23 23 23 23
             23
      0080BB 0D                     259 	.db 0x0d
      0080BC 0A                     260 	.db 0x0a
      0080BD 00                     261 	.db 0x00
                                    262 	.area CODE
                                    263 	.area CONST
      0080BE                        264 __str_1:
      0080BE 20 66 69 6C 65 20 6E   265 	.ascii " file name : stm8s_uart_irq_main.c"
             61 6D 65 20 3A 20 73
             74 6D 38 73 5F 75 61
             72 74 5F 69 72 71 5F
             6D 61 69 6E 2E 63
      0080E0 0D                     266 	.db 0x0d
      0080E1 0A                     267 	.db 0x0a
      0080E2 00                     268 	.db 0x00
                                    269 	.area CODE
                                    270 	.area CONST
      0080E3                        271 __str_2:
      0080E3 20 64 61 74 65 20 3A   272 	.ascii " date :  Dec  4 2024  :  12:23:03"
             20 20 44 65 63 20 20
             34 20 32 30 32 34 20
             20 3A 20 20 31 32 3A
             32 33 3A 30 33
      008104 0D                     273 	.db 0x0d
      008105 0A                     274 	.db 0x0a
      008106 00                     275 	.db 0x00
                                    276 	.area CODE
                                    277 	.area CONST
      008107                        278 __str_3:
      008107 20 77 65 62 67 70 69   279 	.ascii " webgpio.com by MYMEDIA Co., Ltd."
             6F 2E 63 6F 6D 20 62
             79 20 4D 59 4D 45 44
             49 41 20 43 6F 2E 2C
             20 4C 74 64 2E
      008128 0D                     280 	.db 0x0d
      008129 0A                     281 	.db 0x0a
      00812A 00                     282 	.db 0x00
                                    283 	.area CODE
                                    284 	.area CONST
      00812B                        285 __str_4:
      00812B 20 23 23 23 23 23 23   286 	.ascii " ###################################"
             23 23 23 23 23 23 23
             23 23 23 23 23 23 23
             23 23 23 23 23 23 23
             23 23 23 23 23 23 23
             23
      00814F 0D                     287 	.db 0x0d
      008150 0A                     288 	.db 0x0a
      008151 00                     289 	.db 0x00
                                    290 	.area CODE
                                    291 	.area INITIALIZER
                                    292 	.area CABS (ABS)
