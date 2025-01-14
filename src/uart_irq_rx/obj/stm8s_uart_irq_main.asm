;--------------------------------------------------------
; File Created by SDCC : free open source ISO C Compiler 
; Version 4.4.0 #14620 (MINGW32)
;--------------------------------------------------------
	.module stm8s_uart_irq_main
	.optsdcc -mstm8
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _MibReadDebugByte
	.globl _MibWriteDebugString
	.globl _MibWriteDebugByte
	.globl _MibDebugInit
	.globl _GPIO_WriteReverse
	.globl _GPIO_Init
	.globl _GPIO_DeInit
	.globl _CLK_HSIPrescalerConfig
	.globl _CLK_Configuration
	.globl _GPIO_Configuration
	.globl _Toggle
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
	int _TRAP_IRQHandler ; trap
	int _TLI_IRQHandler ; int0
	int _AWU_IRQHandler ; int1
	int _CLK_IRQHandler ; int2
	int _EXTI_PORTA_IRQHandler ; int3
	int _EXTI_PORTB_IRQHandler ; int4
	int _EXTI_PORTC_IRQHandler ; int5
	int _EXTI_PORTD_IRQHandler ; int6
	int _EXTI_PORTE_IRQHandler ; int7
	int 0x000000 ; int8
	int 0x000000 ; int9
	int _SPI_IRQHandler ; int10
	int _TIM1_UPD_OVF_TRG_BRK_IRQHandler ; int11
	int _TIM1_CAP_COM_IRQHandler ; int12
	int _TIM2_UPD_OVF_BRK_IRQHandler ; int13
	int _TIM2_CAP_COM_IRQHandler ; int14
	int _TIM3_UPD_OVF_BRK_IRQHandler ; int15
	int _TIM3_CAP_COM_IRQHandler ; int16
	int 0x000000 ; int17
	int 0x000000 ; int18
	int _I2C_IRQHandler ; int19
	int _UART2_TX_IRQHandler ; int20
	int _UART2_RX_IRQHandler ; int21
	int _ADC1_IRQHandler ; int22
	int _TIM4_UPD_OVF_IRQHandler ; int23
	int _EEPROM_EEC_IRQHandler ; int24
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
;	stm8s_uart_irq_main.c: 46: void main(void)
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
	sub	sp, #4
;	stm8s_uart_irq_main.c: 50: CLK_Configuration();
	call	_CLK_Configuration
;	stm8s_uart_irq_main.c: 53: GPIO_Configuration();
	call	_GPIO_Configuration
;	stm8s_uart_irq_main.c: 56: MibDebugInit(9600);
	push	#0x80
	push	#0x25
	clrw	x
	pushw	x
	call	_MibDebugInit
;	stm8s_uart_irq_main.c: 59: enableInterrupts();
	rim
;	stm8s_uart_irq_main.c: 62: MibWriteDebugStringCheck(1,"\r\n ###################################\r\n");
	ldw	x, #(__str_0+0)
	call	_MibWriteDebugString
;	stm8s_uart_irq_main.c: 63: MibWriteDebugStringCheck(1," file name : " __FILE__ "\r\n");
	ldw	x, #(__str_1+0)
	call	_MibWriteDebugString
;	stm8s_uart_irq_main.c: 64: MibWriteDebugStringCheck(1," date :  "__DATE__"  :  "__TIME__"\r\n");
	ldw	x, #(__str_2+0)
	call	_MibWriteDebugString
;	stm8s_uart_irq_main.c: 65: MibWriteDebugStringCheck(1," webgpio.com by MYMEDIA Co., Ltd.\r\n");
	ldw	x, #(__str_3+0)
	call	_MibWriteDebugString
;	stm8s_uart_irq_main.c: 66: MibWriteDebugStringCheck(1," ###################################\r\n");
	ldw	x, #(__str_4+0)
	call	_MibWriteDebugString
;	stm8s_uart_irq_main.c: 70: uint32_t vdwDelay = 0;
	clrw	x
	ldw	(0x03, sp), x
	ldw	(0x01, sp), x
