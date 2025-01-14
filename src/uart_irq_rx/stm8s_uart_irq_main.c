/**
  ******************************************************************************
  * @file stm8s_uart_irq_main.c
  * @brief 
  * @author MYMEDIA Co., Ltd.
  * @version V1.0.0
  * @date 2023.1.6
  ******************************************************************************
  */
/*
  compile : make
  write   : ST Visual Programmer 
  exe     : ./exe/stm8s_uart_irq.hex
  src : ./mib/stm8s_mib_it.c
        ./mib/stm8s_mib_debug.c
  info :
    STM8S Reference Manuals : <https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf>
    page 320, 참고하여 수신만 인터럽트로 처리하며 송신은 폴링으로 UART를 통신하는 예제이다.
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
  }
  {
    MibDebugInit(9600);
  }
  {
    enableInterrupts();
  }
  {
    MibWriteDebugStringCheck(1,"\r\n ###################################\r\n");
    MibWriteDebugStringCheck(1," file name : " __FILE__ "\r\n");
    MibWriteDebugStringCheck(1," date :  "__DATE__"  :  "__TIME__"\r\n");
    MibWriteDebugStringCheck(1," webgpio.com by MYMEDIA Co., Ltd.\r\n");
    MibWriteDebugStringCheck(1," ###################################\r\n");
  }
  {    
    uint16_t vwCh = 0;
    uint32_t vdwDelay = 0;
    do {
      if((UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN)==0)
      {
        //UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR & ~UCOM_LED1_PIN; // LED on
        UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR | UCOM_LED1_PIN; // LED off
      }
      else
      {
        if(vdwDelay)
        {
          vdwDelay--;
        }
        else
        {
          vdwDelay = 320000;
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
