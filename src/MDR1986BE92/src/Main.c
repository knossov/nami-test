/**
  ******************************************************************************
  * @file    Main.c
  * @author  Phyton Application Team
  * @version V3.0.0
  * @date    10.09.2011
  * @brief   Main program body
  ******************************************************************************
  * <br><br>
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, PHYTON SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 Phyton</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <string.h>
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
//#include "MDR32F9Qx_iwdg.h"
#include "MDR32F9Qx_timer.h"
//#include "..\modbus\mb.h"
#include "port.h"
//#include "..\jdt_1800\disp.h"
//#include "..\jdt_1800\jdt1800_drv.h"
#include "ComDef.h"


static RST_CLK_FreqTypeDef RCC_ClocksStatus;	
static RST_CLK_FreqTypeDef CLK;
static SSP_InitTypeDef sSSP;
static uint32_t CPUclk;
static uint32_t ADCdata __attribute__((at(0x2000FFFC))); 

//uint32_t InpStatus;
static uint32_t CntMs;
//extern float FlashFreqMHz;

uint16_t ComReg =PEN;
uint32_t trim, trim1;
//uint32_t d_eeprom=0;

//extern USHORT usRegInputBuf[];
//extern USHORT usRegHoldingBuf[];
//extern unsigned char ucRegDiscretesBuf[];
//extern unsigned char ucRegCoilsBuf[];


uint32_t Cnt1,Cnt2;//

//static uint16_t i,k,n;


uint32_t mbBaudrate,rs485Baudrate;
uint8_t mbAddress, rs485Address;

//extern uint16_t STPcnt;

//uint16_t position;

extern void ProgramDelay_us(uint32_t Loops); 
//extern void  Init_SPI1(void);
//extern void StepTimerInit( uint16_t  );
//extern void StepTimerEnable( void );
//extern void StepTimerClose(void);

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

#define	LED0		PORT_Pin_2
#define	START 	0x01
#define	MOV 		0x02
#define	RESET 	0x08

#define	BUSY 		0x01			
#define	FAULT 	0x02	

#define	nRST		 	PORT_Pin_2//0x04	
#define	nEN		 		PORT_Pin_4//0x40		
#define	nSLP		 	PORT_Pin_5//	0x20	
#define	nFLT		 	0x08	

// debug printf ////////////////////////////////////////////////////////
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle; /* Add whatever needed */ };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}
// debug printf ////////////////////////////////////////////////////////

/** @addtogroup __MDR32F9Qx_Eval_Demo MDR32F9Qx Demonstration Example
  * @{
  */

/** @addtogroup Main Main
  * @{
  */

/** @defgroup Main_Functions Main Functions
  * @{
  */
//eMBErrorCode  eStatus;
static PORT_InitTypeDef PORT_InitStructure;
//static UART_InitTypeDef UART_InitStructure;

void InitBoard (void)
{
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
    //RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul3);	// HSE * 3
		RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv2, RST_CLK_CPU_PLLmul6);	// (HSE/2) * 6	
    /* Enable CPU_PLL */
    RST_CLK_CPU_PLLcmd(ENABLE);
		//Cnt1 = 200; // about 200us	for 24MHz 
		//for (;Cnt1>0;Cnt1--) {}			
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





//void TimersInit( uint32_t F1, uint32_t F2, uint32_t F3)
void TimersInit(void)
{
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
MDR_TIMER1->STATUS &= ~TIMER_STATUS_CNT_ARR;	
PortPins.b2 ^=1;

}

void Timer2_IRQHandler(void)
{
MDR_TIMER2->STATUS &= ~TIMER_STATUS_CNT_ARR;	
PortPins.b5 ^=1;
}

void Timer3_IRQHandler(void)
{
MDR_TIMER3->STATUS &= ~TIMER_STATUS_CNT_ARR;	
PortPins.b4 ^=1;
}

