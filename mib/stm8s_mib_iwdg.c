/**
  ******************************************************************************
  * @file stm8s_mib_iwdg.c
  * @brief Function to calculate iwdg time delay...
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
#define _DEBUG_IWDG_A_ 1
void calConfigIWDG(uint16_t vwDelay); // msec... 

#define rd_ADDR8(A)     (*((volatile uint8_t *)(A)))
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
/**
  * @brief 
  * @par Parameters:
  * vwDelay: independent watchdog delay msec 
  * @retval Return value:
  * None
  */
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
uint16_t gwTableDataIWDG[] = {15,31,63,127,255,510,1000,0};
void calConfigIWDG(uint16_t vwDelay) // msec... 
{
	uint32_t vdwData = vwDelay;
	uint8_t vbIndex = 0;
	uint8_t vbPRBIT = 0;
	uint8_t vbRLRBIT = 0;
	for(vbIndex=0;;vbIndex++)
	{
		if(gwTableDataIWDG[vbIndex]==0)
		{
			vwDelay = gwTableDataIWDG[vbIndex-1];
			vbPRBIT = vbIndex - 1;
			break;
		}
		else if(vwDelay <= gwTableDataIWDG[vbIndex])
		{
			vbPRBIT = vbIndex;
			break;
		}
	}
	{
		vdwData = (vdwData * 128) / 2;
		vdwData = vdwData / (1<<(vbPRBIT+2));
		if(vdwData > 255)
		{
			vbRLRBIT = 0xff;
		}
		else
		{
			vbRLRBIT = (uint8_t)(vdwData&0xff);
		}
		ccprintf(_DEBUG_IWDG_A_, ("calConfigIWDG...vbPRBIT[%u]/vbRLRBIT[%u]\r\n",vbPRBIT,vbRLRBIT));		
		MibWriteDebugEmptyCheck();
		IWDG->KR = 0xCC; // KEY_START , enable...
		IWDG->KR = 0x55; // KEY_ACCESS
		IWDG->PR = (uint8_t)vbPRBIT; 
		IWDG->RLR = vbRLRBIT;
		IWDG->KR = 0xAA; // KEY_REFRESH
	}
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------


