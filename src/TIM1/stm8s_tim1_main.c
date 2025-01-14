/**"0x00:__IO"0x00:__IO
 ******************************************************************************
 * @file    : stm8s_tim1_main.c
 * @brief   :
 * @author  : MYMEDIA Co., Ltd.
 * @version : V1.0.0
 * @date    : 2023 : 02 : 09
 ******************************************************************************
 */

/*
  * <https://community.st.com/s/question/0D50X00009XkgR7SAJ/stm8s-tim1-pwm2-not-working>  
  * STM8S Reference Manuals : <https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf>
  * page 138.
  * 1. one pulse를 만드는 방법
  * 2. PWM pulse를 만드는 방법 
  * 3. 리모콘 캡쳐 프로그램
  * 4. 리모콘 신호 생성 프로그램
  * TIM1 consists of a 16-bit up-down auto-reload counter driven by a programmable prescaler.
    The timer may be used for a variety of purposes, including:
    • Time base generation
    • Measuring the pulse lengths of input signals (input capture)
    • Generating output waveforms (output compare, PWM and one-pulse mode)
    • Interrupt capability on various events (capture, compare, overflow, break, trigger)
    • Synchronization with TIM5/TIM6 timers or external signals (external clock, reset, trigger and enable).
    This timer is ideally suited for a wide range of control applications, including those requiring
    center-aligned PWM capability with complementary outputs and deadtime insertion.
    The timer clock can be sourced from internal clocks or from an external source selectable
    through a configuration register.
*/

// #if defined(__MIB_DEBUG_TIM1__) || defined(__MIB_DEBUG_TEST__)
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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
static const char *gpbHelp_TIM1_TypeDef =
    "typedef struct TIM1_struct\r\n"
    "{\r\n"
    "0x00:__IO uint8_t CR1;   /*!< control register 1 */\r\n"
    "0x01:__IO uint8_t CR2;   /*!< control register 2 */\r\n"
    "0x02:__IO uint8_t SMCR;  /*!< Synchro mode control register */\r\n"
    "0x03:__IO uint8_t ETR;   /*!< external trigger register */\r\n"
    "0x04:__IO uint8_t IER;   /*!< interrupt enable register*/\r\n"
    "0x05:__IO uint8_t SR1;   /*!< status register 1 */\r\n"
    "0x06:__IO uint8_t SR2;   /*!< status register 2 */\r\n"
    "0x07:__IO uint8_t EGR;   /*!< event generation register */\r\n"
    "0x08:__IO uint8_t CCMR1; /*!< CC mode register 1 */\r\n"
    "0x09:__IO uint8_t CCMR2; /*!< CC mode register 2 */\r\n"
    "0x0a:__IO uint8_t CCMR3; /*!< CC mode register 3 */\r\n"
    "0x0b:__IO uint8_t CCMR4; /*!< CC mode register 4 */\r\n"
    "0x0c:__IO uint8_t CCER1; /*!< CC enable register 1 */\r\n"
    "0x0d:__IO uint8_t CCER2; /*!< CC enable register 2 */\r\n"
    "0x0e:__IO uint8_t CNTRH; /*!< counter high */\r\n"
    "0x0f:__IO uint8_t CNTRL; /*!< counter low */\r\n"
    "0x10:__IO uint8_t PSCRH; /*!< prescaler high */\r\n"
    "0x11:__IO uint8_t PSCRL; /*!< prescaler low */\r\n"
    "0x12:__IO uint8_t ARRH;  /*!< auto-reload register high */\r\n"
    "0x13:__IO uint8_t ARRL;  /*!< auto-reload register low */\r\n"
    "0x14:__IO uint8_t RCR;   /*!< Repetition Counter register */\r\n"
    "0x15:__IO uint8_t CCR1H; /*!< capture/compare register 1 high */\r\n"
    "0x16:__IO uint8_t CCR1L; /*!< capture/compare register 1 low */\r\n"
    "0x17:__IO uint8_t CCR2H; /*!< capture/compare register 2 high */\r\n"
    "0x18:__IO uint8_t CCR2L; /*!< capture/compare register 2 low */\r\n"
    "0x19:__IO uint8_t CCR3H; /*!< capture/compare register 3 high */\r\n"
    "0x1a:__IO uint8_t CCR3L; /*!< capture/compare register 3 low */\r\n"
    "0x1b:__IO uint8_t CCR4H; /*!< capture/compare register 3 high */\r\n"
    "0x1c:__IO uint8_t CCR4L; /*!< capture/compare register 3 low */\r\n"
    "0x1d:__IO uint8_t BKR;   /*!< Break Register */\r\n"
    "0x1e:__IO uint8_t DTR;   /*!< dead-time register */\r\n"
    "0x1f:__IO uint8_t OISR;  /*!< Output idle register */\r\n"
    "}\r\n"
    "TIM1_TypeDef;\r\n";
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/* Private functions ---------------------------------------------------------*/
static int DoTIM1(CMD_MY *cptr, int argc, char **argv);
/* Private macro -------------------------------------------------------------*/
#define CMD_MY_TIM1_ALL                                         \
  {                                                             \
      "tim1",                                                   \
      DoTIM1,                                                   \
      "  tim1 {init} \r\n"                                      \
      "  tim1 {write} [addr] [data]  \r\n"                      \
      "  tim1 {read} [addr] \r\n"                               \
      "  tim1 {onepulse} {high/low} [delay] [width] usec..\r\n" \
      "  tim1 {pwm} [period] [ratio] {N(1/2/3/4):Pin(p/n/b):(h/l)}} usec \r\n"  \
      "  tim1 {compare} [period] usec \r\n"                     \
      "  tim1 {capture} \r\n"                                   \
      "  tim1 {remap} {set/reset} \r\n"                         \
      "  tim1 {reset}\r\n"                                      \
      "  tim1 {help}  tim1 help.\r\n",                          \
  },
