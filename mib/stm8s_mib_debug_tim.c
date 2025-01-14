/**
  ******************************************************************************
  * @file    : stm8s_mib_debug_tim.c
  * @brief   : timer sec function
  * @author  : MYMEDIA Co., Ltd.
  * @version : V1.0.0
  * @date    : 2023 : 02 : 07
  ******************************************************************************
  */
 /* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_mib.h"
 /* Private typedef -----------------------------------------------------------*/
 /* Private define ------------------------------------------------------------*/
 /* Private macro -------------------------------------------------------------*/
 /* Private variables ---------------------------------------------------------*/
 /* Private function prototypes -----------------------------------------------*/
 /* Private functions ---------------------------------------------------------*/
 /* Global variables ----------------------------------------------------------*/
volatile uint16_t gwMibMsec;
volatile uint16_t gwMibSec; // 18 hour
volatile uint8_t gbMibMiliSec;
volatile uint16_t gwMibMiliSec;
 /* Public functions ----------------------------------------------------------*/
/**
 ******************************************************************************
  * @brief          : Main function.
  * @par Parameters :
  * None
  * @retval         : void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void timIrqInit(void)
{
  gwMibSec = 0;
  gwMibMsec = 0;
  /* Configures TIM4 */
  /*
    timer 4를 사용한 초 단위로 증가하는 함수를 만들어 65,536sec를 만든다.
    65,536sec = 1,092 분 = 18 시간
    32비트는 프로그램의 리소스를 너무 많이 소모하여 메인 프로그램의 속도를 저하시킨다.
    
    타이머 4 는 시스템 시간을 측정하거나 딜레이를 주기 위한 것으로 사용하기로 한다.
    즉 1msec 로 인터럽트를 발생을 시키기로 하고 이에 맞추어서 1msec마다 감소하는 변수(gwMibMsec)와 
    1sec 마다 증가하는 변수를 두어서 시스템이 시작된 이후로 계속 증가하는 변수(gwMibSec)를 설정한다.
    위와 같은 동작을 하기위해서는 시스템 클럭이 16MHz이면 이 변수의 프리스케일을 128로 나누고 
    그에따른 카운터를 업카운터를 하여 오버플로우시에 인터럽트를 발생시킨다. 즉 125번의 증가후에 
    인터럽트를 발생하면 1 msec가 되고 바로 업카운터 값을 다시 자동으로 재설정하는 ARP 기능을 활성화 하여
    사용한다. 이 카운터는 업카운터이므로 보수를 취해서 저장을 하여야 한다. 즉 FF에서 0으로 변경시 인터럽트가 
    발생하므로 1일때 FF 2일때 FE이고 0은 사용할 수 없다.

    16MHz / 128 = 125 
    fin = 16MHz
    Prescaler = 128
    UP-COUNTER = 125
    CR1.ARPE =  1 // Auto-reload preload enable
    CR1.CEN = 1
    IER.UIE = 1 //Update interrupt enable
    SR.UIF = 0  // Update interrupt flag
    CNTR = 125 - 1
    PSCR  = 7
    ARR = 125 - 1 // Auto - reload register
  */
  {
    #define _MM_UCOM_MSEC_  (16000/128) // 16MHz / 2**7  = 125
    TIM4->PSCR  = TIM4_PRESCALER_128; // 7 : 8 usec...
    TIM4->ARR = (uint8_t)(_MM_UCOM_MSEC_-1); // 8 * 125 = 1000 usec = 1 msec...
    // TIM4->ARR = 125; // (uint8_t)~(_MM_UCOM_MSEC_-1);
    TIM4->CNTR = TIM4->ARR;
    TIM4->SR1 = (uint8_t)(~TIM4_FLAG_UPDATE);
    // TIM4->EGR = 1; // TIM4_EGR_UG
    TIM4->IER = TIM4_IER_UIE;
    TIM4->CR1 = TIM4_CR1_CEN | TIM4_CR1_ARPE;
  }
}
/**
  ******************************************************************************
  * @brief          : Main function.
  * @par Parameters :
  * None
  * @retval         : void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void timIrqMain(void)
{
  if(gwMibMsec) 
  {
    gwMibMsec--;
  }
  else 
  { 
    // gwMibMsec = 1000-1-10;
    gwMibMsec = 999; // 시간을 측정하여 조정한다.
    gwMibSec++;
  }
  if(gbMibMiliSec) gbMibMiliSec--;  
  if(gwMibMiliSec) gwMibMiliSec--;  
}
/**
  ******************************************************************************
  * @brief          : Main function.
  * @par Parameters :
  * None
  * @retval         : void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
uint16_t MibGetSecs(void)
{
  return gwMibSec; // gwMibSec;
}
/**
  ******************************************************************************
  * @brief          : Main function.
  * @par Parameters :
  * None
  * @retval         : void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void delay_ms(uint8_t vbDelay)
{
  gwMibMiliSec = vbDelay;
  do {
    if(gwMibMiliSec==0) break;
  } while (1);
}