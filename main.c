/**
  ******************************************************************************
  * file    main.c
  * author  A.Jaworowski
  *
  * version 2
  * date	2015-01-15
  * brief	Instead of Delay(), we enable a timer to do the same function.
  * What timer - see Datasheet and search for the port, example PA5, see also
  * chapter 3. Functional overview (all timers are on bus APB1)
  * - ref1: http://visualgdb.com/tutorials/arm/stm32/timers/
  * - ref2: stm32l1xx_tim.c
  *
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
	int16_t period = 500; //This is for the timer TIM
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	/* SysTick end of count event each 1ms */
//	RCC_GetClocksFreq(&RCC_Clocks);
//	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	//Enable timer TIM2 (for PA5)
	//Timer freq = 1 Hz
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM2);
	TIM_TimeBaseInitStruct.TIM_Prescaler = 32000;	//CLK = 32MHz, so our TIM CLK will run at 1000 Hz
	TIM_TimeBaseInitStruct.TIM_Period = period-1;	//0..period-1. timer_clock/period = frequency
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);		//Enable interrupts from TIM2

	//Enable PA5
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit (& GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);

  /* Infinite loop */
	uint32_t timerValue;
	while (1)
	{
		//Simplest alternative - manually check the timer
		//Not perfect, it misses the periods fom time to time
//		if (TIM_GetCounter(TIM2) == (period-1))
//		{
//			GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
//		}

		//Better alternativ. Requires that interrupts are enabled for TIM2
		//Much better behaviour of the blinking
		//Next version: this code will be moved to an interrupt handler.
	    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	    {
	        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	        GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
	    }

	  //Toggle PA5, ie pin 5 on port A
	  //Pin 5 can be accessed with: 1<<5 or 0x20 = 0000 0000 0010 0000
//	  GPIOA->ODR = GPIO_Pin_5; //same as 1<<5 or 0x20 = 0000 0000 0010 0000
//	  Delay(500);
//	  GPIOA->ODR = 0x0;
//	  Delay(500);

	  /* An alternative is to use BSRRx
	   * - [0..15]  BSRRL Set
	   * - [16..31] BSRRH Reset */
//	  GPIOA->BSRRL = GPIO_Pin_5;
//	  Delay(500);
//	  GPIOA->BSRRH = GPIO_Pin_5;
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
