#include "timer.h"

#define TIM_CLOCK_FREQ            (2000000)  

TIM_HandleTypeDef TIM6_Handler;   
volatile uint32_t g_us_tick;  

void TIM6_IRQHandler(void)
{
	if (TIM6->SR & TIMER_SR_UIF)
	{
		g_us_tick++;  
	}
	
	TIM6->SR = 0;   //write 0 to clear hardware flag  
}

void TIM6_MSP_Pre_Init(TIM_HandleTypeDef * htim)
{
	HAL_TIMER_MSP_Init(&TIM6_Handler);   
}

void TIM6_MSP_Post_Init(void)
{
/*
    do nothing here 
*/
}


void TIM6_Init(void)
{
  uint32_t timer_clock; 
    
	timer_clock = System_Get_APBClock(); 
	
	if (System_Get_SystemClock() != System_Get_APBClock())  // if hclk/pclk != 1, then timer clk = pclk * 2  
	{
		 timer_clock =  System_Get_APBClock() << 1;    
	}
	TIM6_Handler.Instance = TIM6;
	TIM6_Handler.Init.ARRPreLoadEn = TIM_ARR_PRELOAD_ENABLE;    
	TIM6_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	TIM6_Handler.Init.CounterMode = TIM_COUNTERMODE_UP; 
	TIM6_Handler.Init.RepetitionCounter = 0;  
	TIM6_Handler.Init.Prescaler = (timer_clock/TIM_CLOCK_FREQ) - 1;  
	TIM6_Handler.Init.Period =  1;
	
	TIM6_MSP_Pre_Init(&TIM6_Handler);       
	HAL_TIMER_Base_Init(&TIM6_Handler);    
	HAL_TIM_ENABLE_IT(&TIM6_Handler, TIMER_INT_EN_UPD); 
	TIM6_MSP_Post_Init();  
	
	HAL_TIMER_Base_Start(TIM6_Handler.Instance);

}

