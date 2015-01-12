/**
  ******************************************************************************
  * file    main.c
  * author  A.Jaworowski
  * version V1.0.0
  * date    2015-01-11
  * brief	Enable digital output on PA5, i.e on Arduino port D13. This is
  * the usual BlinkBlink program, i.e. we connect an LED on PA5. Toggling
  * is performed with Delay().
  *
  * Debugging is done using ST-Link and SWV.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>

static __IO uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

int main(void)
{
  /* SysTick end of count event each 1ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  
  //Enable PA5
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit (& GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA , &GPIO_InitStructure);

  /* Infinite loop */
  while (1)
  {
	  //Toggle PA5, ie pin 5 on port A
	  //Pin 5 can be accessed with: 1<<5 or 0x20 = 0000 0000 0010 0000
	  GPIOA->ODR = GPIO_Pin_5; //same as 1<<5 or 0x20 = 0000 0000 0010 0000
	  Delay(500);
	  GPIOA->ODR = 0x0;
	  Delay(500);

	  /* An alternative is to use BSRRx
	   * - [0..15]  BSRRL Set
	   * - [16..31] BSRRH Reset */
//	  GPIOA->BSRRL = GPIO_Pin_5;
//	  Delay(500);
//	  GPIOA->BSRRH = GPIO_Pin_5;
//	  Delay(500);

	  /* Or using stdlib functions*/
//	  GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
//	  Delay(500);
//	  GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
//	  Delay(500);
  }
}

/**
* @brief  Inserts a delay time.
* @param  nTime: specifies the delay time length, in 1 ms.
* @retval None
*/
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;
  
  while(TimingDelay != 0);
}

/**
* @brief  Decrements the TimingDelay variable.
* @param  None
* @retval None
*/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