;	stm8s_uart_irq_main.c: 71: do {
00109$:
;	stm8s_uart_irq_main.c: 72: if((UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN)==0)
	btjt	0x5006, #5, 00105$
;	stm8s_uart_irq_main.c: 75: UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR | UCOM_LED1_PIN; // LED off
	bset	0x500f, #0
	jra	00106$
00105$:
;	stm8s_uart_irq_main.c: 79: if(vdwDelay)
	ldw	x, (0x03, sp)
	jrne	00143$
	ldw	x, (0x01, sp)
	jreq	00102$
00143$:
;	stm8s_uart_irq_main.c: 81: vdwDelay--;
	ldw	x, (0x03, sp)
	subw	x, #0x0001
	ldw	(0x03, sp), x
	ldw	x, (0x01, sp)
	jrnc	00144$
	decw	x
00144$:
	ldw	(0x01, sp), x
	jra	00106$
00102$:
;	stm8s_uart_irq_main.c: 85: vdwDelay = 320000;
	ldw	x, #0xe200
	ldw	(0x03, sp), x
	ldw	x, #0x0004
	ldw	(0x01, sp), x
;	stm8s_uart_irq_main.c: 86: Toggle();
	call	_Toggle
00106$:
;	stm8s_uart_irq_main.c: 89: vwCh = MibReadDebugByte();
	call	_MibReadDebugByte
;	stm8s_uart_irq_main.c: 90: if(vwCh!=MIB_DEBUG_READ_NODATA)
	ldw	y, x
	incw	y
	jreq	00109$
;	stm8s_uart_irq_main.c: 92: MibWriteDebugByte(vwCh);
	ld	a, xl
	call	_MibWriteDebugByte
;	stm8s_uart_irq_main.c: 94: } while(1);
	jra	00109$
;	stm8s_uart_irq_main.c: 96: }
	addw	sp, #4
	ret
;	stm8s_uart_irq_main.c: 108: void CLK_Configuration(void)
;	-----------------------------------------
;	 function CLK_Configuration
;	-----------------------------------------
_CLK_Configuration:
;	stm8s_uart_irq_main.c: 112: CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	clr	a
;	stm8s_uart_irq_main.c: 114: }
	jp	_CLK_HSIPrescalerConfig
;	stm8s_uart_irq_main.c: 126: void GPIO_Configuration(void)
;	-----------------------------------------
;	 function GPIO_Configuration
;	-----------------------------------------
_GPIO_Configuration:
;	stm8s_uart_irq_main.c: 129: GPIO_DeInit(UCOM_LED1_GPIO);
	ldw	x, #0x500f
	call	_GPIO_DeInit
;	stm8s_uart_irq_main.c: 131: GPIO_Init(UCOM_LED1_GPIO, UCOM_LED1_PIN, UCOM_LED1_MODE);
	push	#0xe0
	ld	a, #0x01
	ldw	x, #0x500f
	call	_GPIO_Init
;	stm8s_uart_irq_main.c: 133: GPIO_DeInit(UCOM_KEY1_GPIO);
	ldw	x, #0x5005
	call	_GPIO_DeInit
;	stm8s_uart_irq_main.c: 135: GPIO_Init(UCOM_KEY1_GPIO, UCOM_KEY1_PIN, UCOM_KEY1_MODE);
	push	#0x40
	ld	a, #0x20
	ldw	x, #0x5005
	call	_GPIO_Init
;	stm8s_uart_irq_main.c: 136: }
	ret
;	stm8s_uart_irq_main.c: 148: void Toggle(void)
;	-----------------------------------------
;	 function Toggle
;	-----------------------------------------
_Toggle:
;	stm8s_uart_irq_main.c: 150: GPIO_WriteReverse(UCOM_LED1_GPIO, UCOM_LED1_PIN);
	ld	a, #0x01
	ldw	x, #0x500f
;	stm8s_uart_irq_main.c: 151: }
	jp	_GPIO_WriteReverse
	.area CODE
	.area CONST
	.area CONST
__str_0:
	.db 0x0d
	.db 0x0a
	.ascii " ###################################"
	.db 0x0d
	.db 0x0a
	.db 0x00
	.area CODE
	.area CONST
__str_1:
	.ascii " file name : stm8s_uart_irq_main.c"
	.db 0x0d
	.db 0x0a
	.db 0x00
	.area CODE
	.area CONST
__str_2:
	.ascii " date :  Dec  4 2024  :  12:23:03"
	.db 0x0d
	.db 0x0a
	.db 0x00
	.area CODE
	.area CONST
__str_3:
	.ascii " webgpio.com by MYMEDIA Co., Ltd."
	.db 0x0d
	.db 0x0a
	.db 0x00
	.area CODE
	.area CONST
__str_4:
	.ascii " ###################################"
	.db 0x0d
	.db 0x0a
	.db 0x00
	.area CODE
	.area INITIALIZER
	.area CABS (ABS)
