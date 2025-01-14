/**
 ******************************************************************************
 * @file    : stm8s_beep_main.c
 * @brief   :
 * @author  : MYMEDIA Co., Ltd.
 * @version : V1.0.0
 * @date    : 2023 : 02 : 09
 ******************************************************************************
 */

// #if defined(__MIB_DEBUG_BEEP_) || defined(__MIB_DEBUG_TEST__)
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
#define UCOM_LED1_GPIO GPIOD		 // PD0
#define UCOM_LED1_PIN GPIO_PIN_0 // PD0
#define UCOM_LED1_MODE GPIO_MODE_OUT_PP_LOW_FAST
#define UCOM_KEY1_GPIO GPIOB								 // PB5
#define UCOM_KEY1_PIN GPIO_PIN_5						 // PB5
#define UCOM_KEY1_MODE GPIO_MODE_IN_PU_NO_IT // GPIO_MODE_IN_FL_NO_IT
#define UCOM_485_GPIO GPIOD									 // PD7
#define UCOM_485_PIN GPIO_PIN_7							 // PD7
#define UCOM_485_MODE GPIO_MODE_OUT_PP_LOW_FAST

#define UCOM_BEEP_GPIO GPIOD		 // PD4
#define UCOM_BEEP_PIN GPIO_PIN_4 // PD4
#define UCOM_BEEP_MODE GPIO_MODE_OUT_PP_LOW_FAST
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void GPIO_Configuration(void);
void calConfigBEEP_test(uint16_t vwDelay);
void Delay(u16 nCount);
extern void fnBeepStart(uint16_t vwFreq, uint16_t vwDelay);
extern void calConfigBEEP(uint16_t vwFreq); // msec...

extern uint16_t gwMibMiliSec;
uint8_t gbDelayData = 0;
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
// PD4 : TIM2_CH1 : BEEP : CN4_#9
/*
	<https://www.devicemart.co.kr/goods/view?no=10916337> Active Buzzer module

	min : 500 Hz
	max : 32000 Hz
	13.3.1 Beeper control/status register (BEEP_CSR)

Address offset: 0x00
Reset value: 0x1F
76543210
BEEPSEL[1:0] BEEPEN BEEPDIV[4:0]
rw rw rw

Bits 7:6 BEEPSEL[1:0]: Beep selection
These bits are set and cleared by software to select 1, 2 or 4 kHz beep output when calibration is
done.
00: fLS/(8 x BEEPDIV) kHz output
01: fLS/(4 x BEEPDIV) kHz output
1x: fLS/(2 x BEEPDIV) kHz output

Bit 5 BEEPEN: Beep enable
This bit is set and cleared by software to enable the beep feature.
0: Beep disabled
1: Beep enabled

Bits 4:0 BEEPDIV[4:0]: Beep prescaler divider
These bits are set and cleared by software to define the Beeper prescaler dividing factor BEEPDIV.
0x00: BEEPDIV = 2
0x01: BEEPDIV = 3
...
0x0E: BEEPDIV = 16
0x0F: BEEPDIV = 17
0x1E: BEEPDIV = 32
Note: This register must not be kept at its reset value (0x1F)

BEEPDIV		BEEPSEL
128000		0	1	2
0	2	8000	16000	32000
1	3	5333.333333	10666.66667	21333.33333
2	4	4000	8000	16000
3	5	3200	6400	12800
4	6	2666.666667	5333.333333	10666.66667
5	7	2285.714286	4571.428571	9142.857143
6	8	2000	4000	8000
7	9	1777.777778	3555.555556	7111.111111
8	10	1600	3200	6400
9	11	1454.545455	2909.090909	5818.181818
10	12	1333.333333	2666.666667	5333.333333
11	13	1230.769231	2461.538462	4923.076923
12	14	1142.857143	2285.714286	4571.428571
13	15	1066.666667	2133.333333	4266.666667
14	16	1000	2000	4000
15	17	941.1764706	1882.352941	3764.705882
16	18	888.8888889	1777.777778	3555.555556
17	19	842.1052632	1684.210526	3368.421053
18	20	800	1600	3200
19	21	761.9047619	1523.809524	3047.619048
20	22	727.2727273	1454.545455	2909.090909
21	23	695.6521739	1391.304348	2782.608696
22	24	666.6666667	1333.333333	2666.666667
23	25	640	1280	2560
24	26	615.3846154	1230.769231	2461.538462
25	27	592.5925926	1185.185185	2370.37037
26	28	571.4285714	1142.857143	2285.714286
27	29	551.7241379	1103.448276	2206.896552
28	30	533.3333333	1066.666667	2133.333333
29	31	516.1290323	1032.258065	2064.516129
30	32	500	1000	2000

*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int DoBEEP(CMD_MY *cptr, int argc, char **argv);
/* Private macro -------------------------------------------------------------*/ /*
	 beep init // gpio init
	 beep remap set // beep pin remap
	 beep freq 1000 // 1000Hz pulse generator..
	 beep enable
	 beep disable
	 beep remap  reset // reset

	 beep init // gpio init
	 beep delay 1000 /// 1000msec per on/off, gpio beep pin test...
	 beep toggle // gpio beep pin toggle... per 1000 msec..
 */
