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
  exe     : ./exe/stm8s_blink_key_irq.hex
  info :
    엘이디(GPIO_D0)를 OFF하며 
    KEY(GPIO_B5)의 입력을 인터럽트로 받아서 엘이디를 ON을 하는 기능이다. 시간이 지나면
    엘이디는 OFF가 된다.
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
// #define UCOM_KEY1_MODE    GPIO_MODE_IN_PU_NO_IT // GPIO_MODE_IN_FL_NO_IT
#define UCOM_KEY1_MODE    GPIO_MODE_IN_FL_IT     /*!< Input floating, external interrupt */
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
// INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
void EXTI_PORTB_IRQHandler(void) __interrupt(4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 {  
  UCOM_LED1_GPIO->ODR ^= (uint8_t)UCOM_LED1_PIN;
  UCOM_KEY1_GPIO->CR2 &= (uint8_t)(~(UCOM_KEY1_PIN)); /* External interrupt disable */
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
  {    // PA3
    #define EXTI_SHIFT_PORTA  0
    #define EXTI_SHIFT_PORTB  2
    #define EXTI_SHIFT_PORTC  4
    #define EXTI_SHIFT_PORTD  6
    #define EXTI_SHIFT_PORTE  0
    #define EXTI_SHIFT_PORT   EXTI_SHIFT_PORTB

    uint8_t vbData = 0;
    // vbData = (uint8_t)(EXTI_SENSITIVITY_FALL_LOW);  /*!< Interrupt on Falling edge and Low level */
    vbData = (uint8_t)(EXTI_SENSITIVITY_RISE_ONLY); /*!< Interrupt on Rising edge only */ 
    // vbData = (uint8_t)(EXTI_SENSITIVITY_FALL_ONLY); /*!< Interrupt on Falling edge only */ 
    // vbData = (uint8_t)(EXTI_SENSITIVITY_RISE_FALL); /*!< Interrupt on Rising and Falling edges */ 
    EXTI->CR1 &= (uint8_t)~(0x03<<EXTI_SHIFT_PORT); /* PB5 : UCOM_KEY1_GPIO */
    EXTI->CR1 |= (vbData<<EXTI_SHIFT_PORT); /* PORT B */
    // EXTI->CR2 &= (uint8_t)~(0x03<<EXTI_SHIFT_PORT); /* PORT E */
    // EXTI->CR2 |= (vbData<<EXTI_SHIFT_PORT); /* PORT E */
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
    {      
       UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR & ~UCOM_LED1_PIN; // led on...
    }
  }
  {
    // enable...
    enableInterrupts();
  }
  {
    uint32_t vwDelay = 0;
    do {
      if(vwDelay==0) 
      {      
        vwDelay = 0x20000*5*20; // 10sec..
        // UCOM_LED1_GPIO->ODR = UCOM_LED1_GPIO->ODR & ~UCOM_LED1_PIN; // led on...
        UCOM_LED1_GPIO->ODR ^= (uint8_t)UCOM_LED1_PIN;
        UCOM_KEY1_GPIO->CR2 |= (uint8_t)UCOM_KEY1_PIN; /* External interrupt enable */
      }
      vwDelay--;
    } while(1);
  }
  #if 0
  {    
    uint32_t vdwDelay = 0;
    do {
      if((UCOM_KEY1_GPIO->IDR & UCOM_KEY1_PIN)==0)
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
