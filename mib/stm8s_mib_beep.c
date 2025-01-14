/**
  ******************************************************************************
  * @file stm8s_mib_beep.c
  * @brief Function to generate buzzer sound
  * @author MYMEDIA Co., Ltd.
  * @version V1.0.0
  * @date 2023.1.6
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_mib_vsprintf.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void fnBeepStart(uint16_t vwFreq,uint16_t vwDelay);
void calConfigBEEP(uint16_t vwFreq); // msec... 

#define rd_ADDR8(A)     (*((volatile uint8_t *)(A)))
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/**
  * @brief 
  * @par Parameters:
  * vwFreq: bbuzzer frequency
  * @retval Return value:
  * None
  */
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#define _DEBUG_BEEP_A_ 1
#define UCOM_BEEP_GPIO    GPIOD        // PD4
#define UCOM_BEEP_PIN     GPIO_PIN_4   // PD4
#define UCOM_BEEP_MODE    GPIO_MODE_OUT_PP_LOW_FAST
#define OPT2_REG 0x4803  // https://www.st.com/resource/en/datasheet/stm8s105c6.pdf  page 49
#define AFR7_BIT 0x80  // 1: Port D4 alternate function = BEEP // AFR7 Alternate function remapping option 7
void fnBeepStart(uint16_t vwFreq,uint16_t vwDelay) // msec... 
{
  // static uint8_t gbBeepStartBit = 0;
  // GPIO init
	/* Configure PD4 (BEEPER) as output push-pull low */
	// GPIO_Init(GPIOD, GPIO_PIN_4 , GPIO_MODE_OUT_PP_LOW_FAST);	
	// PD4 : BEEP
  /* Configure PD4 (BEEP) as output push-pull low (led switched on) */
  //GPIO_Init(UCOM_BEEP_GPIO, UCOM_BEEP_PIN, UCOM_BEEP_MODE);
  // if(gbBeepStartBit == 0)
  { 
    // gbBeepStartBit = 1;
    {									
      UCOM_BEEP_GPIO->DDR |= (UCOM_BEEP_PIN); /* Set Output mode */
      UCOM_BEEP_GPIO->CR1 |= (UCOM_BEEP_PIN);	/* Pull-Up or Push-Pull */
      UCOM_BEEP_GPIO->CR2 |= (UCOM_BEEP_PIN);	/* Output speed up to 10 MHz */
      UCOM_BEEP_GPIO->ODR &= ~(UCOM_BEEP_PIN); // low... 
    }
    // GPIO remap
    {
      /* set option bytes */ 
      if(FLASH_ReadByte(OPT2_REG)!=AFR7_BIT) 
      { 
        FLASH_Unlock(FLASH_MEMTYPE_DATA); 
        /* Enable by HW WWDG */ 
        FLASH_ProgramOptionByte(OPT2_REG,AFR7_BIT); 
      } 
      // vbData=FLASH_ReadOptionByte(OPT2_REG); 
    }
  }
  // set freq
  {
		// ccprintf(_DEBUG_BEEP_A_, ("beep start : [%d] Hz\r\n",(vwFreq)));		
		calConfigBEEP(vwFreq);
  }
  // enable beep
  {
    /* Enable the BEEP peripheral */
    BEEP->CSR |= BEEP_CSR_BEEPEN;
		// ccprintf(_DEBUG_BEEP_A_, ("beep CSR ....(0x%x)\r\n",BEEP->CSR));
		// ccprintf(_DEBUG_BEEP_A_, ("beep AFR7....(0x%x)\r\n",rd_ADDR8(0x4803)));		
  }
  if(vwDelay!=0)
  {
    extern uint16_t gwMibMiliSec;
    gwMibMiliSec = vwDelay;
    do {
      if(gwMibMiliSec==0)
      {
        /* Disable the BEEP peripheral */
        BEEP->CSR &= (uint8_t)(~BEEP_CSR_BEEPEN);
        break;
      }
    } while(1);
  }
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/**
  * @brief 
  * @par Parameters:
  * vwFreq: bbuzzer frequency
  * @retval Return value:
  * None
  */
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
void calConfigBEEP(uint16_t vwFreq) // msec... 
{
	uint8_t vbBEEPSEL = 0;
	uint8_t vbBEEPDIV = 0;
	uint32_t vdwData = vwFreq;
	uint32_t vdwDataLast = 0;
	uint8_t vdwDiv = 0;
	if(vwFreq<500)
	{
		// error...
		vbBEEPSEL = 0xFF;
	}
	else if(vwFreq<1000)
	{
		vbBEEPSEL = 0x00; // f / (8 * div) khz
		vdwDiv = (128000 / 8) / vdwData;
		vbBEEPDIV =  vdwDiv - 2;
		vdwDataLast = (128000 / 8) / vdwDiv;
	}
	else if(vwFreq<2000)
	{
		vbBEEPSEL = 0x01; // f / (4 * div) khz
		vdwDiv = (128000 / 4) / vdwData;
		vbBEEPDIV =  vdwDiv - 2;
		vdwDataLast = (128000 / 8) / vdwDiv;
	}
	else if(vwFreq<32000)
	{
		vbBEEPSEL = 0x02; // f / (2 * div) khz
		vdwDiv = (128000 / 2) / vdwData;
		vbBEEPDIV =  vdwDiv - 2;
		vdwDataLast = (128000 / 8) / vdwDiv;
	}
	else
	{
		// error...		
		vbBEEPSEL = 0xFF;
	}
	if(vbBEEPSEL!=0xFF)
	{
		ccprintf(_DEBUG_BEEP_A_, ("beep init [%d : SEL(%d) : DIV(%d)] Hz\r\n",vwFreq,vbBEEPSEL,vbBEEPDIV));		
		/* Set a default calibration value if no calibration is done */
		// if ((BEEP->CSR & BEEP_CSR_BEEPDIV) == BEEP_CSR_BEEPDIV)
		BEEP->CSR  = BEEP_CSR_BEEPDIV;
		{
			BEEP->CSR &= (uint8_t)(~BEEP_CSR_BEEPDIV); /* Clear bits */
			BEEP->CSR |= (vbBEEPDIV & BEEP_CSR_BEEPDIV);
		}
		{
			/* Select the output frequency */
			BEEP->CSR &= (uint8_t)(~BEEP_CSR_BEEPSEL);
			BEEP->CSR |= (uint8_t)((vbBEEPSEL&0x03)<<6);
		}
	}
	else
	{
		BEEP->CSR  = BEEP_CSR_BEEPDIV;
		ccprintf(_DEBUG_BEEP_A_, ("beep init error... [%d] Hz\r\n",vwFreq));		
	}
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


