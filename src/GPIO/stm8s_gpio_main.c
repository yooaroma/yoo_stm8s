/**
	******************************************************************************
	* @file    : stm8s_gpio_main.c
	* @brief   : 
	* @author  : MYMEDIA Co., Ltd.
	* @version : V1.0.0
	* @date    : 2023 : 02 : 09
	******************************************************************************
	*/
/*
  compile : make
  write   : ST Visual Programmer 
  exe     : ./exe/stm8s_gpio.hex
  src : ./mib/stm8s_mib_it.c
        ./mib/stm8s_mib_debug.c
        ./mib/stm8s_mib_vsprintf.c
        ./mib/stm8s_mib_debug_tim.c
        ./mib/stm8s_mib_debug_cmd.c
        ./mib/stm8s_mib_debug_mem.c
        ./mib/stm8s_mib_debug_top.c
        
  info :
    STM8S Reference Manuals : <https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf>
    page 105, 참고한다.
    stm8s 가 지니고 있는 gpio 기능에 대해서 공부한다.
    이제 부터는 uart 와 디버거를 문자를 주고 받으면서 기능 테스트를 할 예정이다.    
*/   

// #if defined(__MIB_DEBUG_GPIO__) || defined(__MIB_DEBUG_TEST__)
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
void Toggle(void);
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int DoGPIO(CMD_MY * cptr, int argc, char **argv);
/* Private macro -------------------------------------------------------------*/
#define CMD_MY_GPIO_ALL	\
{   "gpio", DoGPIO,		\
	"  gpio {init}  : gpio in/out reg init..\r\n"\
	"  gpio {write} [addr] [data] : (addr 16bit) write (data 8bit)\r\n"\
	"  gpio {read} [addr] : (addr 16bit) read (data 8bit)\r\n"\
	"  gpio {led} [data:on/off(1/0)] : Write 0 or 1 to the PD0\r\n"\
	"  gpio {key} : read to the PB5\r\n"\
	"  gpio {in}  {port : a..g} {bit : 0..7} : Read for each bit 0 to 7 from ports A GPIO to G GPIO. \r\n"\
	"  gpio {out} {port : a..g} {bit : 0..7} {0/1} : It writes to each bit 0 to 7 from ports a to g.\r\n"\
	"  gpio {help}  gpio help.\r\n",\
},
/* Private variables ---------------------------------------------------------*/
static CMD_MY cmdTbl_only[] = {
		CMD_MY_GPIO_ALL
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
void cmd_gpio(void)
{
	CMD_MY *cptr;
	uint32_t index;

	index = 0;
	for (cptr = cmdTbl;; cptr++) 
	{
		if(cptr->cmd==0)
		{
			ccprintf(1,("INFO:+cmd_gpio...\r\n"));
			memcpy((void *)&(cptr->cmd),(void *)&(cmdTbl_only[0].cmd),sizeof(CMD_MY));
			break;
		}
		index++;
		if(index>MAX_COMMANDS)
		{ 
			ccprintf(1,("INFO:-cmd_gpio...\r\n"));
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
  cmd_gpio();
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
static int DoGPIO(CMD_MY *cptr, int argc, char **argv)
{
	uint16_t vwAddr = 0;
	uint8_t vbData = 0;

	if (argc < 2)
	{
		MibWriteDebugString(cptr->usage);
		return FALSE;
	}	
  // argc=4, argv
  // 0       1       2      3
  // gpio {write} [addr] [data]  
	if (!strcmp(argv[1], "write")) // _CMD_MY_WRITE_
	{
		if (argc < 4)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		if (!HexToInt(argv[2], &vwAddr, 16))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}
		if (!HexToInt(argv[3], &vbData, 8))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}    
		wr_ADDR8(vwAddr, vbData);
		ccprintf(_DEBUG_CMD_A_,("wr_ADDR8 : 0x%04x[0x%02x]\r\n", vwAddr,vbData));
	}
  // argc=3, argv
  // 0       1       2      
  // gpio {read} [addr]
	else if (!strcmp(argv[1], "read")) // _CMD_MY_READ_
	{
		if (argc < 3)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		if (!HexToInt(argv[2], &vwAddr, 16))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}
    {      
			vbData = rd_ADDR8(vwAddr);
    }   
		ccprintf(_DEBUG_CMD_A_,("rd_ADDR8 : 0x%04x[0x%02x]\r\n", vwAddr,vbData));
	}	  
  // argc=4, argv
  // 0     1     2      	          3
	// gpio {in}  {port : a/b/c/d/e} {bit : 0..7}
	else if (!strcmp(argv[1], "in")) // _CMD_MY_READ_
	{
		char vbPort = *(char *)(argv[2]); // argv[2][0]
		char vbNum = *(char *)(argv[3]);
		char vbBit = 0;
		GPIO_TypeDef* GPIOx=GPIOA;
		if (argc < 4)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		} 
		if((vbPort>='a')&&(vbPort<='g'))
		{
			switch(vbPort)
			{
			case 'a': GPIOx = GPIOA; break;
			case 'b': GPIOx = GPIOB; break;
			case 'c': GPIOx = GPIOC; break;
			case 'd': GPIOx = GPIOD; break;
			case 'e': GPIOx = GPIOE; break;
			case 'f': GPIOx = GPIOF; break;
			case 'g': GPIOx = GPIOG; break;
			// case 'h': GPIOx = GPIOH; break;
			// case 'i': GPIOx = GPIOI; break;
			default: break;
			}
			if((vbNum>='0')&&(vbNum<='7'))
			{					
				vbNum &= 0x07;
				GPIO_Init(GPIOx,vbNum,GPIO_MODE_IN_FL_NO_IT);
				vbBit = GPIOx->IDR >> vbNum;
				vbBit &= 1;
				ccprintf(_DEBUG_CMD_A_,("gpio in : port[%c] num[%d] bit[%d]\r\n", vbPort,vbNum,vbBit));
			}
		}
	}	 
  // argc=5, argv
  // 0     1     2      	          3            4
	// gpio {out} {port : a/b/c/d/e} {bit : 0..7} {0/1}
	else if (!strcmp(argv[1], "out")) // _CMD_MY_READ_
	{
		char vbPort = *(char *)(argv[2]); // argv[2][0]
		char vbNum = *(char *)(argv[3]);
		char vbBit = *(char *)(argv[4]);
		GPIO_TypeDef* GPIOx=GPIOA;
		if (argc < 5)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		} 
		ccprintf(_DEBUG_CMD_A_,("gpio in : port[%c] num[%c] bit[%c]\r\n", vbPort,vbNum,vbBit));
		if((vbPort>='a')&&(vbPort<='g'))
		{
			switch(vbPort)
			{
			case 'a': GPIOx = GPIOA; break;
			case 'b': GPIOx = GPIOB; break;
			case 'c': GPIOx = GPIOC; break;
			case 'd': GPIOx = GPIOD; break;
			case 'e': GPIOx = GPIOE; break;
			case 'f': GPIOx = GPIOF; break;
			case 'g': GPIOx = GPIOG; break;
			// case 'h': GPIOx = GPIOH; break;
			// case 'i': GPIOx = GPIOI; break;
			default: break;
			}
			if((vbNum>='0')&&(vbNum<='7'))
			{			
				vbNum &= 0x07;
				if((vbBit>='0')&&(vbBit<='1'))
				{					
					vbBit &= 0x01;
					vbNum = 1 << vbNum;
					GPIO_Init(GPIOx,vbNum,GPIO_MODE_OUT_PP_LOW_FAST);
					if(vbBit==1)
					{
						GPIOx->ODR |= vbNum;
					}
					else
					{
						GPIOx->ODR &= ~vbNum;
					}
				}
			}
		}
	}	 
  // argc=3, argv
  // 0    1       2      
  // gpio {led} [data:on/off(1/0)] 
	else if (!strcmp(argv[1], "led")) // _CMD_MY_READ_
	{
		if (argc < 3)
		{
			MibWriteDebugString(cptr->usage);
			return FALSE;
		}
		if (!HexToInt(argv[2], &vbData, 8))
		{
			ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
			return FALSE;
		}
		if(vbData)
		{			
      UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR & ~UCOM_LED1_PIN;
		}
		else
		{
      UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR | UCOM_LED1_PIN;
		}
		ccprintf(_DEBUG_CMD_A_, ("gpio led [%d]\r\n",(vbData)));		
	}
  // argc, argv
  // 0       1           
  // gpio {key}
	else if (!strcmp(argv[1], "key")) // _CMD_MY_READ_
	{
    if((UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN)==0)
    {      
			ccprintf(_DEBUG_CMD_A_, ("gpio key on [%d]\r\n",(UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN)));		
    }
		else
    {      
			ccprintf(_DEBUG_CMD_A_, ("gpio key off [%d]\r\n",(UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN)));		
    }
	}	 
  // argc, argv
  // 0       1           
  // gpio {init}
	else if (!strcmp(argv[1], "init")) // _CMD_MY_READ_
	{
		ccprintf(_DEBUG_CMD_A_, ("gpio init....GPIO_Configuration()\r\n"));		
		{
			GPIO_Configuration();
		}
	}	  
  // argc, argv
  // 0       1           
  // gpio {help}
	else if (!strcmp(argv[1], "help")) // _CMD_MY_READ_
	{
		ccprintf(_DEBUG_CMD_A_, ("gpio help....argc[%x:%d]\r\n",argc,argc));		
		ccprintf(_DEBUG_CMD_A_, ("https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf page 105.\r\n"));		
    // https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf page 105.
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
  /* GPIOD reset */
  GPIO_DeInit(UCOM_LED1_GPIO);
  /* Configure PD0 (LED1) as output push-pull low (led switched on) */
  GPIO_Init(UCOM_LED1_GPIO, UCOM_LED1_PIN, UCOM_LED1_MODE);  
  GPIO_DeInit(UCOM_KEY1_GPIO);
  /* Configure PB5 : KEY IN as input push-pull low (led switched on) */
  GPIO_Init(UCOM_KEY1_GPIO, UCOM_KEY1_PIN, UCOM_KEY1_MODE);
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/**
  ******************************************************************************
  * @brief Toggle PD0 (Led LD1)
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void Toggle(void)
{
  GPIO_WriteReverse(UCOM_LED1_GPIO, UCOM_LED1_PIN);
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#endif // __MIB_DEBUG_GPIO__
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
