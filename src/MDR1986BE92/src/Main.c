

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "types.h"
#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_eeprom.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_ssp.h"
#include "MDR32F9Qx_power.h"
#include "MDR32F9Qx_bkp.h"
#include "MDR32F9Qx_adc.h"
#include "MDR32F9Qx_timer.h"
#include "port.h"


static RST_CLK_FreqTypeDef RCC_ClocksStatus;	
static RST_CLK_FreqTypeDef CLK;
static SSP_InitTypeDef sSSP;
static uint32_t CPUclk;
static uint32_t ADCdata __attribute__((at(0x2000FFFC))); 

static uint32_t CntMs;
static uint32_t Cnt1;//

extern void ProgramDelay_us(uint32_t Loops); 

typedef  struct PortOut_t {
	uint16_t b0:1;
	uint16_t b1:1;
	uint16_t b2:1;
	uint16_t b3:1;
	uint16_t b4:1;
	uint16_t b5:1;
	uint16_t b6:1;
	uint16_t b7:1;
	uint16_t b8:1;
	uint16_t b9:1;
	uint16_t b10:1;
	uint16_t b11:1;	
	uint16_t b12:1;
	uint16_t b13:1;
	uint16_t b14:1;
	uint16_t b15:1;		
} PortOut;
#define PortPins (*((volatile PortOut*)MDR_PORTA))
#define nCS (*((volatile PortOut*)MDR_PORTC))	

#define DELAY_LOOP_CYCLES               (9UL)
#define LOOP_US 												(FLASH_PROG_FREQ_MHZ / DELAY_LOOP_CYCLES)
#define GET_US_LOOPS(N)                 ((uint32_t)((float)(N) * LOOP_US))

static PORT_InitTypeDef PORT_InitStructure;
static UART_InitTypeDef UART_InitStructure;

void InitBoard (void)
{
	// set 48MGz CPU's clock
	
	POWER_DUccMode	(POWER_DUcc_upto_40MHz);	//	
	RST_CLK_HSEconfig(RST_CLK_HSE_ON);
	MDR_BKP->REG_0F &= ~(0x1FUL<<16);
	MDR_BKP->REG_0F |= (0x18UL<<16);	// Set LSI frequency 32kHz
	MDR_BKP->REG_0F |= (1<<15); 			// Enable LSI 
	Cnt1 = 3400; 											// about 10ms 	for 8MHz 
	for (;Cnt1>0;Cnt1--) {}
	if (RST_CLK_HSEstatus() == SUCCESS)
    {
	/* Enables the clock on EEPROM */	
	RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);	
  EEPROM_SetLatency ( EEPROM_Latency_1);				// set Delay EEPROM [2:0] = 001;			
    /* Select HSE clock as CPU_PLL input clock source */
    /* Set PLL multiplier to 3                        */
		RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv2, RST_CLK_CPU_PLLmul6);	// (HSE/2) * 6	
    /* Enable CPU_PLL */
    RST_CLK_CPU_PLLcmd(ENABLE);
		ProgramDelay_us(GET_US_LOOPS(200) );	/* Wait for 200 us */
		if (RST_CLK_CPU_PLLstatus() == SUCCESS)				
			{
       /* Set CPU_C3_prescaler to 1 */
       RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
       /* Set CPU_C2_SEL to CPU_PLL output instead of CPU_C1 clock */
       RST_CLK_CPU_PLLuse(ENABLE);
       /* Select CPU_C3 clock on the CPU clock MUX */
       RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
			}	
		else	
			{
			NVIC_SystemReset ();
			__disable_irq (); while(1);	
			}
		}
	else
		{
		NVIC_SystemReset ();
		__disable_irq (); while(1);				
		}

  
	/* Enables the clock on PORTA */
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);
	PORT_DeInit(MDR_PORTA);			
	/* Enables the clock on PORTB */
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);	
	PORT_DeInit(MDR_PORTB);			
  /* Enables the clock on PORTC */
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
	PORT_DeInit(MDR_PORTC);			
	/* Enables the clock on PORTD */
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTD, ENABLE);
	PORT_DeInit(MDR_PORTD);	
	/* Enables the clock on PORTE */
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTE, ENABLE);
	PORT_DeInit(MDR_PORTE);			
	/* Enables the HSI clock on PORTF */
  RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTF, ENABLE);
	PORT_DeInit(MDR_PORTF);			
	
	// Technological Patch for DMA
	RST_CLK_PCLKcmd( RST_CLK_PCLK_SSP1 | RST_CLK_PCLK_SSP2 ,ENABLE);
	__NOP();
	RST_CLK_PCLKcmd( RST_CLK_PCLK_SSP1 | RST_CLK_PCLK_SSP2 ,DISABLE);
	// Disable all DMA request 
	RST_CLK_PCLKcmd( RST_CLK_PCLK_DMA,ENABLE);
  MDR_DMA->CHNL_REQ_MASK_SET = 0xFFFFFFFF; // KN 12.10.15
	// Disable all sreq 
  MDR_DMA->CHNL_USEBURST_SET = 0xFFFFFFFF; // KN 12.10.15 disable all sreq	
	NVIC_ClearPendingIRQ(DMA_IRQn);
	
}	