void InitSPI1(void)
{
  /* Enable peripheral clocks --------------------------------------------------*/
  RST_CLK_PCLKcmd((RST_CLK_PCLK_RST_CLK | RST_CLK_PCLK_SSP1 | RST_CLK_PCLK_DMA),ENABLE);
  RST_CLK_PCLKcmd((RST_CLK_PCLK_PORTF), ENABLE);
	RST_CLK_PCLKcmd((RST_CLK_PCLK_PORTC), ENABLE);
	
	/* Reset all SSP settings */
  SSP_DeInit(MDR_SSP1);
  SSP_BRGInit(MDR_SSP1,SSP_HCLKdiv2);

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
  sSSP.SSP_CPSDVSR = 12;//48;//120;//4*8; 
  sSSP.SSP_Mode = SSP_ModeMaster;
	//sSSP.SSP_SPH = SSP_SPH_1Edge; 
	//sSSP.SSP_SPO = SSP_SPO_High;
  sSSP.SSP_WordLength = SSP_WordLength16b;
  sSSP.SSP_FRF = SSP_FRF_SPI_Motorola;//SSP_FRF_SSI_TI;//
  sSSP.SSP_HardwareFlowControl = //SSP_HardwareFlowControl_None;
																SSP_HardwareFlowControl_SSE;	//Enable
																//SSP_HardwareFlowControl_LBM; TX -> RX
  SSP_Init (MDR_SSP1,&sSSP);
	//MDR_SSP1-> CR0 |= 1<<6; // SPO
	//MDR_SSP1-> CR0 |= 1<<7; // SPH	
}

void SysTick_Handler(void)
{
SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;	
CntMs++;	
nCS.b0=0; // enable device0	
MDR_SSP1->DR = (uint16_t) ADCdata;
//while((SSP_GetFlagStatus(MDR_SSP1, SSP_FLAG_BSY))) {}; 	// wait till Tx is empty	
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

void ADC_Config(void)
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
	
	ADCx_StructInit(&ADCx_InitStructure);	   ////////////////////
	ADCx_InitStructure.ADC_ClockSource			= ADC_CLOCK_SOURCE_CPU; // ADC_CLOCK_SOURCE_ADC
  ADCx_InitStructure.ADC_SamplingMode     = ADC_SAMPLING_MODE_SINGLE_CONV; //ADC_SAMPLING_MODE_CICLIC_CONV; 
  ADCx_InitStructure.ADC_ChannelNumber    = ADC_CH_ADC2;
  ADCx_InitStructure.ADC_IntVRefSource    = ADC_INT_VREF_SOURCE_EXACT;
  ADCx_InitStructure.ADC_Prescaler        = ADC_CLK_div_128; //ADC_CLK_div_32768;
  ADCx_InitStructure.ADC_DelayGo          = 0x7;
  ADC1_Init (&ADCx_InitStructure);
	
	
}
	
/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
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
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;//PORT_SPEED_MAXFAST;
  PORT_Init(MDR_PORTA, &PORT_InitStructure);	
	
	PORT_StructInit(&PORT_InitStructure);	
	//   Test 											LED2, 		                      
  PORT_InitStructure.PORT_Pin   = PORT_Pin_5;		
  PORT_InitStructure.PORT_OE    = PORT_OE_OUT;  
	PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;  // 
  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
  PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;//PORT_SPEED_MAXFAST;
  PORT_Init(MDR_PORTB, &PORT_InitStructure);		
	

	ADC_Config();
	ADC1_Cmd (ENABLE); ADC1_Start();
	while((MDR_ADC->ADC1_STATUS & ADC1_FLAG_END_OF_CONVERSION)==0);
	MDR_ADC->ADC1_STATUS =0;
	ADCdata = MDR_ADC->ADC1_RESULT & 0xFFF;

	InitSPI1();
	
	NVIC_SetPriority (SysTick_IRQn, 3);	// set  priority 
	RST_CLK_GetClocksFreq(&RCC_ClocksStatus);
	CPUclk = RCC_ClocksStatus.CPU_CLK_Frequency;
	SysTick->VAL = 0;
  SysTick->LOAD = (CPUclk / 10) -1;	// 100ms
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk; 	

	TimersInit();	
	
	while(1)
	{__NOP();}
		
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

/** @} */ /* End of group Main_Functions */

/** @} */ /* End of group Main */

/** @} */ /* End of group __MDR32F9Qx_Eval_Demo */

/******************* (C) COPYRIGHT 2011 Phyton *********************************
*
* END OF FILE Main.c */

