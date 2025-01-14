/**
	******************************************************************************
	* @file    : stm8s_awu_main.c
	* @brief   : auto wakeup 
	* @author  : MYMEDIA Co., Ltd.
	* @version : V1.0.0
	* @date    : 2023 : 02 : 09
	******************************************************************************
	*/
/*
  compile : make
  write   : ST Visual Programmer 
  exe     : ./exe/stm8s_awu.hex
  src : ./mib/stm8s_mib_it.c
        ./mib/stm8s_mib_debug.c
        ./mib/stm8s_mib_vsprintf.c
        ./mib/stm8s_mib_debug_tim.c
        ./mib/stm8s_mib_debug_cmd.c
        ./mib/stm8s_mib_debug_mem.c
        ./mib/stm8s_mib_debug_top.c
        
  info :
    STM8S Reference Manuals : <https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf>
    page 114, 참고한다.
    stm8s 가 지니고 있는 AWU : auto wake up 기능에 대해서 공부한다.
       
    AWU : auto wake up 
    #define __IRQ_AWU_CALLBACK_USE__  
    컴파일 시 위 변수가 define이 되어야 인터럽트시 콜백이 된다.

    awu start 1000 : halt & wait 1000 msec 
    void irqAWU_CALLBACK(void) 함수가 call back 이 된다. 
    halt , 1000msec 후에 irqAWU_CALLBACK 함수가 수행이 될때 클리어가 되어야 한다.
    클리어가 되지 않으면 halt후에 깨어나지 못한다.
    이점 주의해야 한다.

    auto wakeup 은 다음의 값을 calConfigAWU(delay msec) 설정하고 halt 모드에 들어가면 irq가 발생하며 
    irqAWU_CALLBACK 를 callback 하여 수행하며 수행시 irq flag를 클리어를 하여야 한다. 
    클리어를 하지 않으면 동작하지 않는다.
*/ 
// #if defined(__MIB_DEBUG_AWU_) || defined(__MIB_DEBUG_TEST__)
#if 1
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "stm8s_mib.h"
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void GPIO_Configuration(void);
void calConfigAWU(uint16_t vwDelay);
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int DoAWU(CMD_MY * cptr, int argc, char **argv);
/* Private macro -------------------------------------------------------------*/
#define CMD_MY_AWU_ALL	\
{   "awu", DoAWU,		\
	"  awu {init} : gpio init\r\n"\
	"  awu {write} [addr] [data] : (addr 16bit) write (data 8bit)\r\n"\
	"  awu {read} [addr] : (addr 16bit) read (data 8bit)\r\n"\
	"  awu {delay} [data:(?)msec] : AWU set delay var.\r\n"\
	"  awu {disable}\r\n"\
	"  awu {enable}\r\n"\
	"  awu {start} [data : msec] : Wake up after a delay after AWU starts\r\n"\
	"  awu {help}  awu help.\r\n",\
},
/* Private variables ---------------------------------------------------------*/
static CMD_MY cmdTbl_only[] = {
		CMD_MY_AWU_ALL
		CMD_MY_END
};
/* Public functions ----------------------------------------------------------*/
extern CMD_ARRAY cmd_list;
extern CMD_MY cmdTbl[];
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/**
 * @brief
 * @par Parameters: None
 * @retval None
 */ 
void cmd_awu(void)
{
	CMD_MY *cptr;
	uint32_t index;

	index = 0;
	for (cptr = cmdTbl;; cptr++) 
	{
		if(cptr->cmd==0)
		{
			ccprintf(1,("INFO:+cmd_awu...\r\n"));
			memcpy((void *)&(cptr->cmd),(void *)&(cmdTbl_only[0].cmd),sizeof(CMD_MY));
			break;
		}
		index++;
		if(index>MAX_COMMANDS)
		{ 
			ccprintf(1,("INFO:-cmd_awu...\r\n"));
			return;
		}
	}
	return;
}
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#if defined(__MIB_DEBUG_TEST__)
void cmd_test(void)
{
  cmd_awu();
}
#endif
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#if defined(__MIB_DEBUG_TEST__)
void cmd_test_init_before_irq_enable(void)
{
  // 인터럽트 인에이블 하기 전 초기화 시킬 것을 실행하는 곳..
}
#endif
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/**
 * @brief
 * @par Parameters: None
 * @retval None
 */ 
