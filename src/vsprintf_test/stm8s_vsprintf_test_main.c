/**
  ******************************************************************************
  * @file stm8s_vsprintf_test_main.c
  * @brief 
  * @author MYMEDIA Co., Ltd.
  * @version V1.0.0
  * @date 2023.1.6
  ******************************************************************************
  */
/*
  compile : make
  write   : ST Visual Programmer 
  exe     : ./exe/stm8s_vsprintf_test.hex
  src : ./mib/stm8s_mib_it.c
        ./mib/stm8s_mib_debug.c
        ./mib/stm8s_mib_vsprintf.c
  info :
    STM8S Reference Manuals : <https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf>
    page 320, 참고한다.
    또한 printf 문을 구현하기 위한 stm8s에 포팅한 프로그램 예제이다.
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
    uint32_t vdwCount = 0;
    uint32_t vdwTestValue = 0x87620394;
    uint16_t vwTestValue = 0x7fff;
    uint16_t vwTestValueA = 0xff89;
    uint8_t vbTestValue = 0x98;
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
        if(vwCh == 'a')
        {
          mib_printf("test vdwCount[%12d/0x%08lx]\r\n",vdwCount,vdwCount);
          MibWriteDebugStringCheck(1," webgpio.com by MYMEDIA Co., Ltd.\r\n");
        }
        else if(vwCh == 'd')
        {
          mib_printf("test vdwTestValue[0x%08lx/%u]\r\n",vdwTestValue,vdwTestValue);
        }
        else if(vwCh == 'w')
        {
          mib_printf("test vwTestValue [0x%04x/%d]\r\n",vwTestValue,vwTestValue);
          mib_printf("test vwTestValueA[0x%04x/%u]\r\n",vwTestValueA,vwTestValueA);
        }
        else if(vwCh == 'b')
        {
          mib_printf("test vbTestValue[0x%02x/%d]\r\n",vbTestValue,vbTestValue);
        }
      }
      vdwCount++;
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
  //GPIO_WriteReverse(UCOM_LED1_GPIO, UCOM_LED1_PIN);
  if(UCOM_LED1_GPIO->IDR & UCOM_LED1_PIN)
  {
    UCOM_LED1_GPIO->ODR &= ~(UCOM_LED1_PIN); // low... 
  }
  else 
  {
    UCOM_LED1_GPIO->ODR |= (UCOM_LED1_PIN); // high... 
  }
}
/****************** (c) 2008  STMicroelectronics ******************************/
