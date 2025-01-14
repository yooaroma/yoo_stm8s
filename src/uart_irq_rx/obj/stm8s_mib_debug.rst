                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler 
                                      3 ; Version 4.4.0 #14620 (MINGW32)
                                      4 ;--------------------------------------------------------
                                      5 	.module stm8s_mib_debug
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _UART_RX_IRQHandler
                                     12 	.globl _MibWriteDebugByte
                                     13 	.globl _MibWriteDebugByte485
                                     14 	.globl _MibWriteDebugEmptyCheck
                                     15 	.globl _MibWriteDebugString
                                     16 	.globl _MibWriteDebugStringN
                                     17 	.globl _MibReadDebugByte
                                     18 	.globl _MibDebugInit
                                     19 	.globl _MibWriteDebugTransmitHex
                                     20 	.globl _MibWriteDebugTransmitHex2
                                     21 ;--------------------------------------------------------
                                     22 ; ram data
                                     23 ;--------------------------------------------------------
                                     24 	.area DATA
      000001                         25 _rx_buffer:
      000001                         26 	.ds 64
      000041                         27 _rx_buffer_head:
      000041                         28 	.ds 1
      000042                         29 _rx_buffer_tail:
      000042                         30 	.ds 1
                                     31 ;--------------------------------------------------------
                                     32 ; ram data
                                     33 ;--------------------------------------------------------
                                     34 	.area INITIALIZED
      000043                         35 _g_TableHex:
      000043                         36 	.ds 2
                                     37 ;--------------------------------------------------------
                                     38 ; absolute external ram data
                                     39 ;--------------------------------------------------------
                                     40 	.area DABS (ABS)
                                     41 
                                     42 ; default segment ordering for linker
                                     43 	.area HOME
                                     44 	.area GSINIT
                                     45 	.area GSFINAL
                                     46 	.area CONST
                                     47 	.area INITIALIZER
                                     48 	.area CODE
                                     49 
                                     50 ;--------------------------------------------------------
                                     51 ; global & static initialisations
                                     52 ;--------------------------------------------------------
                                     53 	.area HOME
                                     54 	.area GSINIT
                                     55 	.area GSFINAL
                                     56 	.area GSINIT
                                     57 ;--------------------------------------------------------
                                     58 ; Home
                                     59 ;--------------------------------------------------------
                                     60 	.area HOME
                                     61 	.area HOME
                                     62 ;--------------------------------------------------------
                                     63 ; code
                                     64 ;--------------------------------------------------------
                                     65 	.area CODE
                                     66 ;	./../../mib/stm8s_mib_debug.c: 206: void MibWriteDebugByte(uint8_t ch)
                                     67 ;	-----------------------------------------
                                     68 ;	 function MibWriteDebugByte
                                     69 ;	-----------------------------------------
      008796                         70 _MibWriteDebugByte:
      008796 88               [ 1]   71 	push	a
      008797 6B 01            [ 1]   72 	ld	(0x01, sp), a
                                     73 ;	./../../mib/stm8s_mib_debug.c: 227: while (!(UARTXdebug->SR & UARTX_SR_TXE));
      008799                         74 00101$:
      008799 C6 52 40         [ 1]   75 	ld	a, 0x5240
      00879C 2A FB            [ 1]   76 	jrpl	00101$
                                     77 ;	./../../mib/stm8s_mib_debug.c: 228: UARTXdebug->DR = ch;
      00879E AE 52 41         [ 2]   78 	ldw	x, #0x5241
      0087A1 7B 01            [ 1]   79 	ld	a, (0x01, sp)
      0087A3 F7               [ 1]   80 	ld	(x), a
                                     81 ;	./../../mib/stm8s_mib_debug.c: 231: }
      0087A4 84               [ 1]   82 	pop	a
      0087A5 81               [ 4]   83 	ret
                                     84 ;	./../../mib/stm8s_mib_debug.c: 240: void MibWriteDebugByte485(uint8_t ch)
                                     85 ;	-----------------------------------------
                                     86 ;	 function MibWriteDebugByte485
                                     87 ;	-----------------------------------------
      0087A6                         88 _MibWriteDebugByte485:
                                     89 ;	./../../mib/stm8s_mib_debug.c: 248: MibWriteDebugByte(ch);
                                     90 ;	./../../mib/stm8s_mib_debug.c: 257: }
      0087A6 CC 87 96         [ 2]   91 	jp	_MibWriteDebugByte
                                     92 ;	./../../mib/stm8s_mib_debug.c: 266: void MibWriteDebugEmptyCheck(void)
                                     93 ;	-----------------------------------------
                                     94 ;	 function MibWriteDebugEmptyCheck
                                     95 ;	-----------------------------------------
      0087A9                         96 _MibWriteDebugEmptyCheck:
                                     97 ;	./../../mib/stm8s_mib_debug.c: 269: while (!(UARTXdebug->SR & UARTX_SR_TC));
      0087A9                         98 00101$:
      0087A9 72 0D 52 40 FB   [ 2]   99 	btjf	0x5240, #6, 00101$
                                    100 ;	./../../mib/stm8s_mib_debug.c: 270: UARTXdebug->SR &= ~UARTX_SR_TC;
      0087AE 72 1D 52 40      [ 1]  101 	bres	0x5240, #6
                                    102 ;	./../../mib/stm8s_mib_debug.c: 272: }
      0087B2 81               [ 4]  103 	ret
                                    104 ;	./../../mib/stm8s_mib_debug.c: 281: void MibWriteDebugString(uint8_t *v_pStr)
                                    105 ;	-----------------------------------------
                                    106 ;	 function MibWriteDebugString
                                    107 ;	-----------------------------------------
      0087B3                        108 _MibWriteDebugString:
                                    109 ;	./../../mib/stm8s_mib_debug.c: 288: while (*v_pStr)
      0087B3                        110 00101$:
      0087B3 F6               [ 1]  111 	ld	a, (x)
      0087B4 26 01            [ 1]  112 	jrne	00121$
      0087B6 81               [ 4]  113 	ret
      0087B7                        114 00121$:
                                    115 ;	./../../mib/stm8s_mib_debug.c: 290: MibWriteDebugByte(*v_pStr++);
      0087B7 5C               [ 1]  116 	incw	x
      0087B8 89               [ 2]  117 	pushw	x
      0087B9 CD 87 96         [ 4]  118 	call	_MibWriteDebugByte
      0087BC 85               [ 2]  119 	popw	x
      0087BD 20 F4            [ 2]  120 	jra	00101$
                                    121 ;	./../../mib/stm8s_mib_debug.c: 299: }
      0087BF 81               [ 4]  122 	ret
                                    123 ;	./../../mib/stm8s_mib_debug.c: 308: void MibWriteDebugStringN(uint8_t *v_pStr, uint8_t nSize)
                                    124 ;	-----------------------------------------
                                    125 ;	 function MibWriteDebugStringN
                                    126 ;	-----------------------------------------
      0087C0                        127 _MibWriteDebugStringN:
      0087C0 88               [ 1]  128 	push	a
      0087C1 6B 01            [ 1]  129 	ld	(0x01, sp), a
                                    130 ;	./../../mib/stm8s_mib_debug.c: 315: while (1)
      0087C3                        131 00104$:
                                    132 ;	./../../mib/stm8s_mib_debug.c: 317: if (nSize == 0)	break;		
      0087C3 0D 01            [ 1]  133 	tnz	(0x01, sp)
      0087C5 27 0B            [ 1]  134 	jreq	00106$
                                    135 ;	./../../mib/stm8s_mib_debug.c: 318: MibWriteDebugByte(*v_pStr++);
      0087C7 F6               [ 1]  136 	ld	a, (x)
      0087C8 5C               [ 1]  137 	incw	x
      0087C9 89               [ 2]  138 	pushw	x
      0087CA CD 87 96         [ 4]  139 	call	_MibWriteDebugByte
      0087CD 85               [ 2]  140 	popw	x
                                    141 ;	./../../mib/stm8s_mib_debug.c: 319: nSize--;
      0087CE 0A 01            [ 1]  142 	dec	(0x01, sp)
      0087D0 20 F1            [ 2]  143 	jra	00104$
      0087D2                        144 00106$:
                                    145 ;	./../../mib/stm8s_mib_debug.c: 328: }
      0087D2 84               [ 1]  146 	pop	a
      0087D3 81               [ 4]  147 	ret
                                    148 ;	./../../mib/stm8s_mib_debug.c: 337: uint16_t MibReadDebugByte( void )
                                    149 ;	-----------------------------------------
                                    150 ;	 function MibReadDebugByte
                                    151 ;	-----------------------------------------
      0087D4                        152 _MibReadDebugByte:
                                    153 ;	./../../mib/stm8s_mib_debug.c: 344: if (rx_buffer_head == rx_buffer_tail) // wait for character
      0087D4 C6 00 42         [ 1]  154 	ld	a, _rx_buffer_tail+0
      0087D7 C1 00 41         [ 1]  155 	cp	a, _rx_buffer_head+0
      0087DA 26 03            [ 1]  156 	jrne	00102$
                                    157 ;	./../../mib/stm8s_mib_debug.c: 346: return MIB_DEBUG_READ_NODATA; // no data
      0087DC 5F               [ 1]  158 	clrw	x
      0087DD 5A               [ 2]  159 	decw	x
      0087DE 81               [ 4]  160 	ret
      0087DF                        161 00102$:
                                    162 ;	./../../mib/stm8s_mib_debug.c: 348: vbIndex = rx_buffer_tail + 1;
      0087DF C6 00 42         [ 1]  163 	ld	a, _rx_buffer_tail+0
      0087E2 4C               [ 1]  164 	inc	a
                                    165 ;	./../../mib/stm8s_mib_debug.c: 349: if (vbIndex >= RX_BUFFER_SIZE) vbIndex = 0;
      0087E3 A1 40            [ 1]  166 	cp	a, #0x40
      0087E5 25 01            [ 1]  167 	jrc	00104$
      0087E7 4F               [ 1]  168 	clr	a
      0087E8                        169 00104$:
                                    170 ;	./../../mib/stm8s_mib_debug.c: 350: vbCh = rx_buffer[vbIndex];
      0087E8 5F               [ 1]  171 	clrw	x
      0087E9 97               [ 1]  172 	ld	xl, a
      0087EA 1C 00 01         [ 2]  173 	addw	x, #(_rx_buffer+0)
      0087ED 88               [ 1]  174 	push	a
      0087EE F6               [ 1]  175 	ld	a, (x)
      0087EF 97               [ 1]  176 	ld	xl, a
      0087F0 84               [ 1]  177 	pop	a
                                    178 ;	./../../mib/stm8s_mib_debug.c: 351: rx_buffer_tail = vbIndex;
      0087F1 C7 00 42         [ 1]  179 	ld	_rx_buffer_tail+0, a
                                    180 ;	./../../mib/stm8s_mib_debug.c: 352: return (uint16_t)vbCh;
      0087F4 4F               [ 1]  181 	clr	a
      0087F5 95               [ 1]  182 	ld	xh, a
                                    183 ;	./../../mib/stm8s_mib_debug.c: 367: }
      0087F6 81               [ 4]  184 	ret
                                    185 ;	./../../mib/stm8s_mib_debug.c: 442: void MibDebugInit(uint32_t baudrate)
                                    186 ;	-----------------------------------------
                                    187 ;	 function MibDebugInit
                                    188 ;	-----------------------------------------
      0087F7                        189 _MibDebugInit:
      0087F7 52 0C            [ 2]  190 	sub	sp, #12
                                    191 ;	./../../mib/stm8s_mib_debug.c: 446: rx_buffer_head = 0;
      0087F9 72 5F 00 41      [ 1]  192 	clr	_rx_buffer_head+0
                                    193 ;	./../../mib/stm8s_mib_debug.c: 447: rx_buffer_tail = 0;
      0087FD 72 5F 00 42      [ 1]  194 	clr	_rx_buffer_tail+0
                                    195 ;	./../../mib/stm8s_mib_debug.c: 480: uint32_t BaudRate = baudrate;
      008801 1E 11            [ 2]  196 	ldw	x, (0x11, sp)
      008803 16 0F            [ 2]  197 	ldw	y, (0x0f, sp)
                                    198 ;	./../../mib/stm8s_mib_debug.c: 486: UARTXdebug->CR1 &= (uint8_t)(~UARTX_CR1_M);			 /**< Clear the word length bit */
      008805 72 19 52 44      [ 1]  199 	bres	0x5244, #4
                                    200 ;	./../../mib/stm8s_mib_debug.c: 487: UARTXdebug->CR1 |= (uint8_t)UARTX_WORDLENGTH_8D; /**< Set the word length bit according to UART1_WordLength value */
      008809 55 52 44 52 44   [ 1]  201 	mov	0x5244, 0x5244
                                    202 ;	./../../mib/stm8s_mib_debug.c: 489: UARTXdebug->CR3 &= (uint8_t)(~UARTX_CR3_STOP); /**< Clear the STOP bits */
      00880E C6 52 46         [ 1]  203 	ld	a, 0x5246
      008811 A4 CF            [ 1]  204 	and	a, #0xcf
      008813 C7 52 46         [ 1]  205 	ld	0x5246, a
                                    206 ;	./../../mib/stm8s_mib_debug.c: 490: UARTXdebug->CR3 |= (uint8_t)UARTX_STOPBITS_1;	 /**< Set the STOP bits number according to UART1_StopBits value  */
      008816 55 52 46 52 46   [ 1]  207 	mov	0x5246, 0x5246
                                    208 ;	./../../mib/stm8s_mib_debug.c: 492: UARTXdebug->CR1 &= (uint8_t)(~(UARTX_CR1_PCEN | UARTX_CR1_PS)); /**< Clear the Parity Control bit */
      00881B C6 52 44         [ 1]  209 	ld	a, 0x5244
      00881E A4 F9            [ 1]  210 	and	a, #0xf9
      008820 C7 52 44         [ 1]  211 	ld	0x5244, a
                                    212 ;	./../../mib/stm8s_mib_debug.c: 493: UARTXdebug->CR1 |= (uint8_t)UARTX_PARITY_NO;										/**< Set the Parity Control bit to UART1_Parity value */
      008823 55 52 44 52 44   [ 1]  213 	mov	0x5244, 0x5244
                                    214 ;	./../../mib/stm8s_mib_debug.c: 495: UARTXdebug->BRR1 &= (uint8_t)(~UARTX_BRR1_DIVM); /**< Clear the LSB mantissa of UARTDIV  */
      008828 C6 52 42         [ 1]  215 	ld	a, 0x5242
      00882B 35 00 52 42      [ 1]  216 	mov	0x5242+0, #0x00
                                    217 ;	./../../mib/stm8s_mib_debug.c: 496: UARTXdebug->BRR2 &= (uint8_t)(~UARTX_BRR2_DIVM); /**< Clear the MSB mantissa of UARTDIV  */
      00882F C6 52 43         [ 1]  218 	ld	a, 0x5243
      008832 A4 0F            [ 1]  219 	and	a, #0x0f
      008834 C7 52 43         [ 1]  220 	ld	0x5243, a
                                    221 ;	./../../mib/stm8s_mib_debug.c: 497: UARTXdebug->BRR2 &= (uint8_t)(~UARTX_BRR2_DIVF); /**< Clear the Fraction bits of UARTDIV */
      008837 C6 52 43         [ 1]  222 	ld	a, 0x5243
      00883A A4 F0            [ 1]  223 	and	a, #0xf0
      00883C C7 52 43         [ 1]  224 	ld	0x5243, a
                                    225 ;	./../../mib/stm8s_mib_debug.c: 500: BaudRate_Mantissa = ((uint32_t)v_ClkValue / (BaudRate << 4));
      00883F A6 04            [ 1]  226 	ld	a, #0x04
      008841                        227 00103$:
      008841 58               [ 2]  228 	sllw	x
      008842 90 59            [ 2]  229 	rlcw	y
      008844 4A               [ 1]  230 	dec	a
      008845 26 FA            [ 1]  231 	jrne	00103$
      008847 89               [ 2]  232 	pushw	x
      008848 90 89            [ 2]  233 	pushw	y
      00884A 89               [ 2]  234 	pushw	x
      00884B 90 89            [ 2]  235 	pushw	y
      00884D 4B 00            [ 1]  236 	push	#0x00
      00884F 4B 24            [ 1]  237 	push	#0x24
      008851 4B F4            [ 1]  238 	push	#0xf4
      008853 4B 00            [ 1]  239 	push	#0x00
                                    240 ;	./../../mib/stm8s_mib_debug.c: 501: BaudRate_Mantissa100 = (((uint32_t)v_ClkValue * 100) / (BaudRate << 4));
      008855 CD 89 C2         [ 4]  241 	call	__divulong
      008858 5B 08            [ 2]  242 	addw	sp, #8
      00885A 1F 07            [ 2]  243 	ldw	(0x07, sp), x
      00885C 17 05            [ 2]  244 	ldw	(0x05, sp), y
      00885E 90 85            [ 2]  245 	popw	y
      008860 85               [ 2]  246 	popw	x
      008861 89               [ 2]  247 	pushw	x
      008862 90 89            [ 2]  248 	pushw	y
      008864 4B 00            [ 1]  249 	push	#0x00
      008866 4B 10            [ 1]  250 	push	#0x10
      008868 4B 5E            [ 1]  251 	push	#0x5e
      00886A 4B 5F            [ 1]  252 	push	#0x5f
                                    253 ;	./../../mib/stm8s_mib_debug.c: 503: BRR2_1 = (uint8_t)((uint8_t)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100)) << 4) / 100) & (u8)0x0F); /**< Set the fraction of UARTDIV  */
      00886C CD 89 C2         [ 4]  254 	call	__divulong
      00886F 5B 08            [ 2]  255 	addw	sp, #8
      008871 1F 07            [ 2]  256 	ldw	(0x07, sp), x
      008873 90 02            [ 1]  257 	rlwa	y
      008875 6B 05            [ 1]  258 	ld	(0x05, sp), a
      008877 90 01            [ 1]  259 	rrwa	y
      008879 90 9F            [ 1]  260 	ld	a, yl
      00887B 88               [ 1]  261 	push	a
      00887C 1E 04            [ 2]  262 	ldw	x, (0x04, sp)
      00887E 89               [ 2]  263 	pushw	x
      00887F 1E 04            [ 2]  264 	ldw	x, (0x04, sp)
      008881 89               [ 2]  265 	pushw	x
      008882 4B 64            [ 1]  266 	push	#0x64
      008884 5F               [ 1]  267 	clrw	x
      008885 89               [ 2]  268 	pushw	x
      008886 4B 00            [ 1]  269 	push	#0x00
      008888 CD 8A 1D         [ 4]  270 	call	__mullong
      00888B 5B 08            [ 2]  271 	addw	sp, #8
      00888D 1F 0C            [ 2]  272 	ldw	(0x0c, sp), x
      00888F 17 0A            [ 2]  273 	ldw	(0x0a, sp), y
      008891 84               [ 1]  274 	pop	a
      008892 16 07            [ 2]  275 	ldw	y, (0x07, sp)
      008894 72 F2 0B         [ 2]  276 	subw	y, (0x0b, sp)
      008897 12 0A            [ 1]  277 	sbc	a, (0x0a, sp)
      008899 97               [ 1]  278 	ld	xl, a
      00889A 7B 05            [ 1]  279 	ld	a, (0x05, sp)
      00889C 12 09            [ 1]  280 	sbc	a, (0x09, sp)
      00889E 95               [ 1]  281 	ld	xh, a
      00889F A6 04            [ 1]  282 	ld	a, #0x04
      0088A1                        283 00105$:
      0088A1 90 58            [ 2]  284 	sllw	y
      0088A3 59               [ 2]  285 	rlcw	x
      0088A4 4A               [ 1]  286 	dec	a
      0088A5 26 FA            [ 1]  287 	jrne	00105$
      0088A7 4B 64            [ 1]  288 	push	#0x64
      0088A9 4B 00            [ 1]  289 	push	#0x00
      0088AB 4B 00            [ 1]  290 	push	#0x00
      0088AD 4B 00            [ 1]  291 	push	#0x00
      0088AF 90 89            [ 2]  292 	pushw	y
      0088B1 89               [ 2]  293 	pushw	x
      0088B2 CD 89 C2         [ 4]  294 	call	__divulong
      0088B5 5B 08            [ 2]  295 	addw	sp, #8
      0088B7 9F               [ 1]  296 	ld	a, xl
      0088B8 A4 0F            [ 1]  297 	and	a, #0x0f
      0088BA 6B 0C            [ 1]  298 	ld	(0x0c, sp), a
                                    299 ;	./../../mib/stm8s_mib_debug.c: 504: BRR2_2 = (uint8_t)((BaudRate_Mantissa >> 4) & (u8)0xF0);
      0088BC 1E 03            [ 2]  300 	ldw	x, (0x03, sp)
      0088BE A6 10            [ 1]  301 	ld	a, #0x10
      0088C0 62               [ 2]  302 	div	x, a
      0088C1 90 93            [ 1]  303 	ldw	y, x
      0088C3 9F               [ 1]  304 	ld	a, xl
      0088C4 A4 F0            [ 1]  305 	and	a, #0xf0
                                    306 ;	./../../mib/stm8s_mib_debug.c: 506: UARTXdebug->BRR2 = (uint8_t)(BRR2_1 | BRR2_2);
      0088C6 1A 0C            [ 1]  307 	or	a, (0x0c, sp)
      0088C8 C7 52 43         [ 1]  308 	ld	0x5243, a
                                    309 ;	./../../mib/stm8s_mib_debug.c: 507: UARTXdebug->BRR1 = (uint8_t)BaudRate_Mantissa; /**< Set the LSB mantissa of UARTDIV  */
      0088CB 7B 04            [ 1]  310 	ld	a, (0x04, sp)
      0088CD C7 52 42         [ 1]  311 	ld	0x5242, a
                                    312 ;	./../../mib/stm8s_mib_debug.c: 509: UARTXdebug->CR2 &= (uint8_t) ~(UARTX_CR2_TEN | UARTX_CR2_REN);																																		 /**< Disable the Transmitter and Receiver before seting the LBCL, CPOL and CPHA bits */
      0088D0 C6 52 45         [ 1]  313 	ld	a, 0x5245
      0088D3 A4 F3            [ 1]  314 	and	a, #0xf3
      0088D5 C7 52 45         [ 1]  315 	ld	0x5245, a
                                    316 ;	./../../mib/stm8s_mib_debug.c: 510: UARTXdebug->CR3 &= (uint8_t) ~(UARTX_CR3_CPOL | UARTX_CR3_CPHA | UARTX_CR3_LBCL);																									 /**< Clear the Clock Polarity, lock Phase, Last Bit Clock pulse */
      0088D8 C6 52 46         [ 1]  317 	ld	a, 0x5246
      0088DB A4 F8            [ 1]  318 	and	a, #0xf8
      0088DD C7 52 46         [ 1]  319 	ld	0x5246, a
                                    320 ;	./../../mib/stm8s_mib_debug.c: 511: UARTXdebug->CR3 |= (uint8_t)((uint8_t)UARTX_SYNCMODE_CLOCK_DISABLE & (uint8_t)(UARTX_CR3_CPOL | UARTX_CR3_CPHA | UARTX_CR3_LBCL)); /**< Set the Clock Polarity, lock Phase, Last Bit Clock pulse */
      0088E0 55 52 46 52 46   [ 1]  321 	mov	0x5246, 0x5246
                                    322 ;	./../../mib/stm8s_mib_debug.c: 513: UARTXdebug->CR2 |= (uint8_t)UARTX_CR2_TEN;									/**< Set the Transmitter Enable bit */
      0088E5 72 16 52 45      [ 1]  323 	bset	0x5245, #3
                                    324 ;	./../../mib/stm8s_mib_debug.c: 517: UARTXdebug->CR2 |= (uint8_t)UARTX_CR2_REN | UARTX_CR2_RIEN; /**< Set the Receiver Enable bit */
      0088E9 C6 52 45         [ 1]  325 	ld	a, 0x5245
      0088EC AA 24            [ 1]  326 	or	a, #0x24
      0088EE C7 52 45         [ 1]  327 	ld	0x5245, a
                                    328 ;	./../../mib/stm8s_mib_debug.c: 525: UARTXdebug->CR3 &= (uint8_t)(~UARTX_CR3_CKEN); /**< Clear the Clock Enable bit */
      0088F1 72 17 52 46      [ 1]  329 	bres	0x5246, #3
                                    330 ;	./../../mib/stm8s_mib_debug.c: 531: UCOM_TXD_GPIO->DDR |= (UCOM_TXD_PIN); /* Set Output mode */
      0088F5 72 1A 50 11      [ 1]  331 	bset	0x5011, #5
                                    332 ;	./../../mib/stm8s_mib_debug.c: 532: UCOM_TXD_GPIO->CR1 |= (UCOM_TXD_PIN);	/* Pull-Up or Push-Pull */
      0088F9 72 1A 50 12      [ 1]  333 	bset	0x5012, #5
                                    334 ;	./../../mib/stm8s_mib_debug.c: 533: UCOM_TXD_GPIO->CR2 |= (UCOM_TXD_PIN);	/* Output speed up to 10 MHz */
      0088FD 72 1A 50 13      [ 1]  335 	bset	0x5013, #5
                                    336 ;	./../../mib/stm8s_mib_debug.c: 534: UCOM_TXD_GPIO->ODR |= (UCOM_TXD_PIN); // high... 
      008901 72 1A 50 0F      [ 1]  337 	bset	0x500f, #5
                                    338 ;	./../../mib/stm8s_mib_debug.c: 538: UCOM_RXD_GPIO->DDR &= ~(UCOM_RXD_PIN); // Set input mode 
      008905 72 1D 50 11      [ 1]  339 	bres	0x5011, #6
                                    340 ;	./../../mib/stm8s_mib_debug.c: 539: UCOM_RXD_GPIO->CR1 |= (UCOM_RXD_PIN);	 /* Pull-Up or Push-Pull */
      008909 72 1C 50 12      [ 1]  341 	bset	0x5012, #6
                                    342 ;	./../../mib/stm8s_mib_debug.c: 540: UCOM_RXD_GPIO->CR2 &= ~(UCOM_RXD_PIN); /*  External interrupt disabled */
      00890D C6 50 13         [ 1]  343 	ld	a, 0x5013
      008910 A4 BF            [ 1]  344 	and	a, #0xbf
      008912 C7 50 13         [ 1]  345 	ld	0x5013, a
                                    346 ;	./../../mib/stm8s_mib_debug.c: 548: }
      008915 1E 0D            [ 2]  347 	ldw	x, (13, sp)
      008917 5B 12            [ 2]  348 	addw	sp, #18
      008919 FC               [ 2]  349 	jp	(x)
                                    350 ;	./../../mib/stm8s_mib_debug.c: 554: void MibWriteDebugTransmitHex(uint8_t data)
                                    351 ;	-----------------------------------------
                                    352 ;	 function MibWriteDebugTransmitHex
                                    353 ;	-----------------------------------------
      00891A                        354 _MibWriteDebugTransmitHex:
      00891A 88               [ 1]  355 	push	a
      00891B 6B 01            [ 1]  356 	ld	(0x01, sp), a
                                    357 ;	./../../mib/stm8s_mib_debug.c: 561: MibWriteDebugByte('.');
      00891D A6 2E            [ 1]  358 	ld	a, #0x2e
      00891F CD 87 96         [ 4]  359 	call	_MibWriteDebugByte
                                    360 ;	./../../mib/stm8s_mib_debug.c: 562: if (data > 0xd)	MibWriteDebugByte(data);
      008922 7B 01            [ 1]  361 	ld	a, (0x01, sp)
      008924 A1 0D            [ 1]  362 	cp	a, #0x0d
      008926 23 05            [ 2]  363 	jrule	00102$
      008928 7B 01            [ 1]  364 	ld	a, (0x01, sp)
      00892A CD 87 96         [ 4]  365 	call	_MibWriteDebugByte
      00892D                        366 00102$:
                                    367 ;	./../../mib/stm8s_mib_debug.c: 563: MibWriteDebugByte('[');
      00892D A6 5B            [ 1]  368 	ld	a, #0x5b
      00892F CD 87 96         [ 4]  369 	call	_MibWriteDebugByte
                                    370 ;	./../../mib/stm8s_mib_debug.c: 564: MibWriteDebugByte(g_TableHex[data >> 4]);
      008932 7B 01            [ 1]  371 	ld	a, (0x01, sp)
      008934 4E               [ 1]  372 	swap	a
      008935 A4 0F            [ 1]  373 	and	a, #0x0f
      008937 5F               [ 1]  374 	clrw	x
      008938 97               [ 1]  375 	ld	xl, a
      008939 72 BB 00 43      [ 2]  376 	addw	x, _g_TableHex+0
      00893D F6               [ 1]  377 	ld	a, (x)
      00893E CD 87 96         [ 4]  378 	call	_MibWriteDebugByte
                                    379 ;	./../../mib/stm8s_mib_debug.c: 565: MibWriteDebugByte(g_TableHex[data & 0xf]);
      008941 7B 01            [ 1]  380 	ld	a, (0x01, sp)
      008943 A4 0F            [ 1]  381 	and	a, #0x0f
      008945 97               [ 1]  382 	ld	xl, a
      008946 4F               [ 1]  383 	clr	a
      008947 95               [ 1]  384 	ld	xh, a
      008948 72 BB 00 43      [ 2]  385 	addw	x, _g_TableHex+0
      00894C F6               [ 1]  386 	ld	a, (x)
      00894D CD 87 96         [ 4]  387 	call	_MibWriteDebugByte
                                    388 ;	./../../mib/stm8s_mib_debug.c: 566: MibWriteDebugByte(']');
      008950 A6 5D            [ 1]  389 	ld	a, #0x5d
      008952 CD 87 96         [ 4]  390 	call	_MibWriteDebugByte
                                    391 ;	./../../mib/stm8s_mib_debug.c: 567: while (!(UARTXdebug->SR & UARTX_SR_TC));
      008955                        392 00103$:
      008955 72 0D 52 40 FB   [ 2]  393 	btjf	0x5240, #6, 00103$
                                    394 ;	./../../mib/stm8s_mib_debug.c: 568: UARTXdebug->SR &= ~UARTX_SR_TC;	
      00895A 72 1D 52 40      [ 1]  395 	bres	0x5240, #6
                                    396 ;	./../../mib/stm8s_mib_debug.c: 574: }
      00895E 84               [ 1]  397 	pop	a
      00895F 81               [ 4]  398 	ret
                                    399 ;	./../../mib/stm8s_mib_debug.c: 579: void MibWriteDebugTransmitHex2(uint8_t data)
                                    400 ;	-----------------------------------------
                                    401 ;	 function MibWriteDebugTransmitHex2
                                    402 ;	-----------------------------------------
      008960                        403 _MibWriteDebugTransmitHex2:
      008960 88               [ 1]  404 	push	a
      008961 6B 01            [ 1]  405 	ld	(0x01, sp), a
                                    406 ;	./../../mib/stm8s_mib_debug.c: 586: MibWriteDebugByte('.');
      008963 A6 2E            [ 1]  407 	ld	a, #0x2e
      008965 CD 87 96         [ 4]  408 	call	_MibWriteDebugByte
                                    409 ;	./../../mib/stm8s_mib_debug.c: 587: MibWriteDebugByte(g_TableHex[data >> 4]);
      008968 7B 01            [ 1]  410 	ld	a, (0x01, sp)
      00896A 4E               [ 1]  411 	swap	a
      00896B A4 0F            [ 1]  412 	and	a, #0x0f
      00896D 5F               [ 1]  413 	clrw	x
      00896E 97               [ 1]  414 	ld	xl, a
      00896F 72 BB 00 43      [ 2]  415 	addw	x, _g_TableHex+0
      008973 F6               [ 1]  416 	ld	a, (x)
      008974 CD 87 96         [ 4]  417 	call	_MibWriteDebugByte
                                    418 ;	./../../mib/stm8s_mib_debug.c: 588: MibWriteDebugByte(g_TableHex[data & 0xf]);
      008977 7B 01            [ 1]  419 	ld	a, (0x01, sp)
      008979 A4 0F            [ 1]  420 	and	a, #0x0f
      00897B 97               [ 1]  421 	ld	xl, a
      00897C 4F               [ 1]  422 	clr	a
      00897D 95               [ 1]  423 	ld	xh, a
      00897E 72 BB 00 43      [ 2]  424 	addw	x, _g_TableHex+0
      008982 F6               [ 1]  425 	ld	a, (x)
      008983 CD 87 96         [ 4]  426 	call	_MibWriteDebugByte
                                    427 ;	./../../mib/stm8s_mib_debug.c: 589: while (!(UARTXdebug->SR & UARTX_SR_TC));
      008986                        428 00101$:
      008986 72 0D 52 40 FB   [ 2]  429 	btjf	0x5240, #6, 00101$
                                    430 ;	./../../mib/stm8s_mib_debug.c: 590: UARTXdebug->SR &= ~UARTX_SR_TC;
      00898B 72 1D 52 40      [ 1]  431 	bres	0x5240, #6
                                    432 ;	./../../mib/stm8s_mib_debug.c: 596: }
      00898F 84               [ 1]  433 	pop	a
      008990 81               [ 4]  434 	ret
                                    435 ;	./../../mib/stm8s_mib_debug.c: 602: void UART_RX_IRQHandler(void) /* UART RX */
                                    436 ;	-----------------------------------------
                                    437 ;	 function UART_RX_IRQHandler
                                    438 ;	-----------------------------------------
      008991                        439 _UART_RX_IRQHandler:
                                    440 ;	./../../mib/stm8s_mib_debug.c: 608: if (UARTXdebug->SR & UARTX_SR_RXNE) // RXNE :  ISR.bit5 // UART1_FLAG_RXNE
      008991 72 0B 52 40 22   [ 2]  441 	btjf	0x5240, #5, 00108$
                                    442 ;	./../../mib/stm8s_mib_debug.c: 610: vbCh = (uint8_t)(UARTXdebug->DR);
      008996 C6 52 41         [ 1]  443 	ld	a, 0x5241
      008999 90 97            [ 1]  444 	ld	yl, a
                                    445 ;	./../../mib/stm8s_mib_debug.c: 611: vbIndex = rx_buffer_head + 1;
      00899B C6 00 41         [ 1]  446 	ld	a, _rx_buffer_head+0
      00899E 4C               [ 1]  447 	inc	a
                                    448 ;	./../../mib/stm8s_mib_debug.c: 612: if (vbIndex >= RX_BUFFER_SIZE)
      00899F A1 40            [ 1]  449 	cp	a, #0x40
      0089A1 25 01            [ 1]  450 	jrc	00102$
                                    451 ;	./../../mib/stm8s_mib_debug.c: 613: vbIndex = 0;
      0089A3 4F               [ 1]  452 	clr	a
      0089A4                        453 00102$:
                                    454 ;	./../../mib/stm8s_mib_debug.c: 614: if (vbIndex != rx_buffer_tail)
      0089A4 C1 00 42         [ 1]  455 	cp	a, _rx_buffer_tail+0
      0089A7 26 01            [ 1]  456 	jrne	00143$
      0089A9 81               [ 4]  457 	ret
      0089AA                        458 00143$:
                                    459 ;	./../../mib/stm8s_mib_debug.c: 616: rx_buffer[vbIndex] = vbCh;
      0089AA 5F               [ 1]  460 	clrw	x
      0089AB 97               [ 1]  461 	ld	xl, a
      0089AC 1C 00 01         [ 2]  462 	addw	x, #(_rx_buffer+0)
      0089AF 88               [ 1]  463 	push	a
      0089B0 90 9F            [ 1]  464 	ld	a, yl
      0089B2 F7               [ 1]  465 	ld	(x), a
      0089B3 84               [ 1]  466 	pop	a
                                    467 ;	./../../mib/stm8s_mib_debug.c: 617: rx_buffer_head = vbIndex;
      0089B4 C7 00 41         [ 1]  468 	ld	_rx_buffer_head+0, a
      0089B7 81               [ 4]  469 	ret
      0089B8                        470 00108$:
                                    471 ;	./../../mib/stm8s_mib_debug.c: 620: else if (UARTXdebug->SR & UARTX_SR_OR) // OE : ISR.bit4 // /*!< OverRun error flag */
      0089B8 72 06 52 40 01   [ 2]  472 	btjt	0x5240, #3, 00145$
      0089BD 81               [ 4]  473 	ret
      0089BE                        474 00145$:
                                    475 ;	./../../mib/stm8s_mib_debug.c: 622: vbCh = (uint8_t)(UARTXdebug->DR);
      0089BE C6 52 41         [ 1]  476 	ld	a, 0x5241
                                    477 ;	./../../mib/stm8s_mib_debug.c: 625: }
      0089C1 81               [ 4]  478 	ret
                                    479 	.area CODE
                                    480 	.area CONST
                                    481 	.area CONST
      00815E                        482 ___str_0:
      00815E 30 31 32 33 34 35 36   483 	.ascii "0123456789ABCDEF"
             37 38 39 41 42 43 44
             45 46
      00816E 00                     484 	.db 0x00
                                    485 	.area CODE
                                    486 	.area INITIALIZER
      00816F                        487 __xinit__g_TableHex:
      00816F 81 5E                  488 	.dw ___str_0
                                    489 	.area CABS (ABS)