void TimersInit(void)
{
	// configure  
	// tM1 for 100Gz period interrupt
	// tM2 for 300Gz period interrupt
	// tM3 for 500000Gz period interrupt
	
	TIMER_CntInitTypeDef sTIM_CntInit;

	RST_CLK_PCLKcmd( RST_CLK_PCLK_TIMER1 ,ENABLE);
	RST_CLK_PCLKcmd( RST_CLK_PCLK_TIMER2 ,ENABLE);	
	RST_CLK_PCLKcmd( RST_CLK_PCLK_TIMER3 ,ENABLE);
  /* Reset all TIMER1 settings */
  TIMER_DeInit(MDR_TIMER1);
	TIMER_DeInit(MDR_TIMER2);
	TIMER_DeInit(MDR_TIMER3);
	RST_CLK_GetClocksFreq(&CLK);
	CPUclk = CLK.CPU_CLK_Frequency;
	
  TIMER_BRGInit(MDR_TIMER1,TIMER_HCLKdiv1);
	TIMER_BRGInit(MDR_TIMER2,TIMER_HCLKdiv1);
	TIMER_BRGInit(MDR_TIMER3,TIMER_HCLKdiv1);

  /* TIM1 configuration ------------------------------------------------*/
  /* Initializes the TIMERx Counter ------------------------------------*/
  sTIM_CntInit.TIMER_Prescaler                = (uint16_t)(CPUclk / 1000000 ) -1;
  sTIM_CntInit.TIMER_Period                   = (1000000/100) -1;
  sTIM_CntInit.TIMER_CounterMode              = TIMER_CntMode_ClkFixedDir;
  sTIM_CntInit.TIMER_CounterDirection         = TIMER_CntDir_Up;
  sTIM_CntInit.TIMER_EventSource              = TIMER_EvSrc_None;
  sTIM_CntInit.TIMER_FilterSampling           = TIMER_FDTS_TIMER_CLK_div_1;
  sTIM_CntInit.TIMER_ARR_UpdateMode           = TIMER_ARR_Update_Immediately;
  sTIM_CntInit.TIMER_ETR_FilterConf           = TIMER_Filter_1FF_at_TIMER_CLK;
  sTIM_CntInit.TIMER_ETR_Prescaler            = TIMER_ETR_Prescaler_None;
  sTIM_CntInit.TIMER_ETR_Polarity             = TIMER_ETRPolarity_NonInverted;
  sTIM_CntInit.TIMER_BRK_Polarity             = TIMER_BRKPolarity_NonInverted;
  TIMER_CntInit (MDR_TIMER1,&sTIM_CntInit);	
	NVIC_SetPriority (Timer1_IRQn, 4);	
	TIMER_ITConfig(MDR_TIMER1, TIMER_STATUS_CNT_ARR, ENABLE);
	TIMER_Cmd(MDR_TIMER1, ENABLE);
	NVIC_EnableIRQ(Timer1_IRQn);	
	
	sTIM_CntInit.TIMER_Prescaler                = (uint16_t)(CPUclk / 3000000 ) -1;
  sTIM_CntInit.TIMER_Period                   = (3000000/300) -1;
  TIMER_CntInit (MDR_TIMER2,&sTIM_CntInit);	
	NVIC_SetPriority (Timer2_IRQn, 4);	
	TIMER_ITConfig(MDR_TIMER2, TIMER_STATUS_CNT_ARR, ENABLE);
	TIMER_Cmd(MDR_TIMER2, ENABLE);
	NVIC_EnableIRQ(Timer2_IRQn);	
	
	sTIM_CntInit.TIMER_Prescaler                = 1;
  sTIM_CntInit.TIMER_Period                   = (24000000/500000) -1;
  TIMER_CntInit (MDR_TIMER3,&sTIM_CntInit);	
	NVIC_SetPriority (Timer3_IRQn, 2);	
	TIMER_ITConfig(MDR_TIMER3, TIMER_STATUS_CNT_ARR, ENABLE);	
	TIMER_Cmd(MDR_TIMER3, ENABLE);
	NVIC_EnableIRQ(Timer3_IRQn);

}

