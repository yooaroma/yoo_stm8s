/**
	******************************************************************************
	* @file    : stm8s_wwdg_main.c
	* @brief   : Window watchdog (WWDG)
	* @author  : MYMEDIA Co., Ltd.
	* @version : V1.0.0
	* @date    : 2023 : 02 : 09
	******************************************************************************
	<https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf>
	page 129
  초기화 방법에서 다음의 순서에 의해서 시작된다
	1. WWDG_CR의 레지스터의 WDGA의 bit의 값을 enable하여야 한다. 
	2. T6의 비트가 1에서 0으로 되는 순간 무조건 리셋이 된다.
	3. WWDG_CR의 값이 설정이 되면 먼저 WWDG_WR의 레지스터의 값보다 크면 곧 바로 리셋이 발생한다.

	사용방법은 딜레이 주기를 WWDG_CR에 계속적으로 로드함으로서 유효하다.
	한번 enable이 되면 클리어는 리셋에 의해서만 된다.
	따라서 계속적으로 WWDG_CR에 새로운 딜레이 값을 계속적으로 로드하여야 한다.
	되지 않으면 리셋이 발생한다.

  프로그램 예제에서는 키 입력 또는 100번 후에는 refresh를 멈춘다.

	*/

// #if defined(__MIB_DEBUG_WWDG__) || defined(__MIB_DEBUG_TEST__)
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

