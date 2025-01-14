/**
  ******************************************************************************
  * @file stm8s_uart_irq_no_main.c
  * @brief A program that does not use IRQ using the UART module inside the chip
  * @author MYMEDIA Co., Ltd.
  * @version V1.0.0
  * @date 2023.1.6
  ******************************************************************************
  */
/*
  compile : make
  write   : ST Visual Programmer 
  exe     : ./exe/stm8s_uart_irq_no.hex
  info :
    STM8S Reference Manuals : <https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf>
    page 320, 참고하여 폴링으로 UART를 통신하는 예제이다.
    타이머 4를 사용한 딜레이며 엘이디를 깜빡이는 것은 유지하며 UART 통신을 하는 예제이다.
*/ 
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_mib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void CLK_Configuration(void);
void GPIO_Configuration(void);
void Toggle(void);

/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
  ******************************************************************************
  * @brief Main function.
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void main(void)
{
  {
    /* Configures clocks */
    CLK_Configuration();

    /* Configures GPIOs */
    GPIO_Configuration();
    
    /* Configures TIM4 */
    TIM4_TimeBaseInit(TIM4_PRESCALER_128, 125 - 1);
    TIM4_Cmd(ENABLE);
  }
  {
    MibDebugInit(9600);
  }
  {    
    uint32_t vdwDelay = 500; // 500msec
    uint16_t vwCh = 0;
    do {
      if((UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN)==0)
      {
        UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR & ~UCOM_LED1_PIN;
        vdwDelay = 500; // 500msec
      }
      else
      {
        // UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR | UCOM_LED1_PIN;
        if(TIM4_GetFlagStatus(TIM4_FLAG_UPDATE) == SET)
        {
          TIM4_SetCounter(0);
          TIM4_ClearFlag(TIM4_FLAG_UPDATE);
          if(vdwDelay) vdwDelay--;
        }
        if(vdwDelay==0)
        {
          vdwDelay = 500; // 500msec
          Toggle();
        }
      }
      vwCh = MibReadDebugByte();
      if(vwCh!=MIB_DEBUG_READ_NODATA)
      {
        MibWriteDebugByte(vwCh);
      }
    } while(1);
  }
}

/**
  ******************************************************************************
  * @brief Configures clocks
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  ******************************************************************************
  */
void CLK_Configuration(void)
{

  /* Fmaster = 16MHz */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);

}

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
  /* Configure PA3 : KEY IN as input push-pull low (led switched on) */
  GPIO_Init(UCOM_KEY1_GPIO, UCOM_KEY1_PIN, UCOM_KEY1_MODE);
}

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
/****************** (c) 2008  STMicroelectronics ******************************/