#define CMD_MY_BEEP_ALL                                             \
	{                                                                 \
			"beep",                                                       \
			DoBEEP,                                                       \
			"  beep {write} [addr] [data]  \r\n"                          \
			"  beep {read} [addr] \r\n"                                   \
			"  beep {init} \r\n"                                          \
			"  beep {remap} {set/reset}\r\n"                              \
			"  beep {freq} [data:(?)Hz] \r\n"                             \
			"  beep {enable} \r\n"                                        \
			"  beep {disable} \r\n"                                       \
			"  beep {start} [data:(?)Hz] [delay:(?)msec] 0:continous\r\n" \
			"  beep {toggle} [data:(?)msec] GPIO toggle\r\n"              \
			"  beep {help}  beep help.\r\n",                              \
	},
/* Private variables ---------------------------------------------------------*/
static CMD_MY cmdTbl_only[] = {
		CMD_MY_BEEP_ALL
				CMD_MY_END};
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
void cmd_beep(void)
{
	CMD_MY *cptr;
	uint32_t index;

	index = 0;
	for (cptr = cmdTbl;; cptr++)
	{
		if (cptr->cmd == 0)
		{
			ccprintf(1, ("INFO:+cmd_beep...\r\n"));
			memcpy((void *)&(cptr->cmd), (void *)&(cmdTbl_only[0].cmd), sizeof(CMD_MY));
			break;
		}
		index++;
		if (index > MAX_COMMANDS)
		{
			ccprintf(1, ("INFO:-cmd_beep...\r\n"));
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
	cmd_beep();
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
#define _DEBUG_CMD_A_ 1
#define OPT2_REG 0x4803 // https://www.st.com/resource/en/datasheet/stm8s105c6.pdf  page 49
#define AFR7_BIT 0x80		// 1: Port D4 alternate function = BEEP // AFR7 Alternate function remapping option 7
static int DoBEEP(CMD_MY *cptr, int argc, char **argv)
{
	uint32_t vdwAddr = 0;
	uint8_t vbData = 0;
	uint16_t vwData = 0;
	uint16_t vwDataDelay = 0;

	if (argc < 2)
	{
		MibWriteDebugString(cptr->usage);
		return FALSE;
	}
	// argc=4, argv
	// 0       1       2      3
	// beep {write} [addr] [data]
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
		ccprintf(_DEBUG_CMD_A_, ("wr_ADDR8 : 0x%08lx[0x%02x]\r\n", vdwAddr, vbData));
	}
	// argc=3, argv
	// 0       1       2
	// beep {read} [addr]
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
		ccprintf(_DEBUG_CMD_A_, ("rd_ADDR8 : 0x%08lx[0x%02x]\r\n", vdwAddr, vbData));
	}
	// argc=3, argv
	// 0    1       2
	// beep {freq} [data:(?)Hz]
	else if (!strcmp(argv[1], "freq")) // _CMD_MY_READ_
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
		ccprintf(_DEBUG_CMD_A_, ("beep freq : [%d] Hz\r\n", (vwData)));
		calConfigBEEP_test(vwData);
	}
	// argc, argv
	// 0       1
	// beep {enable}
	else if (!strcmp(argv[1], "enable")) // _CMD_MY_READ_
	{
		/* Enable the BEEP peripheral */
		BEEP->CSR |= BEEP_CSR_BEEPEN;
		ccprintf(_DEBUG_CMD_A_, ("beep enable....(0x%x)\r\n", BEEP->CSR));
	}
	// argc, argv
	// 0       1
	// beep {disable}
	else if (!strcmp(argv[1], "disable")) // _CMD_MY_READ_
	{
		/* Disable the BEEP peripheral */
		BEEP->CSR &= (uint8_t)(~BEEP_CSR_BEEPEN);
		ccprintf(_DEBUG_CMD_A_, ("beep disable....(0x%x)\r\n", BEEP->CSR));
	}
	// argc=3, argv
	// 0    1       2
	//  beep {start} [data:(?)Hz] [delay:(?)msec] 0:continous beep
	else if (!strcmp(argv[1], "start")) // _CMD_MY_READ_
	{
		vwDataDelay = 0;
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
		if (argc >= 4)
		{
			if (!DecToLong(argv[3], &vwDataDelay, 16))
			{
				ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
				return FALSE;
			}
		}
		ccprintf(_DEBUG_CMD_A_, ("beep freq : [%d]Hz\r\n", (vwData)));
		ccprintf(_DEBUG_CMD_A_, ("beep delay : [%d]msec\r\n", (vwDataDelay)));
		fnBeepStart(vwData, vwDataDelay);
#if 0
		GPIO_Configuration();		
		{
			/* set option bytes */ 
			if(FLASH_ReadByte(OPT2_REG)!=AFR7_BIT) 
			{ 
				FLASH_Unlock(FLASH_MEMTYPE_DATA); 
				/* Enable by HW WWDG */ 
				FLASH_ProgramOptionByte(OPT2_REG,AFR7_BIT); 
			} 
			vbData=FLASH_ReadOptionByte(OPT2_REG); 
		}
		ccprintf(_DEBUG_CMD_A_, ("beep start : [%d] Hz\r\n",(vwData)));		
		calConfigBEEP(vwData);
    /* Enable the BEEP peripheral */
    BEEP->CSR |= BEEP_CSR_BEEPEN;
		ccprintf(_DEBUG_CMD_A_, ("beep CSR ....(0x%x)\r\n",BEEP->CSR));
		ccprintf(_DEBUG_CMD_A_, ("beep AFR7....(0x%x)\r\n",rd_ADDR8(0x4803)));
#endif
	}
	// argc=3, argv
	// 0    1       2
	// beep {toggle} [data:(?)msec]
	else if (!strcmp(argv[1], "toggle")) // _CMD_MY_READ_
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
		ccprintf(_DEBUG_CMD_A_, ("beep toggle : [%d] msec\r\n", (vwData)));
		gbDelayData = vwData;
		/* Disable the BEEP peripheral */
		GPIO_Configuration();
		BEEP->CSR &= (uint8_t)(~BEEP_CSR_BEEPEN);
		ccprintf(_DEBUG_CMD_A_, ("beep toggle....(0x%x)\r\n", BEEP->CSR));
		{
			if (UCOM_LED1_GPIO->IDR & UCOM_LED1_PIN)
			{
				UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR & ~UCOM_LED1_PIN;
			}
			else
			{
				UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR | UCOM_LED1_PIN;
			}
		}
		if (gbDelayData == 0)
			gbDelayData = 10;
		gwMibMiliSec = gbDelayData;
		ccprintf(_DEBUG_CMD_A_, ("beep delay [%d]msec\r\n", gbDelayData));
		ccprintf(_DEBUG_CMD_A_, ("stop gpio key push : [%d]\r\n", (UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN)));
		do
		{
			if (gwMibMiliSec == 0)
			{
				gwMibMiliSec = gbDelayData;
				if (UCOM_BEEP_GPIO->IDR & UCOM_BEEP_PIN)
				{
					UCOM_BEEP_GPIO->ODR = UCOM_BEEP_GPIO->ODR & ~UCOM_BEEP_PIN;
				}
				else
				{
					UCOM_BEEP_GPIO->ODR = UCOM_BEEP_GPIO->ODR | UCOM_BEEP_PIN;
				}
			}
			if (UCOM_LED1_GPIO->IDR & UCOM_LED1_PIN)
			{
				UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR & ~UCOM_LED1_PIN;
			}
			else
			{
				UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR | UCOM_LED1_PIN;
			}
			if ((UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN) == 0)
			{
				ccprintf(_DEBUG_CMD_A_, ("gpio key on [%d]\r\n", (UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN)));
				break;
			}
			else
			{
				// ccprintf(_DEBUG_CMD_A_, ("gpio key off [%d]\r\n",(UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN)));
			}
		} while (1);
	}
	// argc, argv
	// 0       1
	// beep {remap}
	else if (!strcmp(argv[1], "remap")) // _CMD_MY_READ_
	{
		if (!strcmp(argv[2], "set"))
		{
			/* set option bytes */
			if (FLASH_ReadByte(OPT2_REG) != AFR7_BIT)
			{
				FLASH_Unlock(FLASH_MEMTYPE_DATA);
				/* Enable by HW WWDG */
				FLASH_ProgramOptionByte(OPT2_REG, AFR7_BIT);
			}
			vbData = FLASH_ReadOptionByte(OPT2_REG);
		}
		else if (!strcmp(argv[2], "reset"))
		{
			/* set option bytes */
			if (FLASH_ReadByte(OPT2_REG) != 0x00)
			{
				FLASH_Unlock(FLASH_MEMTYPE_DATA);
				/* Enable by HW WWDG */
				FLASH_ProgramOptionByte(OPT2_REG, 0x00);
			}
			vbData = FLASH_ReadOptionByte(OPT2_REG);
		}
		else
		{
			ccprintf(_DEBUG_CMD_A_, ("beep init....error !!!\r\n"));
		}
		// AWU_Init(AWU_TIMEBASE_2S);
		ccprintf(_DEBUG_CMD_A_, ("beep init....(0x%02X)\r\n", vbData));
		ccprintf(_DEBUG_CMD_A_, ("beep AFR7....(0x%x)\r\n", rd_ADDR8(OPT2_REG)));
	}
	// argc, argv
	// 0       1
	// beep {init}
	else if (!strcmp(argv[1], "init")) // _CMD_MY_READ_
	{
		GPIO_Configuration();
	}
	// argc, argv
	// 0       1
	// beep {help}
	else if (!strcmp(argv[1], "help")) // _CMD_MY_READ_
	{
		MibWriteDebugString(("beep help....	\r\n"
												 "beep init // gpio init\r\n"
												 "beep remap set // beep pin remap\r\n"
												 "ex1)\r\n"
												 "beep freq 1000 // 1000Hz pulse generator..\r\n"
												 "beep enable\r\n"
												 "beep disable\r\n"
												 "beep remap  reset // reset\r\n"
												 "ex2)\r\n"
												 "beep start  4000  200// 4KHz freq 200 msec delay\r\n"
												 "beep toggle 1000 /// 500msec per on/off, gpio beep pin test...\r\n"
												 "// gpio beep pin toggle... per 1000 msec..\r\n"));
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
	// GPIO_Init(UCOM_LED1_GPIO, UCOM_LED1_PIN, UCOM_LED1_MODE);
	{
		UCOM_LED1_GPIO->DDR |= (UCOM_LED1_PIN);	 /* Set Output mode */
		UCOM_LED1_GPIO->CR1 |= (UCOM_LED1_PIN);	 /* Pull-Up or Push-Pull */
		UCOM_LED1_GPIO->CR2 |= (UCOM_LED1_PIN);	 /* Output speed up to 10 MHz */
		UCOM_LED1_GPIO->ODR &= ~(UCOM_LED1_PIN); // low...
	}
	/* Configure PA3 : KEY IN as input push-pull low (led switched on) */
	// GPIO_Init(UCOM_KEY1_GPIO, UCOM_KEY1_PIN, UCOM_KEY1_MODE);
	{
		UCOM_KEY1_GPIO->DDR &= ~(UCOM_KEY1_PIN); /* Set input mode */
		UCOM_KEY1_GPIO->CR1 |= (UCOM_KEY1_PIN);	 /* Pull-Up or Push-Pull */
		UCOM_KEY1_GPIO->CR2 &= ~(UCOM_KEY1_PIN); /*  External interrupt disabled */
	}
	/* Configure PD4 (BEEPER) as output push-pull low */
	// GPIO_Init(GPIOD, GPIO_PIN_4 , GPIO_MODE_OUT_PP_LOW_FAST);
	// PD4 : BEEP
	/* Configure PD4 (BEEP) as output push-pull low (led switched on) */
	// GPIO_Init(UCOM_BEEP_GPIO, UCOM_BEEP_PIN, UCOM_BEEP_MODE);
	{
		UCOM_BEEP_GPIO->DDR |= (UCOM_BEEP_PIN);	 /* Set Output mode */
		UCOM_BEEP_GPIO->CR1 |= (UCOM_BEEP_PIN);	 /* Pull-Up or Push-Pull */
		UCOM_BEEP_GPIO->CR2 |= (UCOM_BEEP_PIN);	 /* Output speed up to 10 MHz */
		UCOM_BEEP_GPIO->ODR &= ~(UCOM_BEEP_PIN); // low...
	}
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void calConfigBEEP_test(uint16_t vwFreq) // msec...
{
	uint8_t vbBEEPSEL = 0;
	uint8_t vbBEEPDIV = 0;
	uint32_t vdwData = vwFreq;
	uint32_t vdwDataLast = 0;
	uint8_t vdwDiv = 0;
	if (vwFreq < 500)
	{
		// error...
		vbBEEPSEL = 0xFF;
	}
	else if (vwFreq < 1000)
	{
		vbBEEPSEL = 0x00; // f / (8 * div) khz
		vdwDiv = (128000 / 8) / vdwData;
		vbBEEPDIV = vdwDiv - 2;
		vdwDataLast = (128000 / 8) / vdwDiv;
	}
	else if (vwFreq < 2000)
	{
		vbBEEPSEL = 0x01; // f / (4 * div) khz
		vdwDiv = (128000 / 4) / vdwData;
		vbBEEPDIV = vdwDiv - 2;
		vdwDataLast = (128000 / 8) / vdwDiv;
	}
	else if (vwFreq < 32000)
	{
		vbBEEPSEL = 0x02; // f / (2 * div) khz
		vdwDiv = (128000 / 2) / vdwData;
		vbBEEPDIV = vdwDiv - 2;
		vdwDataLast = (128000 / 8) / vdwDiv;
	}
	else
	{
		// error...
		vbBEEPSEL = 0xFF;
	}
	if (vbBEEPSEL != 0xFF)
	{
		ccprintf(_DEBUG_CMD_A_, ("beep init [%d : SEL(%d) : DIV(%d)] Hz\r\n", vwFreq, vbBEEPSEL, vbBEEPDIV));
		/* Set a default calibration value if no calibration is done */
		// if ((BEEP->CSR & BEEP_CSR_BEEPDIV) == BEEP_CSR_BEEPDIV)
		BEEP->CSR = BEEP_CSR_BEEPDIV;
		{
			BEEP->CSR &= (uint8_t)(~BEEP_CSR_BEEPDIV); /* Clear bits */
			BEEP->CSR |= (vbBEEPDIV & BEEP_CSR_BEEPDIV);
		}
		{
			/* Select the output frequency */
			BEEP->CSR &= (uint8_t)(~BEEP_CSR_BEEPSEL);
			BEEP->CSR |= (uint8_t)((vbBEEPSEL & 0x03) << 6);
		}
	}
	else
	{
		BEEP->CSR = BEEP_CSR_BEEPDIV;
		ccprintf(_DEBUG_CMD_A_, ("beep init error... [%d] Hz\r\n", vwFreq));
	}
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/**
 * @brief Delay.
 * @param[in] nCount
 * @retval
 * None
 */
void Delay(u16 nCount)
{
	/* Decrement nCount value */
	while (nCount != 0)
	{
		nCount--;
	}
}
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
