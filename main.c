/**
  ******************************************************************************
  * file    main.c
  * author  A.Jaworowski
  * version V1.0.0
  * date    2015-01-11
  * brief	Enable analog input on Arduino port A0. This means that we will
  * connect ADC_IN0 to the ADC. Using channel 0. Measuring with delay(),
  * every 1000ms. For some reason, we must keep the code in the
  * EXTI15_10_IRQHandler. Otherwise ADC values varies all the time, just as
  * when we didn't enabled the RCC_HSICmd.
  *
  * The main references for this code:
  * - http://www.embedds.com/introducing-to-stm32-adc-programming-part2/
  * - ADC1_AnalogWatchdog from STM32L1xx_StdPeriph_Examples
  *
  * Debugging is done using ST-Link and SWV.
  *----------------------------------------------------------------------------
  *
  * version	2
  * date	2015-01-11
  * brief	Enable digital OUT so that analog IN can control something - an LED.
  *
  * adding references
  * - http://www.cs.indiana.edu/~geobrown/book.pdf
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h> //printf()

static __IO uint32_t TimingDelay;
uint8_t BlinkSpeed = 0;
uint16_t AD_value, ledval;
RCC_ClocksTypeDef RCC_Clocks;


int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure, GPIO_InitStructurPA5;
    ADC_InitTypeDef ADC_InitStructure;

	/* SysTick end of count event each 1ms */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	/* Enable the GPIOA Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	/* Enable PA5 as digital OUT */
//	GPIO_InitStructurPA5.GPIO_Pin = GPIO_Pin_5;
//	GPIO_InitStructurPA5.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructurPA5.GPIO_Speed = GPIO_Speed_400KHz;
//	GPIO_InitStructurPA5.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructurPA5.GPIO_PuPd = GPIO_PuPd_DOWN;
//	GPIO_Init(GPIOA, &GPIO_InitStructurPA5);

	/* Enable the HSI - the ADC internal clock running at 16 MHz
	 * Was unsure if needed - it is!*/
	RCC_HSICmd(ENABLE);
	/* Wait until HSI oscillator is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	/* Enable ADC1 clock. ADC is connected to the APB2 bus.
	 * For some reason there is no ADC0*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/* ADC1 Configuration -----------------------------------------------------*/
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel0 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_384Cycles);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Wait until the ADC1 is ready */
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET) {}

	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConv(ADC1);

	/* Infinite loop */
	uint8_t divider = 1;
	//GPIOA->BSRRL = GPIO_Pin_5;
	while (1)
	{
		//wait for conversion complete
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)){}
		AD_value = ADC_GetConversionValue(ADC1);
		printf("ADC value: %d\t\tDelay value: %d\n", AD_value, AD_value/divider);
		Delay(1000);

//		GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
//		Delay(AD_value/divider);
//		GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
//		Delay(AD_value/divider);
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
