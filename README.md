# stm32nucleol1115re_GPIO_Analog_Input
Enable analog input on Arduino port A0. This means that we will
connect ADC_IN0 to the ADC. Using channel 0. Measuring with delay(),
every 1000ms. For some reason, we must keep the code in the
EXTI15_10_IRQHandler. Otherwise ADC values varies all the time, just as
when we didn't enabled the RCC_HSICmd.

The main references for this code:
- http://www.embedds.com/introducing-to-stm32-adc-programming-part2/
- ADC1_AnalogWatchdog from STM32L1xx_StdPeriph_Examples

Debugging is done using ST-Link and SWV.
