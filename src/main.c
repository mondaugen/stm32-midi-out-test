/* Includes ------------------------------------------------------------------*/
#include <stdlib.h> 
#include "stm32f4xx_conf.h" 

#define MIDI_BAUD_RATE 31250
#define RANDOM_NOTES 0 

GPIO_InitTypeDef  GPIO_USART_InitStruct;
GPIO_InitTypeDef  GPIO_Trig_InitStruct;
USART_InitTypeDef  USART_InitStruct;

void Delay(__IO uint32_t nCount);
void UART5_Enable_Tx(void);

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f4xx.c file
     */
    
    /* Enable USART For Tx */
    UART5_Enable_Tx();

    /* we have to do this */

  while (1)
  {
#if RANDOM_NOTES
      uint8_t note = (uint8_t)(127 * drand48());
      uint8_t vel  = (uint8_t)(127 * drand48());
      uint32_t delay = (uint32_t)((0xffffff) * drand48()) / 100;
      /* Send note on channel 10 */
      while(!(UART5->SR & USART_SR_TC));
      USART_SendData(UART5, 0x99);
      while(!(UART5->SR & USART_SR_TC));
      USART_SendData(UART5, note);
      while(!(UART5->SR & USART_SR_TC));
      USART_SendData(UART5, vel);
    
      /* Insert delay */
      Delay(delay);

      /* Send note off */
      while(!(UART5->SR & USART_SR_TC));
      USART_SendData(UART5, 0x89);
      while(!(UART5->SR & USART_SR_TC));
      USART_SendData(UART5, note);
      while(!(UART5->SR & USART_SR_TC));
      USART_SendData(UART5, 0);

      /* Insert delay */
      Delay(delay);
#else
      uint8_t note = (uint8_t)(60);
      uint8_t vel  = (uint8_t)(100);
      uint32_t delay = (uint32_t)(0xffff);
      /* Send trigger */
      GPIO_SetBits(GPIOA, GPIO_Pin_5);
      Delay(0xff);
      GPIO_ResetBits(GPIOA, GPIO_Pin_5);

      /* Send note on channel 1 */
      while(!(UART5->SR & USART_SR_TC));
      USART_SendData(UART5, 0x90);
      while(!(UART5->SR & USART_SR_TC));
      USART_SendData(UART5, note);
      while(!(UART5->SR & USART_SR_TC));
      USART_SendData(UART5, vel);
    
      /* Insert delay */
      Delay(delay);

      /* Send note off */
      //while(!(UART5->SR & USART_SR_TC));
      //USART_SendData(UART5, 0x80);
      //while(!(UART5->SR & USART_SR_TC));
      //USART_SendData(UART5, note);
      //while(!(UART5->SR & USART_SR_TC));
      //USART_SendData(UART5, 0);

      /* Insert delay */
      //Delay(delay);
#endif /* RANDOM_NOTES */
  }
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

void UART5_Enable_Tx(void)
{
    /* Enable Clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* Set up GPIO for alternate function */
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);

    /* Configure GPIO to transmit */
    GPIO_USART_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_USART_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_USART_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_USART_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_USART_InitStruct.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOC, &GPIO_USART_InitStruct);

    /* Configure GPIO To make a trigger */
    GPIO_Trig_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Trig_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Trig_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_Trig_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Trig_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_Trig_InitStruct);

    /* Configure USART */
    USART_InitStruct.USART_BaudRate = MIDI_BAUD_RATE;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Tx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(UART5, &USART_InitStruct);

    /* Enable UART */
    USART_Cmd(UART5, ENABLE);
}





#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 