/* Private variables ---------------------------------------------------------*/
static CMD_MY cmdTbl_only[] = {
    CMD_MY_TIM1_ALL
        CMD_MY_END};
/* Public functions ----------------------------------------------------------*/
extern CMD_ARRAY cmd_list;
extern CMD_MY cmdTbl[];
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
int mmTim1Onepulse(uint8_t vbPulseState, uint32_t vdwDataDelay, uint32_t vdwDataWidth);
int mmTim1CalPscrCntr(uint32_t vdwDelayUsec, uint16_t *pvwPSCR, uint16_t *pvwCNTR);
int mmTim1PWM(uint32_t vdwDataPeriod, uint8_t vbDataRatio, uint8_t vbChannelNumber, uint8_t vbPinUseFlag, uint8_t vbLevelState, uint8_t vbPulseState);
void mmRemapCH1N_CH2N_CH3N_ETR(void);
int mmTim1GpioInit(void);
#if defined(__IRQ_TIM1_UPD_OVF_CALLBACK_USE__)
void irqTim1UpdOvf_CALLBACK(void);
#endif
#if defined(__IRQ_TIM1_CAP_CALLBACK_USE__)
void irqTim1Cap_CALLBACK(void);
#endif
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/**
 * @brief
 * @par Parameters: None
 * @retval None
 */