extern volatile uint16_t gwMibMiliSec;
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void GPIO_Configuration(void);
extern uint16_t calConfigWWDG(uint16_t vwDelay);
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/*
	iwdg init : gpio init
  iwdg start 1000 
*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int DoWWDG(CMD_MY * cptr, int argc, char **argv);
/* Private macro -------------------------------------------------------------*/
#define CMD_MY_WWDG_ALL	\
{   "wwdg", DoWWDG,		\
	"  wwdg {init} \r\n"\
	"  wwdg {write} [addr] [data]  \r\n"\
	"  wwdg {read} [addr] \r\n"\
	"  wwdg {start} [delay:msec] fcpu : 16MHz delay (1 to 49) \r\n"\
	"  wwdg {help}  iwdg help.\r\n",\
},
/* Private variables ---------------------------------------------------------*/
static CMD_MY cmdTbl_only[] = {
		CMD_MY_WWDG_ALL
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
void cmd_wwdg(void)
{
	CMD_MY *cptr;
	uint32_t index;

	index = 0;
	for (cptr = cmdTbl;; cptr++) 
	{
		if(cptr->cmd==0)
		{
			ccprintf(1,("INFO:+cmd_wwdg...\r\n"));
			memcpy((void *)&(cptr->cmd),(void *)&(cmdTbl_only[0].cmd),sizeof(CMD_MY));
			break;
		}
		index++;
		if(index>MAX_COMMANDS)
		{ 
			ccprintf(1,("INFO:-cmd_wwdg...\r\n"));
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
  cmd_wwdg();
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
static int DoWWDG(CMD_MY *cptr, int argc, char **argv)
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
  // wwdg {write} [addr] [data]  
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
  // wwdg {read} [addr]
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
  // 0       1    2       
  // wwdg {start} [data : msec]
	else if (!strcmp(argv[1], "start")) // _CMD_MY_READ_
	{
		uint8_t vbIndex = 0;
		uint8_t vbDelayCR = 0;
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
		GPIO_Configuration();
		ccprintf(_DEBUG_CMD_A_, ("wwdg delay [%d] msec  fcpu : 16MHz delay (1 to 49)msec\r\n",(vwData)));		
		if(vwData<10) vwData = 10;
		else if(vwData>49) vwData = 49;
		vbDelayCR = (calConfigWWDG(vwData) & 0x3F) | 0xC0;
		gwMibMiliSec = 0; 
		WWDG->WR = 0x7F;
		WWDG->CR = vbDelayCR; 
		do {				
			if(gwMibMiliSec==0)
			{
				vbIndex++;
				if(vbIndex==100)
				{					
					WWDG->CR = 0xFF; // vbDelayCR; 
					ccprintf(_DEBUG_CMD_A_, ("wwdg start....(%d)[%u:%d]\r\n",vbIndex,vwData,vwData));		
          MibWriteDebugEmptyCheck();
					WWDG->CR = 0x0; 
					break;
				}
				else
				{
					WWDG->CR = vbDelayCR; 
				}
				gwMibMiliSec = vwData-10; 
				// Toggle();
				UCOM_LED1_GPIO->ODR ^= (uint8_t)UCOM_LED1_PIN;
			}
      if((UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN)==0)
      {
        WWDG->CR = 0xFF; // vbDelayCR; 
        ccprintf(_DEBUG_CMD_A_, ("wwdg start....(%d)[%u:%d]\r\n",vbIndex,vwData,vwData));		
        MibWriteDebugEmptyCheck();
        WWDG->CR = 0x0; 
        break;
      }
		} while(1);

	}	 
  // argc, argv
  // 0       1           
  // iwdg {init}
	else if (!strcmp(argv[1], "init")) // _CMD_MY_READ_
	{
		GPIO_Configuration();
		//AWU_Init(AWU_TIMEBASE_2S);
		ccprintf(_DEBUG_CMD_A_, ("wwdg init....()\r\n"));		
	}	  
  // argc, argv
  // 0       1           
  // iwdg {help}
	else if (!strcmp(argv[1], "help")) // _CMD_MY_READ_
	{
		// ccprintf(_DEBUG_CMD_A_, ("wwdg help....argc[%x:%d]\r\n",argc,argc));		
		MibWriteDebugString(("wwdg help....	\r\n"	
			"wwdg init // gpio init\r\n"\
	    "wwdg start 49 // It should refresh within 49 miliseconds.  \r\n"\
			"// fcpu : 16MHz delay (1 to 49)msec\r\n"));
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
/*
  PD0 : LED1
  PB5 : KEY1 
  PD7 : 485_DIR
  PD5 : TXD
  PD6 : RXD
  PB7 : AIN7 : JOYSTICK
  PC7 : SPI_MISO
  PC6 : SPI_MOSI
  PC5 : SPI_CLK
  PE5 : SPI_NSS
  PE1 : I2C_SCL
  PE2 : I2C_SDA
*/
#define UCOM_LED1_GPIO    GPIOD        // PD0
#define UCOM_LED1_PIN     GPIO_PIN_0   // PD0
#define UCOM_LED1_MODE    GPIO_MODE_OUT_PP_LOW_FAST
#define UCOM_KEY1_GPIO    GPIOB        // PB5
#define UCOM_KEY1_PIN     GPIO_PIN_5   // PB5
#define UCOM_KEY1_MODE    GPIO_MODE_IN_PU_NO_IT // GPIO_MODE_IN_FL_NO_IT
#define UCOM_485_GPIO     GPIOD        // PD7
#define UCOM_485_PIN      GPIO_PIN_7   // PD7
#define UCOM_485_MODE     GPIO_MODE_OUT_PP_LOW_FAST
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
  // GPIO_DeInit(UCOM_LED1_GPIO);

  // /* Configure PD0 (LED1) as output push-pull low (led switched on) */
  // GPIO_Init(UCOM_LED1_GPIO, UCOM_LED1_PIN, UCOM_LED1_MODE);
  
  // GPIO_DeInit(UCOM_KEY1_GPIO);
  // /* Configure PA3 : KEY IN as input push-pull low (led switched on) */
  // GPIO_Init(UCOM_KEY1_GPIO, UCOM_KEY1_PIN, UCOM_KEY1_MODE);

  /* Configure PD0 (LED1) as output push-pull low (led switched on) */
  //GPIO_Init(UCOM_LED1_GPIO, UCOM_LED1_PIN, UCOM_LED1_MODE);
	{									
		UCOM_LED1_GPIO->DDR |= (UCOM_LED1_PIN); /* Set Output mode */
		UCOM_LED1_GPIO->CR1 |= (UCOM_LED1_PIN);	/* Pull-Up or Push-Pull */
		UCOM_LED1_GPIO->CR2 |= (UCOM_LED1_PIN);	/* Output speed up to 10 MHz */
		UCOM_LED1_GPIO->ODR &= ~(UCOM_LED1_PIN); // low... 
	}
  /* Configure PA3 : KEY IN as input push-pull low (led switched on) */
  //GPIO_Init(UCOM_KEY1_GPIO, UCOM_KEY1_PIN, UCOM_KEY1_MODE);
	{											
		UCOM_KEY1_GPIO->DDR &= ~(UCOM_KEY1_PIN);  /* Set input mode */
		UCOM_KEY1_GPIO->CR1 |= (UCOM_KEY1_PIN);	 /* Pull-Up or Push-Pull */
		UCOM_KEY1_GPIO->CR2 &= ~(UCOM_KEY1_PIN); /*  External interrupt disabled */
	}
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#endif // __MIB_DEBUG_WWDG__
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------