/**
  ******************************************************************************
  * @file    : stm8s_blink_key_main.c
  * @brief   : LED flickers according to key input.
  * @author  : MYMEDIA Co., Ltd.
  * @version : V1.0.0
  * @date    : 2023 : 02 : 03
  ******************************************************************************
  */ 
/*
  compile : make
  write   : ST Visual Programmer 
  exe     : ./exe/stm8s_blink_key_tli_irq.hex
  info :
    non-maskable top level hardware interrupt (TLI)에 프로그램 예제이다.
    TLI 인터럽트를 하기 위해서는 GPIO_D7에 KEY 입력으로 주어야 한다. 이 인터럽트는 NMI 이다.
    STM8S Reference Manuals : <https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf>
    66page 에 나옴.  인터럽트로 받아서 엘이디를 ON을 하는 기능이다. 시간이 지나면 엘이디는 OFF가 된다.
    이 프로그램에서는 gpio 인터럽트를 이해하고 인터럽트가 발생시 처리하는 절차를 
    간단히 이해하기 위한 프로그램이다. 모든 개발에는 gpio 인터럽트에 대한 처리를 하기위해서
    필요한 순서에 대한 이해가 필요하며 이것에 대한 간단 명료하게 이해하기 위한 예제이다.
*/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Hardware define */
/*
  PD0 : LED1
  PA3 : KEY1 
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
#define UCOM_LED1_GPIO       GPIOD        // PD0
#define UCOM_LED1_PIN        GPIO_PIN_0   // PD0
#define UCOM_LED1_MODE       GPIO_MODE_OUT_PP_LOW_FAST
#define UCOM_KEY_TLI_GPIO    GPIOD        // PD7
#define UCOM_KEY_TLI_PIN     GPIO_PIN_7   // PD7
// #define UCOM_KEY_TLI_MODE    GPIO_MODE_IN_FL_NO_IT
#define UCOM_KEY_TLI_MODE    GPIO_MODE_IN_PU_IT // GPIO_MODE_IN_FL_IT     
/*!< Input floating, external interrupt */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void CLK_Configuration(void);
void GPIO_Configuration(void);
void Toggle(void);

/* Private functions ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief External Interrupt PORTA Interrupt routine.
  * @param  None
  * @retval None
  */
// INTERRUPT_HANDLER(TLI_IRQHandler, 0)
void TLI_IRQHandler(void) __interrupt(0)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 {  
  UCOM_LED1_GPIO->ODR ^= (uint8_t)UCOM_LED1_PIN;
  UCOM_KEY_TLI_GPIO->CR2 &= (uint8_t)(~(UCOM_KEY_TLI_PIN)); /* External interrupt disable */
 }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
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
void main(void)
{
  /* Configures clocks */
  // CLK_Configuration();
  {
    /* Clear High speed internal clock prescaler */
    CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);    
    /* Set High speed internal clock prescaler */
    CLK->CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV1;
  }
  // void EXTI_SetExtIntSensitivity(EXTI_Port_TypeDef Port, EXTI_Sensitivity_TypeDef SensitivityValue)
  // EXTI_SetExtIntSensitivity(GPIOA,EXTI_SENSITIVITY_RISE_ONLY); // PA3
  {    // PD7 : TLI (top level hardware interrupt)
    uint8_t vbData = 0;
    vbData = (uint8_t)(EXTI_TLISENSITIVITY_FALL_ONLY); /*!< Top Level Interrupt on Falling edge only */
    // vbData = (uint8_t)(EXTI_TLISENSITIVITY_RISE_ONLY); /*!< Top Level Interrupt on Rising edge only */
    EXTI->CR2 &= (uint8_t)~(0x04); /* TLI : PD7 */
    EXTI->CR2 |= (vbData); /* TLI */
  }
  /* Configures GPIOs */
  // GPIO_Configuration();
  {
    /* Configure PD0 (LED1) as output push-pull low (led switched on) */
    // GPIO_Init(UCOM_LED1_GPIO, UCOM_LED1_PIN, UCOM_LED1_MODE);
    {
      UCOM_LED1_GPIO->ODR &= (uint8_t)(~(UCOM_LED1_PIN)); /* Output mode  Low level */
      UCOM_LED1_GPIO->DDR |= (uint8_t)UCOM_LED1_PIN; /* Set Output mode */
      UCOM_LED1_GPIO->CR1 |= (uint8_t)UCOM_LED1_PIN; /* Push-Pull */
      // UCOM_LED1_GPIO->CR1 &= (uint8_t)(~(UCOM_LED1_PIN)); /* Open drain */
      UCOM_LED1_GPIO->CR2 |= (uint8_t)UCOM_LED1_PIN; /* 10MHz */
      // UCOM_LED1_GPIO->CR2 &= (uint8_t)(~(UCOM_LED1_PIN)); /* 2MHz */
    }
    /* Configure PD7 : KEY IN as input push-pull low (led switched on) */
    // GPIO_Init(UCOM_KEY_TLI_GPIO, UCOM_KEY_TLI_PIN, UCOM_KEY_TLI_MODE);
    {
      UCOM_KEY_TLI_GPIO->DDR &= (uint8_t)(~(UCOM_KEY_TLI_PIN)); /* Set Input mode */
      UCOM_KEY_TLI_GPIO->CR1 |= (uint8_t)UCOM_KEY_TLI_PIN; /* Pull-up */
      // UCOM_KEY_TLI_GPIO->CR1 &= (uint8_t)(~(UCOM_KEY_TLI_PIN)); /* Float */
      // UCOM_KEY_TLI_GPIO->CR2 |= (uint8_t)UCOM_KEY_TLI_PIN; /* External interrupt enable */
      UCOM_KEY_TLI_GPIO->CR2 &= (uint8_t)(~(UCOM_KEY_TLI_PIN)); /* External interrupt disable */
    }
    {      
       UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR & ~UCOM_LED1_PIN; // led on...
    }
  }
  {
    // enable...
    // enableInterrupts();
  }
  {
    uint32_t vwDelay = 0;
    do {
      if(vwDelay==0) 
      {      
        vwDelay = 0x20000*5*20; // 10sec..
        // UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR & ~UCOM_LED1_PIN; // led on...
        UCOM_LED1_GPIO->ODR ^= (uint8_t)UCOM_LED1_PIN;
        UCOM_KEY_TLI_GPIO->CR2 |= (uint8_t)UCOM_KEY_TLI_PIN; /* External interrupt enable */
      }
      vwDelay--;
    } while(1);
  }
  #if 0
  {    
    uint32_t vdwDelay = 0;
    do {
      if(UCOM_KEY_TLI_GPIO->IDR & UCOM_KEY_TLI_PIN)
      {
        UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR & ~UCOM_LED1_PIN;
      }
      else
      {
        // UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR | UCOM_LED1_PIN;
        if(vdwDelay)
        {
          vdwDelay--;
        }
        else
        {
          vdwDelay = 0x20000*5*5;
          // Toggle();
          UCOM_LED1_GPIO->ODR ^= (uint8_t)UCOM_LED1_PIN;
        }
      }
    } while(1);
  }
  #endif
}
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