void Timer1_IRQHandler(void)
{
//change state of port's pin	
MDR_TIMER1->STATUS &= ~TIMER_STATUS_CNT_ARR;	
PortPins.b2 ^=1;

}

void Timer2_IRQHandler(void)
{
//change state of port's pin		
MDR_TIMER2->STATUS &= ~TIMER_STATUS_CNT_ARR;	
PortPins.b5 ^=1;
}

void Timer3_IRQHandler(void)
{
//change state of port's pin		
MDR_TIMER3->STATUS &= ~TIMER_STATUS_CNT_ARR;	
PortPins.b4 ^=1;
}

void InitSPI1(void)
{
	// configure SPI1: 2MGz, 16bits, Motorola packet
	
  /* Enable peripheral clocks --------------------------------------------------*/
  RST_CLK_PCLKcmd((RST_CLK_PCLK_RST_CLK | RST_CLK_PCLK_SSP1 ),ENABLE);
  RST_CLK_PCLKcmd((RST_CLK_PCLK_PORTF), ENABLE);
	RST_CLK_PCLKcmd((RST_CLK_PCLK_PORTC), ENABLE);
	
	/* Reset all SSP settings */
  SSP_DeInit(MDR_SSP1);
  SSP_BRGInit(MDR_SSP1,SSP_HCLKdiv2);	// 48/2 = 24MGz

  /* Reset PORTF settings */
	PORT_DeInit(MDR_PORTF);
	PORT_StructInit(&PORT_InitStructure);
  /* Configure SSP1 pins: TXD, CLK, FSS, RXD  */
  /* Configure PORTF pins 0,   1,   2,   3 */
  PORT_InitStructure.PORT_Pin   = (PORT_Pin_3);;
  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
  PORT_InitStructure.PORT_FUNC  = PORT_FUNC_ALTER;
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_PULL_DOWN	= PORT_PULL_DOWN_ON;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;//PORT_SPEED_FAST
  PORT_Init(MDR_PORTF, &PORT_InitStructure);
  PORT_InitStructure.PORT_Pin   = (PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_0);
  PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
  PORT_Init(MDR_PORTF, &PORT_InitStructure);
	
	PORT_DeInit(MDR_PORTC);
	PORT_StructInit(&PORT_InitStructure);
  /* Configure SSP1 pins: nCS0, nCS1  */
  /* Configure PORTF pins 0,    1 */
  PORT_InitStructure.PORT_Pin   = ( PORT_Pin_0 + PORT_Pin_1 );
  PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
	PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
	PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;//PORT_SPEED_FAST
  PORT_Init(MDR_PORTC, &PORT_InitStructure);
	MDR_PORTC->RXTX |= ( PORT_Pin_0 + PORT_Pin_1 );	// set nCS0 =1; nCS1 =1;

  /* SSP1 Slave configuration ------------------------------------------------*/
  SSP_StructInit (&sSSP);

  sSSP.SSP_SCR  = 0;
  sSSP.SSP_CPSDVSR = 12; // 24/12 = 2MGz
  sSSP.SSP_Mode = SSP_ModeMaster;
  sSSP.SSP_WordLength = SSP_WordLength16b;
  sSSP.SSP_FRF = SSP_FRF_SPI_Motorola;//SSP_FRF_SSI_TI;//
  sSSP.SSP_HardwareFlowControl = SSP_HardwareFlowControl_SSE;	//Enable
  SSP_Init (MDR_SSP1,&sSSP);
}

void SysTick_Handler(void)
{
// send ADC reading to two SPI devices	
// update ADC reading every 1 sec	
	
SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;	
CntMs++;	
nCS.b0=0; // enable device0	
MDR_SSP1->DR = (uint16_t) ADCdata;
while (MDR_SSP1->SR & SSP_FLAG_BSY);	
nCS.b0=1; // disable device0	
nCS.b1=0; // enable device1		
MDR_SSP1->DR = (uint16_t) ADCdata;	
while (MDR_SSP1->SR & SSP_FLAG_BSY);	
nCS.b1=1; // disable device1	
if ((CntMs%10)==0) // 1 sec
		{
		ADCdata = MDR_ADC->ADC1_RESULT & 0xFFF;
		MDR_ADC->ADC1_CFG |= ADC1_CFG_REG_GO;}
}	

