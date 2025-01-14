/**
  ******************************************************************************
  * @file stm8s_blink_key_timer_main.c
  * @brief Depending on the key input, the LED blinks using the timer inside the chip.
  * @author MYMEDIA Co., Ltd.
  * @version V1.0.0
  * @date 2023.1.6
  ******************************************************************************
  */
/*
  compile : make
  write   : ST Visual Programmer 
  exe     : ./exe/stm8s_blink_key_timer.hex
  info :
    타이머 4 모쥴을 통해서 500msec 동안 깜빡이도록 하는 기능을 구현한 예제이다.
    즉 타이머 4에 stm8s의 마스터 클럭(16MHz)을 입력으로 받아서 
    STM8S Reference Manuals : <https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf>
    page 253, 참고하여 update flag를 사용하여 엘이디(GPIO_D0)를 토글하는 기능을 구현한 예제이다.
    여기에서는 인터럽트를 사용하지 않은 타이머 사용 예제이다.
    key 입력시에는 delay를 초기화 한다.
*/
/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
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
  /* Configures clocks */
  // CLK_Configuration();
  {
    /* Clear High speed internal clock prescaler */
    CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);    
    /* Set High speed internal clock prescaler */
    CLK->CKDIVR |= (uint8_t)CLK_PRESCALER_HSIDIV1;
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
    /* Configure PA3 : KEY IN as input push-pull low (led switched on) */
    // GPIO_Init(UCOM_KEY1_GPIO, UCOM_KEY1_PIN, UCOM_KEY1_MODE);
    {
      UCOM_KEY1_GPIO->DDR &= (uint8_t)(~(UCOM_KEY1_PIN)); /* Set Input mode */
      UCOM_KEY1_GPIO->CR1 |= (uint8_t)UCOM_KEY1_PIN; /* Pull-up */
      //UCOM_KEY1_GPIO->CR1 &= (uint8_t)(~(UCOM_KEY1_PIN)); /* Float */
      // UCOM_KEY1_GPIO->CR2 |= (uint8_t)UCOM_KEY1_PIN; /* External interrupt enable */
      UCOM_KEY1_GPIO->CR2 &= (uint8_t)(~(UCOM_KEY1_PIN)); /* External interrupt disable */
    }
  }     
  {
    /* Configures TIM4 */
    /*
      19.2 TIM4 main features
          The main features include:
          • 8-bit auto-reload up counter
          • 3-bit programmable prescaler which allows dividing (also “on the fly”) the counter clock
            frequency by 1, 2, 4, 8, 16, 32, 64 and 128.
          • Interrupt generation
            – On counter update: Counter overflow 
    */
    // TIM4_TimeBaseInit(TIM4_PRESCALER_128, 125 - 1);
    // TIM4_Cmd(ENABLE);
    #define _MM_UCOM_MSEC_  (16000/128) // 16MHz / 2**7  = 125
    TIM4->PSCR  = TIM4_PRESCALER_128; // 7 : 8 usec / clock...
    TIM4->ARR = (uint8_t)(_MM_UCOM_MSEC_-1); // 8 * 125 = 1000 usec = 1 msec....
    // TIM4->ARR = 125; // (uint8_t)~(_MM_UCOM_MSEC_-1);
    TIM4->CNTR = 0; // TIM4->ARR;
    TIM4->SR1 = (uint8_t)(~TIM4_FLAG_UPDATE);
    // TIM4->EGR = 1; // TIM4_EGR_UG
    // TIM4->IER = TIM4_IER_UIE;
    // TIM4->CR1 = TIM4_CR1_CEN | TIM4_CR1_ARPE;
    TIM4->CR1 = TIM4_CR1_CEN;
  }
  {    
    uint32_t vdwDelay = 500; // 500msec
    do {
      if((UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN)==0)
      {
        UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR & ~UCOM_LED1_PIN;
        vdwDelay = 500; // 500msec
      }
      else
      {
        // UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR | UCOM_LED1_PIN;
        // if(TIM4_GetFlagStatus(TIM4_FLAG_UPDATE) == SET)
        if(TIM4->SR1==TIM4_FLAG_UPDATE)
        {
          // TIM4_SetCounter(0);
          // TIM4->CNTR = 0; // TIM4->ARR;
          // TIM4_ClearFlag(TIM4_FLAG_UPDATE);
          TIM4->SR1&=(~TIM4_FLAG_UPDATE);
          // UCOM_LED1_GPIO->ODR ^= (uint8_t)UCOM_LED1_PIN; // 1msec...
          if(vdwDelay) vdwDelay--;
        }
        if(vdwDelay==0)
        {
          vdwDelay = 500; // 500msec
          // Toggle();
          UCOM_LED1_GPIO->ODR ^= (uint8_t)UCOM_LED1_PIN; // 500 msec
        }
      }
    } while(1);
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