void cmd_tim1(void)
{
  CMD_MY *cptr;
  uint32_t index;

  index = 0;
  for (cptr = cmdTbl;; cptr++)
  {
    if (cptr->cmd == 0)
    {
      ccprintf(1, ("INFO:+cmd_tim1...\r\n"));
      memcpy((void *)&(cptr->cmd), (void *)&(cmdTbl_only[0].cmd), sizeof(CMD_MY));
      break;
    }
    index++;
    if (index > MAX_COMMANDS)
    {
      ccprintf(1, ("INFO:-cmd_tim1...\r\n"));
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
  cmd_tim1();
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
static int DoTIM1(CMD_MY *cptr, int argc, char **argv)
{
  uint32_t vdwAddr = 0;
  uint8_t vbData = 0;
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
  // argc=5, argv
  // 0       1          2          3           4
  //"  tim1 {onepulse} {high/low} [delay] [width] usec \r\n"
  else if (!strcmp(argv[1], "onepulse"))
  {
    uint8_t vbPulseState = 1; // high
    uint32_t vdwDataDelay = 0;
    uint32_t vdwDataWidth = 0;
    if (argc < 5)
    {
      MibWriteDebugString(cptr->usage);
      return FALSE;
    }
    if (!strcmp(argv[2], "low"))
    {
      vbPulseState = 0;
    }
    else
    {
      vbPulseState = 1;
    }
    if (!DecToLong(argv[3], &vdwDataDelay, 32))
    {
      ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
      return FALSE;
    }
    if (!DecToLong(argv[4], &vdwDataWidth, 32))
    {
      ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
      return FALSE;
    }
    mmTim1GpioInit();
    mmRemapCH1N_CH2N_CH3N_ETR();
    mmTim1Onepulse(vbPulseState, vdwDataDelay, vdwDataWidth);
  }
  // argc=5, argv
  // 0       1     2        3       4
  //"  tim1 {pwm} [period] [ratio] {N(1/2/3/4):Pin(p/n/b):(h/l)}} usec \r\n"
  else if (!strcmp(argv[1], "pwm"))
  {
    uint32_t vdwDataPeriod = 0;
    uint8_t vbDataRatio = 0;
    uint8_t vbChannelNumber = TIM1_CHANNEL_1;
    uint8_t vbPinUseFlag = 1; // 1 : P, 2 : N, 3 : BOTH
    uint8_t vbLevelState = TIM1_OCPOLARITY_LOW; // 0 : LOW, 1 : HIGH
    
    if (argc < 3)
    {
      MibWriteDebugString(cptr->usage);
      return FALSE;
    }
    if(argc < 4)
    {
      vbChannelNumber = 1; // ch1
      vbPinUseFlag = 1; // Plus
      vbLevelState = TIM1_OCPOLARITY_LOW;
    }
    else
    {
      if (argv[4][0]=='1')
      {
        vbChannelNumber = TIM1_CHANNEL_1;
      }
      else if (argv[4][0]=='2')
      {
        vbChannelNumber = TIM1_CHANNEL_2;
      }
      else if (argv[4][0]=='3')
      {
        vbChannelNumber = TIM1_CHANNEL_3;
      }
      else if (argv[4][0]=='4')
      {
        vbChannelNumber = TIM1_CHANNEL_4;
      }
      else 
      {
        vbChannelNumber = TIM1_CHANNEL_1;
      }

      if (argv[4][1]=='p')
      {
        vbPinUseFlag = 1;
      }
      else if (argv[4][1]=='n')
      {
        vbPinUseFlag = 2;
      }
      else if (argv[4][1]=='b')
      {
        vbPinUseFlag = 3;
      }
      else 
      {
        vbPinUseFlag = 1;
      }

      if (argv[4][2]=='h')
      {
        vbLevelState = TIM1_OCPOLARITY_HIGH;
      }
      else if (argv[4][2]=='l')
      {
        vbLevelState = TIM1_OCPOLARITY_LOW;
      }
      else 
      {
        vbLevelState = TIM1_OCPOLARITY_LOW;
      }
    }
    if (!DecToLong(argv[2], &vdwDataPeriod, 32))
    {
      ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
      return FALSE;
    }
    if (!DecToLong(argv[3], &vbDataRatio, 8))
    {
      ccprintf(_DEBUG_CMD_A_, ("Illugal character is used.\r\n"));
      return FALSE;
    }
    ccprintf(_DEBUG_CMD_A_, ("mmTim1 : vdwDataPeriod (0x%08lx) vbDataRatio (%d)\r\n", vdwDataPeriod, vbDataRatio));
    ccprintf(_DEBUG_CMD_A_, ("mmTim1 : vbChannelNumber (%d) vbPinUseFlag (%d) vbLevelState (%d)\r\n", vbChannelNumber, vbPinUseFlag, vbLevelState));
    mmTim1GpioInit();
    mmRemapCH1N_CH2N_CH3N_ETR();
    mmTim1PWM(vdwDataPeriod, vbDataRatio, vbChannelNumber, vbPinUseFlag,vbLevelState,0);
  }  
  // argc=3, argv
  // 0       1      2     
  // tim1 {remap} {set/reset}
	else if (!strcmp(argv[1], "remap")) // _CMD_MY_READ_
	{
    /*
      AFR5 Alternate function remapping option 5
        0: AFR5 remapping option inactive: Default alternate function.(2)
        1: Port B3 alternate function = TIM1_ETR; port B2 alternate function = TIM1_NCC3; port B1 alternate function = TIM1_CH2N; port B0 alternate function = TIM1_CH1N.     
    */
		#define OPT2_REG 0x4803  // https://www.st.com/resource/en/datasheet/stm8s105c6.pdf  page 49
		#define AFR5_BIT 0x20  // 1 : port B0 alternate function = TIM1_CH1N.// AFR5 Alternate function remapping option 5
		if (!strcmp(argv[2], "set"))
		{
			/* set option bytes */ 
			if(FLASH_ReadByte(OPT2_REG)!=AFR5_BIT) 
			{ 
				FLASH_Unlock(FLASH_MEMTYPE_DATA); 
				/* Enable by HW WWDG */ 
				FLASH_ProgramOptionByte(OPT2_REG,AFR5_BIT); 
			} 
			vbData=FLASH_ReadOptionByte(OPT2_REG); 
		}
		else if (!strcmp(argv[2], "reset"))
		{
			/* set option bytes */ 
			if(FLASH_ReadByte(OPT2_REG)!=0x00) 
			{ 
				FLASH_Unlock(FLASH_MEMTYPE_DATA); 
				/* Enable by HW WWDG */ 
				FLASH_ProgramOptionByte(OPT2_REG,0x00); 
			} 
			vbData=FLASH_ReadOptionByte(OPT2_REG); 
		}
		else 
		{
			ccprintf(_DEBUG_CMD_A_, ("tim1 init....error !!!\r\n"));		
		}
		//AWU_Init(AWU_TIMEBASE_2S);
		ccprintf(_DEBUG_CMD_A_, ("tim1 init....(0x%02X)\r\n",vbData));		
		ccprintf(_DEBUG_CMD_A_, ("tim1 AFR5....(0x%x)\r\n",rd_ADDR8(OPT2_REG)));		
	}	 
  // argc, argv
  // 0       1
  // tim1 {init}
  else if (!strcmp(argv[1], "init")) // _CMD_MY_READ_
  {
    mmTim1GpioInit();
  }
  // argc, argv
  // 0       1
  // tim1 {init}
  else if (!strcmp(argv[1], "reset")) // _CMD_MY_READ_
  {
    TIM1_DeInit();
    {
			/* set option bytes */ 
			if(FLASH_ReadByte(OPT2_REG)!=0x00) 
			{ 
				FLASH_Unlock(FLASH_MEMTYPE_DATA); 
				/* Enable by HW WWDG */ 
				FLASH_ProgramOptionByte(OPT2_REG,0x00); 
			} 
			vbData=FLASH_ReadOptionByte(OPT2_REG); 
      ccprintf(_DEBUG_CMD_A_, ("tim1 reset....(0x%02X)\r\n",vbData));		
      ccprintf(_DEBUG_CMD_A_, ("tim1 AFR5....(0x%x)\r\n",rd_ADDR8(OPT2_REG)));		
    }
  }
  // argc, argv
  // 0       1
  // tim1 {help}
  else if (!strcmp(argv[1], "help")) // _CMD_MY_READ_
  {
    // 2594,9: #define TIM1_BaseAddress        0x5250
    ccprintf(_DEBUG_CMD_A_, ("TIM1_BaseAddress : 0x%04x\r\n", TIM1_BaseAddress));
    MibWriteDebugString((uint8_t *)gpbHelp_TIM1_TypeDef);
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
/*
     이곳이 기능적인 역활을 하는 함수 들이다.
*/
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#if defined(__IRQ_TIM1_UPD_OVF_CALLBACK_USE__)
void irqTim1UpdOvf_CALLBACK(void)
{
}
#endif
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#if defined(__IRQ_TIM1_CAP_CALLBACK_USE__)
void irqTim1Cap_CALLBACK(void)
{
}
#endif
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#define UCOM_LED1_GPIO GPIOD     // PD0
#define UCOM_LED1_PIN  GPIO_PIN_0 // PD0
#define UCOM_LED1_MODE GPIO_MODE_OUT_PP_LOW_FAST
#define UCOM_KEY1_GPIO GPIOB      // PB5
#define UCOM_KEY1_PIN  GPIO_PIN_5 // PB5
#define UCOM_KEY1_MODE GPIO_MODE_IN_FL_NO_IT
int mmTim1Onepulse(uint8_t vbPulseState, uint32_t vdwDataDelay, uint32_t vdwDataWidth)
{
  uint32_t vdwPeriod = 0;
  uint8_t vbDataRatio = 0;
  {
    // pd0 : log to high
    UCOM_LED1_GPIO->ODR &= (uint8_t)(~(UCOM_LED1_PIN)); /* Output mode  Low level */
    //UCOM_LED1_GPIO->ODR |= (uint8_t)((UCOM_LED1_PIN)); /* Output mode  high level */
  }
  {
    vdwPeriod = vdwDataDelay + vdwDataWidth;
    vbDataRatio = (vdwDataWidth * 100) / vdwPeriod;
  }
  {
    uint8_t vbChannelNumber = TIM1_CHANNEL_1;
    uint8_t vbLevelState = TIM1_OCPOLARITY_HIGH;
    uint8_t vbPinUseFlag = 3; // Plus Pin... both...
    if(vbPulseState)
    {
      vbLevelState = TIM1_OCPOLARITY_HIGH;
    }
    else
    {
      vbLevelState = TIM1_OCPOLARITY_LOW;
    }
    vbPulseState = (vbPulseState<<1) | 0x1;
    ccprintf(_DEBUG_CMD_A_, ("mmTim1Onepulse : vdwPeriod (0x%08lx) vbDataRatio (%d)\r\n", vdwPeriod, vbDataRatio));
    {
      // pd0 : log to high
      //UCOM_LED1_GPIO->ODR &= (uint8_t)(~(UCOM_LED1_PIN)); /* Output mode  Low level */
      UCOM_LED1_GPIO->ODR |= (uint8_t)((UCOM_LED1_PIN)); /* Output mode  high level */
    }
    mmTim1PWM(vdwPeriod, vbDataRatio, vbChannelNumber, vbPinUseFlag,vbLevelState,vbPulseState);
  }
  return 0;
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
int mmTim1PWM(uint32_t vdwDataPeriod, uint8_t vbDataRatio, uint8_t vbChannelNumber, uint8_t vbPinUseFlag, uint8_t vbLevelState, uint8_t vbPulseState)
{
  uint16_t vwPSCR = 0;
  uint16_t vwCNTR = 0;
  uint32_t vdwRatioTemp = 0;
  uint16_t vwRatioReg = 0;
  
  mmTim1CalPscrCntr(vdwDataPeriod, (uint16_t *)&vwPSCR, (uint16_t *)&vwCNTR);
  vdwRatioTemp = vwCNTR;
  vdwRatioTemp = vdwRatioTemp * vbDataRatio;
  vdwRatioTemp = vdwRatioTemp / 100;
  vwRatioReg = (uint16_t)vdwRatioTemp;
  ccprintf(_DEBUG_CMD_A_, ("mmTim1PWM : vdwDataPeriod (0x%08lx) vbDataRatio (%d)\r\n", vdwDataPeriod, vbDataRatio));
  ccprintf(_DEBUG_CMD_A_, ("mmTim1PWM : vwPSCR (0x%04x) vwCNTR (0x%04x) vwRatioReg (0x%04x)\r\n", vwPSCR, vwCNTR,vwRatioReg));

#if 1
  { // tim1 reg init
    /*
      <https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf>
      page 175
      PWM edge-aligned mode
        Up-counting configuration
        Up-counting is active when the DIR bit in the TIM1_CR1 register is low.
      Example
        This example uses PWM mode 1. The reference PWM signal, OCiREF, is high as long as
        TIM1_CNT < TIM1_CCRi. Otherwise, it becomes low. If the compare value in TIM1_CCRi is
        greater than the auto-reload value (in TIM1_ARR) then OCiREF is held at 1. If the compare
        value is 0, OCiREF is held at 0. Figure 70 shows some edge-aligned PWM waveforms in an
        example where TIM1_ARR = 8.

        Down-counting configuration
        Down-counting is active when the DIR bit in the TIM1_CR1 register is high. Refer to Downcounting mode on page 145
        In PWM mode 1, the reference signal OCiREF is low as long as TIM1_CNT> TIM1_CCRi.
        Otherwise, it becomes high. If the compare value in the TIM1_CCRi registers is greater than
        the auto-reload value in the TIM1_ARR register, OCiREF is held at 1. Zero percent PWM is
        not possible in this mode.

    */
    TIM1_DeInit();
    TIM1_TimeBaseInit(vwPSCR, TIM1_COUNTERMODE_UP, vwCNTR, 0);
    if(0)
    {
      TIM1->ARRH = (uint8_t)((vwCNTR >> 8) & 0xff);
      TIM1->ARRL = (uint8_t)((vwCNTR >> 0) & 0xff);
      TIM1->PSCRH = (uint8_t)((vwPSCR >> 8) & 0xff);
      TIM1->PSCRL = (uint8_t)((vwPSCR >> 0) & 0xff);
      TIM1->CNTRH = (uint8_t)((vwCNTR >> 8) & 0xff);
      TIM1->CNTRL = (uint8_t)((vwCNTR >> 0) & 0xff);
      TIM1->RCR = 0x00;
      TIM1->CR1  |= (TIM1_COUNTERMODE_UP); // Edge-aligned mode...
    }
    // TIM1_SelectOCxM(TIM1_CHANNEL_1, TIM1_OCMODE_PWM2);
    TIM1_SelectOCxM(vbChannelNumber, TIM1_OCMODE_PWM2);
    if(0)
    {    
      /* Disable the Channel 1: Reset the CCE Bit */
      TIM1->CCER1 &= (uint8_t)(~TIM1_CCER1_CC1E); //  OC1 is not active
      
      /* Reset the Output Compare Bits & Set the Output Compare Mode */
      TIM1->CCMR1 = (uint8_t)((uint8_t)(TIM1->CCMR1 & (uint8_t)(~TIM1_CCMR_OCM)) 
                              | (uint8_t)TIM1_OCMODE_PWM2); // TIM1_OCMODE_PWM2 = 0x70(112)
    }
    if(vbChannelNumber==TIM1_CHANNEL_1) // ch1
    {
      TIM1_SetCompare1(vwRatioReg);
      if(0)
      {
          /* Set the Capture Compare1 Register value */
          TIM1->CCR1H = (uint8_t)(vwRatioReg >> 8);
          TIM1->CCR1L = (uint8_t)(vwRatioReg);
      }
      TIM1_OC1PolarityConfig(vbLevelState);
      if(0)
      {
        TIM1->CCER1 |= TIM1_CCER1_CC1P; //  OC1 active low
      }
      TIM1_OC1NPolarityConfig(vbLevelState);
      if(0)
      {
        TIM1->CCER1 |= TIM1_CCER1_CC1NP;
      }
      TIM1_OC1PreloadConfig(ENABLE);
      if(0)
      {
        TIM1->CCMR1 |= TIM1_CCMR_OCxPE; // : Preload register on TIM1_CCR1 enabled
      }
    }
    else if(vbChannelNumber==TIM1_CHANNEL_2) // ch2
    {
      TIM1_SetCompare2(vwRatioReg);
      TIM1_OC2PolarityConfig(vbLevelState);
      TIM1_OC2NPolarityConfig(vbLevelState);
      TIM1_OC2PreloadConfig(ENABLE);
    }
    else if(vbChannelNumber==TIM1_CHANNEL_3) // ch3
    {
      TIM1_SetCompare3(vwRatioReg);
      TIM1_OC3PolarityConfig(vbLevelState);
      TIM1_OC3NPolarityConfig(vbLevelState);
      TIM1_OC3PreloadConfig(ENABLE);
    }
    else if(vbChannelNumber==TIM1_CHANNEL_4) // ch4
    {
      TIM1_SetCompare4(vwRatioReg);
      TIM1_OC4PolarityConfig(vbLevelState);
      // TIM1_OC4NPolarityConfig(vbLevelState);
      TIM1_OC4PreloadConfig(ENABLE);
    }
    if(vbPinUseFlag&0x1)
    {
      TIM1_CCxCmd(vbChannelNumber, ENABLE);
      if(0)
      {
        TIM1->CCER1 |= TIM1_CCER1_CC1E; // On - OC1 signal is output on the corresponding output pin depending on the MOE, OSSI, OSSR, OIS1, OIS1N and CC1NE bits
      }
    }
    if(vbPinUseFlag&0x2)
    {
      TIM1_CCxNCmd(vbChannelNumber, ENABLE);
      if(0)
      {        
        TIM1->CCER1 |= TIM1_CCER1_CC1NE;
      }
    }
    TIM1_ARRPreloadConfig(ENABLE);
    if(0)
    {
      TIM1->CR1 |= TIM1_CR1_ARPE; // TIM1_ARR register is buffered through a preload register
    }
    if(vbPulseState)
    {
      TIM1_SelectOnePulseMode(TIM1_OPMODE_SINGLE);
      // TIM1->CR1 |= TIM1_CR1_OPM;
      TIM1->EGR = 0xFF;
    }
    /* TIM1 Main Output Enable */
    TIM1_CtrlPWMOutputs(ENABLE);
    if(0)
    {
      TIM1->BKR |= TIM1_BKR_MOE; // OC and OCN outputs are enabled if their respective enable bits are set (CCiE in TIM1_CCERi registers).
    }
    /* TIM1 counter enable */
    TIM1_Cmd(ENABLE);
    if(0)
    {
      TIM1->CR1 |= TIM1_CR1_CEN; // Counter enabled
    }
  }
#endif

  return 1;
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/*
  Period = Count * Tclock = Count * ((Pscr+1)/fMasterClock)
  Period Max < 2 ^ (N + 1) 
  ((Pscr+1)/fMasterClock) = 2 ^ M 
  Count Max < 2 ^ 16

  2 ^ (N + 1) = 2 ^ 16 * 2 ^ M = 2 ^ (16 + M)
  N + 1 = 16 + M
  M = N - 15
  ((Pscr+1)/fMasterClock) = 2 ^ M 

  Pscr = (2 ^ M) * fMasterClock - 1;
  Count = Period >> M;
*/
#define _MM_CK_PSC_   (16000000)  // fMASTER_CLOCK = 16MHz...
int mmTim1CalPscrCntr(uint32_t vdwDelayUsec, uint16_t *pvwPSCR, uint16_t *pvwCNTR)
{
  uint32_t vdwMask = vdwDelayUsec;
  uint8_t vbIndex = 0;
  uint8_t vbMaxBitDelay = 0;
  uint8_t vbPscrBitData = 0;
  {
    for (vbIndex = 0; vbIndex < 32; vbIndex++)
    {
      if (vdwMask & 0x80000000)
      {
        break;
      }
      vdwMask <<= 1;
    }
    vbMaxBitDelay = 31 - vbIndex;
    ccprintf(_DEBUG_CMD_A_, ("mmTim1CalPscrCntr : vbMaxBitDelay : %d\r\n", vbMaxBitDelay));
    if (vbMaxBitDelay <= 15)
    {
      vbMaxBitDelay = 15;
    }
    else if(vbMaxBitDelay >= (31-5)) // 2 ^ 5 = 32 > (_MM_CK_PSC_/1000000) =  16
    {      
      vbMaxBitDelay = 31-5;
    }
    vbPscrBitData = vbMaxBitDelay - 15;
    *pvwPSCR = (1 << vbPscrBitData) * (_MM_CK_PSC_/1000000) - 1;
    *pvwCNTR = vdwDelayUsec >> vbPscrBitData;
    ccprintf(_DEBUG_CMD_A_, ("mmTim1CalPscrCntr : vwPSCR (0x%04x) vwCNTR (0x%04x) vdwDelayUsec [0x%08lx]\r\n", *pvwPSCR, *pvwCNTR, vdwDelayUsec));
  }
  return 1;
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
int mmTim1GpioInit(void)
{
  /* Configures GPIOs */
  // GPIO_Configuration();
  {
    /* Configure PD0 (LED1) as output push-pull low (led switched on) */
    // GPIO_Init(UCOM_LED1_GPIO, UCOM_LED1_PIN, UCOM_LED1_MODE);
    {
      UCOM_LED1_GPIO->ODR &= (uint8_t)(~(UCOM_LED1_PIN)); /* Output mode  Low level */
      UCOM_LED1_GPIO->DDR |= (uint8_t)UCOM_LED1_PIN;      /* Set Output mode */
      UCOM_LED1_GPIO->CR1 |= (uint8_t)UCOM_LED1_PIN;      /* Push-Pull */
      // UCOM_LED1_GPIO->CR1 &= (uint8_t)(~(UCOM_LED1_PIN)); /* Open drain */
      UCOM_LED1_GPIO->CR2 |= (uint8_t)UCOM_LED1_PIN; /* 10MHz */
      // UCOM_LED1_GPIO->CR2 &= (uint8_t)(~(UCOM_LED1_PIN)); /* 2MHz */
    }
    /* Configure PA3 : KEY IN as input push-pull low (led switched on) */
    // GPIO_Init(UCOM_KEY1_GPIO, UCOM_KEY1_PIN, UCOM_KEY1_MODE);
    {
      UCOM_KEY1_GPIO->DDR &= (uint8_t)(~(UCOM_KEY1_PIN)); /* Set Input mode */
      UCOM_KEY1_GPIO->CR1 |= (uint8_t)UCOM_KEY1_PIN; /* Pull-up */
      // UCOM_KEY1_GPIO->CR1 &= (uint8_t)(~(UCOM_KEY1_PIN)); /* Float */
      // UCOM_KEY1_GPIO->CR2 |= (uint8_t)UCOM_KEY1_PIN; /* External interrupt enable */
      UCOM_KEY1_GPIO->CR2 &= (uint8_t)(~(UCOM_KEY1_PIN)); /* External interrupt disable */
    }
  }
  return 1;
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void mmRemapCH1N_CH2N_CH3N_ETR(void)
{
		#define OPT2_REG 0x4803  // https://www.st.com/resource/en/datasheet/stm8s105c6.pdf  page 49
		#define AFR5_BIT 0x20  // 1 : port B0 alternate function = TIM1_CH1N.// AFR5 Alternate function remapping option 5
		// if (!strcmp(argv[2], "set"))
		{
			/* set option bytes */ 
			if(FLASH_ReadByte(OPT2_REG)!=AFR5_BIT) 
			{ 
				FLASH_Unlock(FLASH_MEMTYPE_DATA); 
				/* Enable by HW WWDG */ 
				FLASH_ProgramOptionByte(OPT2_REG,AFR5_BIT); 
			} 
			// vbData=FLASH_ReadOptionByte(OPT2_REG); 
		}
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#endif // __MIB_DEBUG_TIM1__
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#if 0

void main(void)
{
  CLK_HSICmd(ENABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
  GPIO_Config();
  TIM1_Config();
  while (1)
  {
    // TIM1_SetCompare1(349);
  }
}

void TIM1_Config()
{
  TIM1_DeInit();
  /* cau hinh timer 1 co dau ra PWM la 10kHz
  - bo chia tan 2
  - duty =30%
   Ftimer=16/2=8Mhz
   TimerPeriod= Ftimer/Fpwm  -1=799
  */
  TIM1_TimeBaseInit(1, TIM1_COUNTERMODE_UP, 799, 0);
  TIM1_SelectOCxM(TIM1_CHANNEL_1, TIM1_OCMODE_PWM2);
  TIM1_OC1PolarityConfig(TIM1_OCPOLARITY_LOW);
  TIM1_CCxCmd(TIM1_CHANNEL_1, ENABLE);
  TIM1_SetCompare1(239);
  TIM1_OC1PreloadConfig(ENABLE);
  TIM1_ARRPreloadConfig(ENABLE);
  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);
  /* TIM1 Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
}

void GPIO_Config(void)
{
  GPIO_DeInit(GPIOB);
  GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_DeInit(GPIOC);
  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_FAST);
}
#endif