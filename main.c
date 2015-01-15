/**
  ******************************************************************************
  * file    main.c
  * author  A.Jaworowski
  *
  * version V1.2
  * date	2015-01-15
  * brief	Instead of blinking, fading LED, using timer outout compare and PWM.
  * The whole idea is to run the led with short periods and control the
  * duty cycle. The effect is that the LED looks as if dimmed.
  * This is done by configuring alternate function (AF) for PA5 (crucial),
  * see GPIO init structure.
  * Observe1 no interrupts!
  * Observe2 empty while (1){}
  * - ref: http://visualgdb.com/tutorials/arm/stm32/pwm/
  *
  * version V1.1
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
	int16_t timerPrescaler = 3200; //CLK 32 MHz with prescaler 32000 -> timer clk = 1000 Hz
	int16_t period = 100; //This is period for the timer TIM
	int16_t dutyCycle = period/10;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	/* SysTick end of count event each 1ms */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	//Enable PA5 alternate function (AF), see Datasheet
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit (& GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);

	//Enable timer TIM2 (for PA5)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM2);
	TIM_TimeBaseInitStruct.TIM_Prescaler = timerPrescaler;	//CLK = 32MHz, so our TIM CLK will run at 1000 Hz
	TIM_TimeBaseInitStruct.TIM_Period = period-1;			//0..period-1. timer_clock/period = frequency
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	//Enable timer
	TIM_Cmd(TIM2, ENABLE);

	//Configure timer TIM2 as an output compare timer, PWM mode
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = dutyCycle;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStruct); //Observe that channel number is in the name
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);

  /* Infinite loop */
	uint32_t timerValue;
	while (1) {//Everything handled by the timer}
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