void ADC_Config(void)  	// Configure PORTD.2 as ADC input
{
	/* ADC Init structures */
	ADC_InitTypeDef  ADC_InitStructure;
	ADCx_InitTypeDef ADCx_InitStructure;

	/* Enable the RTCHSE clock on ADC1 */
  RST_CLK_PCLKcmd((RST_CLK_PCLK_ADC), ENABLE);
	RST_CLK_PCLKcmd((RST_CLK_PCLK_PORTD), ENABLE);	
  /* Configure ADC pin: ADC2 */  // 
  /* Configure PORTD pin 2 */
  PORT_InitStructure.PORT_Pin   = PORT_Pin_2;
  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
  PORT_InitStructure.PORT_MODE  = PORT_MODE_ANALOG;
  PORT_Init(MDR_PORTD, &PORT_InitStructure);	

  /* ADC Configuration */
  /* Reset all ADC settings */
  ADC_DeInit();

  ADC_StructInit(&ADC_InitStructure);
  ADC_InitStructure.ADC_SynchronousMode      = ADC_SyncMode_Independent;
	ADC_InitStructure.ADC_TempSensorAmplifier  = ADC_TEMP_SENSOR_AMPLIFIER_Enable;
  /* Use base voltage source to reduce the power consumption,
  / thus enable the temperature sensor */
  ADC_InitStructure.ADC_TempSensor           = ADC_TEMP_SENSOR_Enable;
  ADC_Init (&ADC_InitStructure);
	
	ADCx_StructInit(&ADCx_InitStructure);	   
	ADCx_InitStructure.ADC_ClockSource			= ADC_CLOCK_SOURCE_CPU; 
  ADCx_InitStructure.ADC_SamplingMode     = ADC_SAMPLING_MODE_SINGLE_CONV;  
  ADCx_InitStructure.ADC_ChannelNumber    = ADC_CH_ADC2;
  ADCx_InitStructure.ADC_IntVRefSource    = ADC_INT_VREF_SOURCE_EXACT;
  ADCx_InitStructure.ADC_Prescaler        = ADC_CLK_div_128; 
  ADCx_InitStructure.ADC_DelayGo          = 0x7;
  ADC1_Init (&ADCx_InitStructure);
	
}

uint32_t UARTx_Set_BaudRate( MDR_UART_TypeDef* UARTx)  	// set baudrate for specified UART
{
	uint32_t cpuclock;
	uint32_t realspeed, speederror;
	uint32_t divider;
	uint32_t integerdivider;
	uint32_t fractionaldivider;
	RST_CLK_FreqTypeDef RST_CLK_Clocks;	
	
	//static UART_InitTypeDef UART_InitStructure;
	RST_CLK_GetClocksFreq(&RST_CLK_Clocks);
	cpuclock = RST_CLK_Clocks.CPU_CLK_Frequency;	
	
	/* Determine the integer part */
	divider = (8*cpuclock)/ (UART_InitStructure.UART_BaudRate);
	divider = (divider+1)>>1;
	integerdivider = divider >> 6;
	/* Determine the fractional part */
	fractionaldivider = (divider & 0x3F);
	/* Determine the speed error */
	realspeed = (4*cpuclock ) / ((integerdivider * 64) + fractionaldivider);
	speederror = ((realspeed - UART_InitStructure.UART_BaudRate) * 128)
								/ UART_InitStructure.UART_BaudRate;
	/* Write UART Baud Rate */
	UARTx->IBRD = integerdivider;
	UARTx->FBRD = fractionaldivider;	
	return 	speederror;
}