#define _DEBUG_CMD_A_  1
static int DoAWU(CMD_MY *cptr, int argc, char **argv)
{
	uint32_t vdwAddr = 0;
	uint8_t vbData = 0;
	uint16_t vwData = 0;

	if (argc < 2)
	{
		MibWriteDebugString(cptr->usage);
		return FALSE;
	}	
  // argc=4, argv
  // 0       1       2      3
  // awu {write} [addr] [data]  
	if (!strcmp(argv[1], "write")) // _CMD_MY_WRITE_
	{
		if (argc < 4)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		if (!HexToInt(argv[2], &vdwAddr, 32))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}
		if (!HexToInt(argv[3], &vbData, 8))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}    
		wr_ADDR8(vdwAddr, vbData);
		ccprintf(_DEBUG_CMD_A_,("wr_ADDR8 : 0x%08lx[0x%02x]\r\n", vdwAddr,vbData));
	}
  // argc=3, argv
  // 0       1       2      
  // awu {read} [addr]
	else if (!strcmp(argv[1], "read")) // _CMD_MY_READ_
	{
		if (argc < 3)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		if (!HexToInt(argv[2], &vdwAddr, 32))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}
    {      
			vbData = rd_ADDR8(vdwAddr);
    }   
		ccprintf(_DEBUG_CMD_A_,("rd_ADDR8 : 0x%08lx[0x%02x]\r\n", vdwAddr,vbData));
	}	  
  // argc=3, argv
  // 0    1       2      
  // awu {delay} [data:(?)msec]
	else if (!strcmp(argv[1], "delay")) // _CMD_MY_READ_
	{
		if (argc < 3)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		// if (!HexToInt(argv[2], &vwData, 16))
		if (!DecToLong(argv[2], &vwData, 16))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}
		ccprintf(_DEBUG_CMD_A_, ("awu delay [%d] msec\r\n",(vwData)));		
		calConfigAWU(vwData);
	}
  // argc, argv
  // 0       1           
  // awu {enable}
	else if (!strcmp(argv[1], "enable")) // _CMD_MY_READ_
	{
		ccprintf(_DEBUG_CMD_A_, ("awu enable....()\r\n"));
    /* Enable the AWU peripheral */
    AWU->CSR |= AWU_CSR_AWUEN;
	}	 
  // argc, argv
  // 0       1           
  // awu {disable}
	else if (!strcmp(argv[1], "disable")) // _CMD_MY_READ_
	{
		ccprintf(_DEBUG_CMD_A_, ("awu disable....()\r\n"));
    /* Disable the AWU peripheral */
    AWU->CSR &= (uint8_t)(~AWU_CSR_AWUEN);
	}	 
  // argc=3, argv
  // 0       1    2       
  // awu {start} [data : msec]
	else if (!strcmp(argv[1], "start")) // _CMD_MY_READ_
	{
		uint8_t vbIndex = 10;
		if (argc < 3)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		if (!DecToLong(argv[2], &vwData, 16))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}
		ccprintf(_DEBUG_CMD_A_, ("awu delay [%d] msec\r\n",(vwData)));		
		GPIO_Configuration();
		calConfigAWU(vwData);
    AWU->CSR |= AWU_CSR_AWUEN;
		do {
			// if(UCOM_LED1_GPIO->IDR & UCOM_LED1_PIN)
			// {			
			// 	UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR & ~UCOM_LED1_PIN;
			// }
			// else
			// {
			// 	UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR | UCOM_LED1_PIN;
			// }
			ccprintf(_DEBUG_CMD_A_, ("awu start....(%d)\r\n",vbIndex));
			MibWriteDebugEmptyCheck();
			{
				// halt & Wait...
				halt();
				//wfi();
			}
			///* Disable the AWU peripheral */
			//AWU->CSR &= (uint8_t)(~AWU_CSR_AWUEN);
			ccprintf(_DEBUG_CMD_A_, ("awu end....(%d)\r\n",vbIndex));
			// if(UCOM_LED1_GPIO->IDR & UCOM_LED1_PIN)
			// {			
			// 	UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR & ~UCOM_LED1_PIN;
			// }
			// else
			// {
			// 	UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR | UCOM_LED1_PIN;
			// }
			vbIndex--;
			if(vbIndex==0) break;
		} while(1);
    AWU->CSR &= (uint8_t)(~AWU_CSR_AWUEN);
	}	 
  // argc, argv
  // 0       1           
  // awu {init}
	else if (!strcmp(argv[1], "init")) // _CMD_MY_READ_
	{
		GPIO_Configuration();
		//AWU_Init(AWU_TIMEBASE_2S);
		ccprintf(_DEBUG_CMD_A_, ("awu init....()\r\n"));		
	}	  
  // argc, argv
  // 0       1           
  // awu {help}
	else if (!strcmp(argv[1], "help")) // _CMD_MY_READ_
	{
		ccprintf(_DEBUG_CMD_A_, ("awu start 1000 : halt & wait 1000 msec \r\n"));		
		ccprintf(_DEBUG_CMD_A_, ("https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf page 114\r\n"));		
    // https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf page 114
	}	 
	else
	{
		MibWriteDebugString(cptr->usage);
		return FALSE;
	}
	return TRUE;
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Hardware define */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
// void GPIO_Configuration(void);
// void Toggle(void);
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
  ******************************************************************************
  * @brief Configures GPIOs
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void GPIO_Configuration(void)
{
  // /* GPIOD reset */
  GPIO_DeInit(UCOM_LED1_GPIO);

  // /* Configure PD0 (LED1) as output push-pull low (led switched on) */
  GPIO_Init(UCOM_LED1_GPIO, UCOM_LED1_PIN, UCOM_LED1_MODE);
  
  GPIO_DeInit(UCOM_KEY1_GPIO);
  // /* Configure PB5 : KEY IN as input push-pull low (led switched on) */
  GPIO_Init(UCOM_KEY1_GPIO, UCOM_KEY1_PIN, UCOM_KEY1_MODE);
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void calConfigAWU(uint16_t vwDelay) // msec... 
{
	uint32_t vdwData = vwDelay;
	uint32_t vdwDataLast = 0;
	uint8_t vbIndex = 15;
	if(vwDelay>5120)
	{
		vbIndex = 12;
	}
	else if(vwDelay>2048)
	{
		vbIndex = 11;
	}
	else
	{		
		do {
			if(vwDelay>(1<<vbIndex))
			{
				break;
			}
			if(vbIndex) vbIndex--;
			else break;
		} while(1);	
	}
	if(vwDelay == 1) vbIndex = 2;
	else vbIndex = vbIndex + 3;
	ccprintf(_DEBUG_CMD_A_, ("awu vwDelay [%08lx,%d] bit\r\n",vdwData,vbIndex));		
	{
		uint8_t vbAWUTB = 0;
		uint8_t vbAPR = 0;
		vbAWUTB = vbIndex;		
		vdwData = vdwData*128;
		ccprintf(_DEBUG_CMD_A_, ("awu vdwData [%08lx,%d,%d] \r\n",vdwData,vbAPR,vbAWUTB));		
		if(vbAWUTB==14)
		{
			vdwData = vdwData /(uint32_t)(5*(1<<11));
			vdwDataLast = (5*((1<<11)/128)) * vdwData;
		}
		else if(vbAWUTB==15)
		{
			vdwData = vdwData/30;
			vdwData = vdwData /(uint32_t)(1*(1<<11));
			vdwDataLast = (30*((1<<11)/128)) * vdwData;
		}
		else
		{
			vdwData = vdwData /(1<<(vbAWUTB-1));
			vdwDataLast = (1<<(vbAWUTB-1)) * vdwData / 128;
		}
		vbAPR = vdwData;
		ccprintf(_DEBUG_CMD_A_, ("awu vdwData [%08lx,%d,%d] \r\n",vdwDataLast,vbAPR,vbAWUTB));		
		{
    	AWU->APR = (uint8_t)vbAPR;
			AWU->TBR = (uint8_t)vbAWUTB;
			// AWU->CSR = (uint8_t)0;
		}
	}
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#if defined(__IRQ_AWU_CALLBACK_USE__)
void irqAWU_CALLBACK(void)
{
	// clear AWUF
	uint8_t vbData;
	vbData = (((uint8_t)(AWU->CSR & AWU_CSR_AWUF) == (uint8_t)0x00) ? RESET : SET);	
	if(vbData==SET)
	{
  	UCOM_LED1_GPIO->ODR ^= (uint8_t)UCOM_LED1_PIN;
	}
	else
	{
  	UCOM_LED1_GPIO->ODR ^= (uint8_t)UCOM_LED1_PIN;
	}
}
#endif // __IRQ_AWU_CALLBACK_USE__
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#endif // __MIB_DEBUG_AWU__
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/*
https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf

12 Auto-wakeup (AWU)
12.1 AWU introduction
The AWU is used to provide an internal wakeup time base that is used when the MCU goes
into Active-halt power saving mode. This time base is clocked by the low speed internal
(LSI) RC oscillator clock or the HSE crystal oscillator clock divided by a prescaler.

10.2 Low power modes
Table 20. Low power mode management

Mode (consumption level)        Main voltage regulator           Oscillators      CPU     Peripherals          Wakeup trigger event

Wait                                                                                                           All internal interrupts
( - )                              On                              On             Off        On(1)             (including AWU) or external interrupts, reset

Active-halt
( - - )                           On                     Off except LSI (or HSE)   Off       Only AWU(2)         AWU or external(3)  interrupts, reset

Active-halt with         Off (low power regulator on)   Off except LSI only       Off        Only AWU(2)         AWU or external(3) interrupts, reset
MVR auto power off
( - - - )

Halt                    Off (low power regulator on)    Off                        Off         Off(2)              External(3) interrupts, reset
( - - - - )

1. If the peripheral clock is not disabled by peripheral clock gating function.
2. If activated, BEEP or IWDG stay switched on. In this case, the LSI clock is forced to run.
3. Including communication peripheral interrupts.

OPTION bit reset value :  PRSC[1:0] = 0, CKAWUSEL = 0
HSE clock to 128 kHz LS clock

APR[5:0]
AWUTB[3:0]
AWUF   :  AWUF: Auto-wakeup flag
AWUEN  :  AWUEN: Auto-wakeup enable

12.4.1 Control/status register (AWU_CSR)
Address offset: 0x00
Reset value: 0x00

76543210
Reserved AWUF AWUEN Reserved MSR
r rc_r rw r rw
Bits 7:6 Reserved
Bit 5 AWUF: Auto-wakeup flag
This bit is set by hardware when the AWU module generates an interrupt and cleared by reading the
AWU_CSR register. Writing to this bit does not change its value.
0: No AWU interrupt occurred
1: AWU interrupt occurred
Bit 4 AWUEN: Auto-wakeup enable
This bit is set and cleared by software. It enables the auto-wakeup feature. If the microcontroller enters
Active-halt or Wait mode, the AWU feature wakes up the microcontroller after a programmable time
delay.
0: AWU (Auto-wakeup) disabled
1: AWU (Auto-wakeup) enabled
Bits 3:1 Reserved
Bit 0 MSR: Measurement enable
This bit connects the fLS clock to a timer input capture. This allows the timer to be used to measure the
LS frequency (fLS).
0: Measurement disabled
1: Measurement enabled
Note: Refer to the datasheet for information on which timer input capture can be connected to the LSI
clock in the specific product).


12.4.2 Asynchronous prescaler register (AWU_APR)
Address offset: 0x01
Reset value: 0x3F

76543210
Reserved APR[5:0]
r rw
Bits 7:6 Reserved
Bits 5:0 APR[5:0]: Asynchronous prescaler divider
These bits are written by software to select the prescaler divider (APRDIV) feeding the counter clock.
0x00: APRDIV = 2 0x0E: APRDIV = 16
0x01: APRDIV = 3 0x0F: APRDIV = 17
... ...
0x06: APRDIV = 8 0x3E: APRDIV = 64
Note: This register must not be kept at its reset value (0x3F)



12.4.3 Timebase selection register (AWU_TBR)
Address offset: 0x02
Reset value: 0x00

76543210
Reserved AWUTB[3:0]
r rw
Bits 7:4 Reserved
Bits 3:0 AWUTB[3:0]: Auto-wakeup timebase selection
These bits are written by software to define the time interval between AWU interrupts. AWU
interrupts are enabled when AWUEN = 1.
0000: No interrupt
0001: APRDIV/fLS 0010: 2xAPRDIV/fLS 0011: 22APRDIV/fLS
0100: 23APRDIV/fLS 0101: 24APRDIV/fLS 0110: 25APRDIV/fLS
0111: 26APRDIV/fLS 1000: 27APRDIV/fLS 1001: 28APRDIV/fLS
1010: 29APRDIV/fLS 1011: 210APRDIV/fLS 1100: 211APRDIV/fLS
1101: 212APRDIV/fLS 1110: 5x211APRDIV/fLS 1111: 30x211APRDIV/fLS

*/
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