void
UartInit(  uint32_t ulBaudRate )
{
	// configure UART: 115200baud, 8bits
	/* Enables the clock on PORTB */
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTD, ENABLE);	
	
  /* Fill PortInit structure */
  PORT_InitStructure.PORT_PULL_UP = PORT_PULL_UP_OFF;	//PORT_PULL_UP_ON;
  PORT_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
  PORT_InitStructure.PORT_PD_SHM = PORT_PD_SHM_OFF;
  PORT_InitStructure.PORT_PD = PORT_PD_DRIVER;	
  PORT_InitStructure.PORT_GFEN = PORT_GFEN_OFF;
  PORT_InitStructure.PORT_FUNC = PORT_FUNC_ALTER;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;	
  PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;

  /* Configure PORTD pins 1 (UART2_TX) as output */
  PORT_InitStructure.PORT_OE = PORT_OE_OUT;
  PORT_InitStructure.PORT_Pin = PORT_Pin_1;
  PORT_Init(MDR_PORTD, &PORT_InitStructure);

  /* Configure PORTD pins 0 (UART2_RX) as input */
  PORT_InitStructure.PORT_OE = PORT_OE_IN;
  PORT_InitStructure.PORT_Pin = PORT_Pin_0;
  PORT_Init(MDR_PORTD, &PORT_InitStructure);	
	
  /* Enables the CPU_CLK clock on UART1 */
  RST_CLK_PCLKcmd(RST_CLK_PCLK_UART2, ENABLE);

  /* Set the HCLK division factor = 1 for UART1,UART2 */
  UART_BRGInit(MDR_UART2, UART_HCLKdiv1);
	
	UART_DeInit(MDR_UART2);
	UART_StructInit(&UART_InitStructure);

  /* Initialize UART_InitStructure */
  UART_InitStructure.UART_BaudRate                = (uint32_t)ulBaudRate; 
  UART_InitStructure.UART_WordLength              = UART_WordLength8b;
  UART_InitStructure.UART_StopBits                = UART_StopBits1;
  UART_InitStructure.UART_Parity                  = UART_Parity_No;
  UART_InitStructure.UART_FIFOMode                = UART_FIFO_OFF;
  UART_InitStructure.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

  /* Configure UART1 parameters */
  UART_Init (MDR_UART2,&UART_InitStructure);
	UARTx_Set_BaudRate(MDR_UART2);  
	
  /* Enable transmitter interrupt (UARTTXINTR) */
  UART_Cmd(MDR_UART2,ENABLE);	
}






#ifdef __CC_ARM 
int main(void)
#else
void main(void)
#endif
{ 
	InitBoard();

	PORT_StructInit(&PORT_InitStructure);	
	//   Test 											LED0, 		    LED1        LEDx                     
  PORT_InitStructure.PORT_Pin   = PORT_Pin_2 + PORT_Pin_4 + PORT_Pin_5;		
  PORT_InitStructure.PORT_OE    = PORT_OE_OUT;  
	PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;  // 
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;
  PORT_Init(MDR_PORTA, &PORT_InitStructure);	
	
	PORT_StructInit(&PORT_InitStructure);	
	//   Test 											LED2, 		                      
  PORT_InitStructure.PORT_Pin   = PORT_Pin_5;		
  PORT_InitStructure.PORT_OE    = PORT_OE_OUT;  
	PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;  // 
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;
  PORT_Init(MDR_PORTB, &PORT_InitStructure);		
	

	ADC_Config();
	ADC1_Cmd (ENABLE); ADC1_Start();
	while((MDR_ADC->ADC1_STATUS & ADC1_FLAG_END_OF_CONVERSION)==0);
	MDR_ADC->ADC1_STATUS =0;
	ADCdata = MDR_ADC->ADC1_RESULT & 0xFFF;

	InitSPI1();
	UartInit(115200);
	
	NVIC_SetPriority (SysTick_IRQn, 3);	// set  priority 
	RST_CLK_GetClocksFreq(&RCC_ClocksStatus);
	CPUclk = RCC_ClocksStatus.CPU_CLK_Frequency;
	SysTick->VAL = 0;
  SysTick->LOAD = (CPUclk / 10) -1;	// 100ms
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk; 	

	TimersInit();	
	
	while(1)
	{
	while(MDR_UART2->FR	& UART_FLAG_RXFE); // wait till RX buffer is not empty
  MDR_UART2->DR = (MDR_UART2->DR & (uint16_t)0x00FF); // send the received byte
	}
		
}



/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the source file ID, the source line number 
*                : and (if USE_ASSERT_INFO == 2) expression text where
*                : the assert_param error has occurred.
* Input          : file_id - pointer to the source file name
*                : line - assert_param error line source number
*                : expr - expression text
* Output         : None
* Return         : None
*******************************************************************************/

#if (USE_ASSERT_INFO == 1)
void assert_failed(uint32_t file_id, uint32_t line)
{
  /* User can add his own implementation to report the source file ID and line number.
     Ex: printf("Wrong parameters value: file Id %d on line %d\r\n", file_id, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint32_t file_id, uint32_t line, const uint8_t* expr)
{
  /* User can add his own implementation to report the source file ID, line number and
     expression text.
     Ex: printf("Wrong parameters value (%s): file Id %d on line %d\r\n", expr, file_id, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_ASSERT_INFO */

/* END OF FILE Main.c */

