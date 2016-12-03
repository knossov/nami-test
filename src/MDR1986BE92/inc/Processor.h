#include "stdint.h"

void processor (void);
float F1x (float );
float dF1x (float );
float P1n (float , float );
float F2x (float );
float dF2x (float );
float P2n (float , float);
float FT1x (float );
float dFT1x (float );
float T1n (float , float );
float FT2x (float );
float dFT2x (float );
float T2n (float , float);
float Tn (float , float , uint16_t );

uint16_t MaxIt = 8;
uint16_t RT1Time;
uint16_t RT2Time;
uint16_t P1Time;
uint16_t P2Time;


float P1_0 = 0.5F; // kgs/cm2
float P2_0 = 0.5F; // kgs/cm2
float E1 = 0.0001F;	// kgs/cm2
float E2 = 0.0001F;	// kgs/cm2
float RT1_0 = 0.5F; // kgs/cm2
float RT2_0 = 0.5F; // kgs/cm2
float E3 = 0.0001F;	// kgs/cm2
float E4 = 0.0001F;	// kgs/cm2
float Ex[6] = {0.0001F};
uint32_t mbBaudrate = 115200; //57600;	// default value
uint8_t mbAddress = 0x01;		// default	value
uint32_t rs485Baudrate = 115200; 	// default value
uint8_t rs485Address = 0x30;	

float TP1_0;
float TP2_0;
float	RTx_0[6];

float InputP1;
float InputP2;
float InputTP1;
float InputTP2;
float InputRT1;
float InputRT2;


float A1_0;
float A1_1;
float A1_2;
float A1_3;
float A2_0;
float A2_1;
float A2_2;
float A2_3;

uint16_t P1max	= 0xFFF; 
uint16_t P1min	= 0;
uint16_t P2max	=	0xFFF; 
uint16_t P2min	=	0;
uint16_t TP1max	= 0xFFF; 
uint16_t TP1min	=	0;
uint16_t TP2max	= 0xFFF; 
uint16_t TP2min	=	0;
uint16_t RT1max	= 0xFFF; 
uint16_t RT1min	= 0;	
uint16_t RT2max	=	0xFFF; 
uint16_t RT2min	=	0;

uint16_t RT3max	= 0xFFF; 
uint16_t RT3min	= 0;	
uint16_t RT4max	=	0xFFF; 
uint16_t RT4min	=	0;
uint16_t RT5max	= 0xFFF; 
uint16_t RT5min	= 0;	
uint16_t RT6max	=	0xFFF; 
uint16_t RT6min	=	0;
uint16_t RT7max	= 0xFFF; 
uint16_t RT7min	= 0;	
uint16_t RT8max	=	0xFFF; 
uint16_t RT8min	=	0;

uint16_t PGA[6] = {0,0,0,0,0,0};
uint32_t CAN_ID[4] = {326,325,324,0};
uint32_t CANbaudrate = 1000000;
uint8_t NodeID;

struct
{
//float StP;
//float DifP;
//float StT;
uint32_t baudrate;
uint16_t NodeID;	
uint16_t ID1;	
uint16_t ID2;	
uint16_t ID3;	
uint16_t ID4;	
uint16_t ID5;	
uint16_t ID6;	
uint16_t ID7;	
uint16_t ID8;	
} __attribute__((packed)) CAN =	
{
//0,
//0,
//0,
1000000,
1,
326,
325,
324,
322,
316,
250,
1324,
1326
	
};	
#define CANlen sizeof(CAN)
	

struct type_Ai
{
float A[10];	
}__attribute__((packed));


struct PolynomFactors
{
float A1_0_0;
float A1_0_1;
float A1_0_2;
float A1_0_3;
float A1_0_4;
float A1_0_5;
float A1_0_6;
float A1_0_7;
float A1_0_8;
float A1_0_9;	
float A1_1_0;
float A1_1_1;
float A1_1_2;
float A1_1_3;
float A1_1_4;
float A1_1_5;
float A1_1_6;
float A1_1_7;
float A1_1_8;
float A1_1_9;
float A1_2_0;
float A1_2_1;
float A1_2_2;
float A1_2_3;
float A1_2_4;
float A1_2_5;
float A1_2_6;
float A1_2_7;
float A1_2_8;
float A1_2_9;
float A1_3_0;
float A1_3_1;
float A1_3_2;
float A1_3_3;
float A1_3_4;
float A1_3_5;
float A1_3_6;
float A1_3_7;
float A1_3_8;
float A1_3_9;
float A2_0_0;
float A2_0_1;
float A2_0_2;
float A2_0_3;
float A2_0_4;
float A2_0_5;
float A2_0_6;
float A2_0_7;
float A2_0_8;
float A2_0_9;
float A2_1_0;
float A2_1_1;
float A2_1_2;
float A2_1_3;
float A2_1_4;
float A2_1_5;
float A2_1_6;
float A2_1_7;
float A2_1_8;
float A2_1_9;
float A2_2_0;
float A2_2_1;
float A2_2_2;
float A2_2_3;
float A2_2_4;
float A2_2_5;
float A2_2_6;
float A2_2_7;
float A2_2_8;
float A2_2_9;
float A2_3_0;
float A2_3_1;
float A2_3_2;
float A2_3_3;
float A2_3_4;
float A2_3_5;
float A2_3_6;
float A2_3_7;
float A2_3_8;
float A2_3_9;
float A3_0;
float A3_1;
float A3_2;
float A3_3;
float A3_4;
float A3_5;
float A3_6;
float A3_7;
float A3_8;
float A3_9;
float A4_0;
float A4_1;
float A4_2;
float A4_3;
float A4_4;
float A4_5;
float A4_6;
float A4_7;
float A4_8;
float A4_9;
//float Akn[10][6];
struct type_Ai ADC[6];
} __attribute__((packed)) PF = 
{
/*float A1_0_0 =*/ 43776.17845,
/*float A1_0_1 =*/ 0.05827,
/*float A1_0_2 =*/ 0.0004077,
/*float A1_0_3 =*/ 0,
/*float A1_0_4 =*/ 0,
/*float A1_0_5 =*/ 0,
/*float A1_0_6 =*/ 0,
/*float A1_0_7 =*/ 0,
/*float A1_0_8 =*/ 0,
/*float A1_0_9 =*/ 0,	
/*float A1_1_0 =*/ 1939.33736,
/*float A1_1_1 =*/ -0.1902936,
/*float A1_1_2 =*/ 0,
/*float A1_1_3 =*/ 0,
/*float A1_1_4 =*/ 0,
/*float A1_1_5 =*/ 0,
/*float A1_1_6 =*/ 0,
/*float A1_1_7 =*/ 0,
/*float A1_1_8 =*/ 0,
/*float A1_1_9 =*/ 0,	
/*float A1_2_0 =*/ -43.588019,
/*float A1_2_1 =*/ 0.015982,
/*float A1_2_2 =*/ 0,
/*float A1_2_3 =*/ 0,
/*float A1_2_4 =*/ 0,
/*float A1_2_5 =*/ 0,
/*float A1_2_6 =*/ 0,
/*float A1_2_7 =*/ 0,
/*float A1_2_8 =*/ 0,
/*float A1_2_9 =*/ 0,
/*float A1_3_0 =*/ 0,
/*float A1_3_1 =*/ 0,
/*float A1_3_2 =*/ 0,
/*float A1_3_3 =*/ 0,
/*float A1_3_4 =*/ 0,
/*float A1_3_5 =*/ 0,
/*float A1_3_6 =*/ 0,
/*float A1_3_7 =*/ 0,
/*float A1_3_8 =*/ 0,
/*float A1_3_9 =*/ 0,
/*float A2_0_0 =*/ 0,
/*float A2_0_1 =*/ 0,
/*float A2_0_2 =*/ 0,
/*float A2_0_3 =*/ 0,
/*float A2_0_4 =*/ 0,
/*float A2_0_5 =*/ 0,
/*float A2_0_6 =*/ 0,
/*float A2_0_7 =*/ 0,
/*float A2_0_8 =*/ 0,
/*float A2_0_9 =*/ 0,
/*float A2_1_0 =*/ 0,
/*float A2_1_1 =*/ 0,
/*float A2_1_2 =*/ 0,
/*float A2_1_3 =*/ 0,
/*float A2_1_4 =*/ 0,
/*float A2_1_5 =*/ 0,
/*float A2_1_6 =*/ 0,
/*float A2_1_7 =*/ 0,
/*float A2_1_8 =*/ 0,
/*float A2_1_9 =*/ 0,
/*float A2_2_0 =*/ 0,
/*float A2_2_1 =*/ 0,
/*float A2_2_2 =*/ 0,
/*float A2_2_3 =*/ 0,
/*float A2_2_4 =*/ 0,
/*float A2_2_5 =*/ 0,
/*float A2_2_6 =*/ 0,
/*float A2_2_7 =*/ 0,
/*float A2_2_8 =*/ 0,
/*float A2_2_9 =*/ 0,
/*float A2_3_0 =*/ 0,
/*float A2_3_1 =*/ 0,
/*float A2_3_2 =*/ 0,
/*float A2_3_3 =*/ 0,
/*float A2_3_4 =*/ 0,
/*float A2_3_5 =*/ 0,
/*float A2_3_6 =*/ 0,
/*float A2_3_7 =*/ 0,
/*float A2_3_8 =*/ 0,
/*float A2_3_9 =*/ 0,
/*float A3_0 =*/ 0,
/*float A3_1 =*/ 0,
/*float A3_2 =*/ 0,
/*float A3_3 =*/ 0,
/*float A3_4 =*/ 0,
/*float A3_5 =*/ 0,
/*float A3_6 =*/ 0,
/*float A3_7 =*/ 0,
/*float A3_8 =*/ 0,
/*float A3_9 =*/ 0,
/*float A4_0 =*/ 0,
/*float A4_1 =*/ 0,
/*float A4_2 =*/ 0,
/*float A4_3 =*/ 0,
/*float A4_4 =*/ 0,
/*float A4_5 =*/ 0,
/*float A4_6 =*/ 0,
/*float A4_7 =*/ 0,
/*float A4_8 =*/ 0,
/*float A4_9 =*/ 0,
/*float A5_0 =*/ 0,
/*float A5_1 =*/ 0,
/*float A5_2 =*/ 0,
/*float A5_3 =*/ 0,
/*float A5_4 =*/ 0,
/*float A5_5 =*/ 0,
/*float A5_6 =*/ 0,
/*float A5_7 =*/ 0,
/*float A5_8 =*/ 0,
/*float A5_9 =*/ 0,
/*float A6_0 =*/ 0,
/*float A6_1 =*/ 0,
/*float A6_2 =*/ 0,
/*float A6_3 =*/ 0,
/*float A6_4 =*/ 0,
/*float A6_5 =*/ 0,
/*float A6_6 =*/ 0,
/*float A6_7 =*/ 0,
/*float A6_8 =*/ 0,
/*float A6_9 =*/ 0,
/*float A7_0 =*/ 0,
/*float A7_1 =*/ 0,
/*float A7_2 =*/ 0,
/*float A7_3 =*/ 0,
/*float A7_4 =*/ 0,
/*float A7_5 =*/ 0,
/*float A7_6 =*/ 0,
/*float A7_7 =*/ 0,
/*float A7_8 =*/ 0,
/*float A7_9 =*/ 0,
/*float A7_0 =*/ 0,
/*float A8_1 =*/ 0,
/*float A8_2 =*/ 0,
/*float A8_3 =*/ 0,
/*float A8_4 =*/ 0,
/*float A8_5 =*/ 0,
/*float A8_6 =*/ 0,
/*float A8_7 =*/ 0,
/*float A8_8 =*/ 0,
/*float A8_9 =*/ 0,
/*float A9_0 =*/ 0,
/*float A9_1 =*/ 0,
/*float A9_2 =*/ 0,
/*float A9_3 =*/ 0,
/*float A9_4 =*/ 0,
/*float A9_5 =*/ 0,
/*float A9_6 =*/ 0,
/*float A9_7 =*/ 0,
/*float A9_8 =*/ 0,
/*float A9_9 =*/ 0,
/*float A10_0 =*/ 0,
/*float A10_1 =*/ 0,
/*float A10_2 =*/ 0,
/*float A10_3 =*/ 0,
/*float A10_4 =*/ 0,
/*float A10_5 =*/ 0,
/*float A10_6 =*/ 0,
/*float A10_7 =*/ 0,
/*float A10_8 =*/ 0,
/*float A10_9 =*/ 0
};

struct PolynomFactors1
{
float A1_0_0;
float A1_0_1;
float A1_0_2;
float A1_0_3;
float A1_0_4;
float A1_0_5;
float A1_0_6;
float A1_0_7;
float A1_0_8;
float A1_0_9;	
float A1_1_0;
float A1_1_1;
float A1_1_2;
float A1_1_3;
float A1_1_4;
float A1_1_5;
float A1_1_6;
float A1_1_7;
float A1_1_8;
float A1_1_9;
float A1_2_0;
float A1_2_1;
float A1_2_2;
float A1_2_3;
float A1_2_4;
float A1_2_5;
float A1_2_6;
float A1_2_7;
float A1_2_8;
float A1_2_9;
float A1_3_0;
float A1_3_1;
float A1_3_2;
float A1_3_3;
float A1_3_4;
float A1_3_5;
float A1_3_6;
float A1_3_7;
float A1_3_8;
float A1_3_9;
float A2_0_0;
float A2_0_1;
float A2_0_2;
float A2_0_3;
float A2_0_4;
float A2_0_5;
float A2_0_6;
float A2_0_7;
float A2_0_8;
float A2_0_9;
float A2_1_0;
float A2_1_1;
float A2_1_2;
float A2_1_3;
float A2_1_4;
float A2_1_5;
float A2_1_6;
float A2_1_7;
float A2_1_8;
float A2_1_9;
float A2_2_0;
float A2_2_1;
float A2_2_2;
float A2_2_3;
float A2_2_4;
float A2_2_5;
float A2_2_6;
float A2_2_7;
float A2_2_8;
float A2_2_9;
float A2_3_0;
float A2_3_1;
float A2_3_2;
float A2_3_3;
float A2_3_4;
float A2_3_5;
float A2_3_6;
float A2_3_7;
float A2_3_8;
float A2_3_9;
float A3_0;
float A3_1;
float A3_2;
float A3_3;
float A3_4;
float A3_5;
float A3_6;
float A3_7;
float A3_8;
float A3_9;
float A4_0;
float A4_1;
float A4_2;
float A4_3;
float A4_4;
float A4_5;
float A4_6;
float A4_7;
float A4_8;
float A4_9;
float A5_0;
float A5_1;
float A5_2;
float A5_3;
float A5_4;
float A5_5;
float A5_6;
float A5_7;
float A5_8;
float A5_9;
float A6_0;
float A6_1;
float A6_2;
float A6_3;
float A6_4;
float A6_5;
float A6_6;
float A6_7;
float A6_8;
float A6_9;
float A7_0;
float A7_1;
float A7_2;
float A7_3;
float A7_4;
float A7_5;
float A7_6;
float A7_7;
float A7_8;
float A7_9;
float A8_0;
float A8_1;
float A8_2;
float A8_3;
float A8_4;
float A8_5;
float A8_6;
float A8_7;
float A8_8;
float A8_9;
float A9_0;
float A9_1;
float A9_2;
float A9_3;
float A9_4;
float A9_5;
float A9_6;
float A9_7;
float A9_8;
float A9_9;
float A10_0;
float A10_1;
float A10_2;
float A10_3;
float A10_4;
float A10_5;
float A10_6;
float A10_7;
float A10_8;
float A10_9;
} __attribute__((packed)) PF2 =
{
/*float A1_0_0 =*/ 43776.17845,
/*float A1_0_1 =*/ 0.05827,
/*float A1_0_2 =*/ 0.0004077,
/*float A1_0_3 =*/ 0,
/*float A1_0_4 =*/ 0,
/*float A1_0_5 =*/ 0,
/*float A1_0_6 =*/ 0,
/*float A1_0_7 =*/ 0,
/*float A1_0_8 =*/ 0,
/*float A1_0_9 =*/ 0,	
/*float A1_1_0 =*/ 1939.33736,
/*float A1_1_1 =*/ -0.1902936,
/*float A1_1_2 =*/ 0,
/*float A1_1_3 =*/ 0,
/*float A1_1_4 =*/ 0,
/*float A1_1_5 =*/ 0,
/*float A1_1_6 =*/ 0,
/*float A1_1_7 =*/ 0,
/*float A1_1_8 =*/ 0,
/*float A1_1_9 =*/ 0,	
/*float A1_2_0 =*/ -43.588019,
/*float A1_2_1 =*/ 0.015982,
/*float A1_2_2 =*/ 0,
/*float A1_2_3 =*/ 0,
/*float A1_2_4 =*/ 0,
/*float A1_2_5 =*/ 0,
/*float A1_2_6 =*/ 0,
/*float A1_2_7 =*/ 0,
/*float A1_2_8 =*/ 0,
/*float A1_2_9 =*/ 0,
/*float A1_3_0 =*/ 0,
/*float A1_3_1 =*/ 0,
/*float A1_3_2 =*/ 0,
/*float A1_3_3 =*/ 0,
/*float A1_3_4 =*/ 0,
/*float A1_3_5 =*/ 0,
/*float A1_3_6 =*/ 0,
/*float A1_3_7 =*/ 0,
/*float A1_3_8 =*/ 0,
/*float A1_3_9 =*/ 0,
/*float A2_0_0 =*/ 0,
/*float A2_0_1 =*/ 0,
/*float A2_0_2 =*/ 0,
/*float A2_0_3 =*/ 0,
/*float A2_0_4 =*/ 0,
/*float A2_0_5 =*/ 0,
/*float A2_0_6 =*/ 0,
/*float A2_0_7 =*/ 0,
/*float A2_0_8 =*/ 0,
/*float A2_0_9 =*/ 0,
/*float A2_1_0 =*/ 0,
/*float A2_1_1 =*/ 0,
/*float A2_1_2 =*/ 0,
/*float A2_1_3 =*/ 0,
/*float A2_1_4 =*/ 0,
/*float A2_1_5 =*/ 0,
/*float A2_1_6 =*/ 0,
/*float A2_1_7 =*/ 0,
/*float A2_1_8 =*/ 0,
/*float A2_1_9 =*/ 0,
/*float A2_2_0 =*/ 0,
/*float A2_2_1 =*/ 0,
/*float A2_2_2 =*/ 0,
/*float A2_2_3 =*/ 0,
/*float A2_2_4 =*/ 0,
/*float A2_2_5 =*/ 0,
/*float A2_2_6 =*/ 0,
/*float A2_2_7 =*/ 0,
/*float A2_2_8 =*/ 0,
/*float A2_2_9 =*/ 0,
/*float A2_3_0 =*/ 0,
/*float A2_3_1 =*/ 0,
/*float A2_3_2 =*/ 0,
/*float A2_3_3 =*/ 0,
/*float A2_3_4 =*/ 0,
/*float A2_3_5 =*/ 0,
/*float A2_3_6 =*/ 0,
/*float A2_3_7 =*/ 0,
/*float A2_3_8 =*/ 0,
/*float A2_3_9 =*/ 0,
/*float A3_0 =*/ 0,
/*float A3_1 =*/ 0,
/*float A3_2 =*/ 0,
/*float A3_3 =*/ 0,
/*float A3_4 =*/ 0,
/*float A3_5 =*/ 0,
/*float A3_6 =*/ 0,
/*float A3_7 =*/ 0,
/*float A3_8 =*/ 0,
/*float A3_9 =*/ 0,
/*float A4_0 =*/ 0,
/*float A4_1 =*/ 0,
/*float A4_2 =*/ 0,
/*float A4_3 =*/ 0,
/*float A4_4 =*/ 0,
/*float A4_5 =*/ 0,
/*float A4_6 =*/ 0,
/*float A4_7 =*/ 0,
/*float A4_8 =*/ 0,
/*float A4_9 =*/ 0,
/*float A5_0 =*/ 0,
/*float A5_1 =*/ 0,
/*float A5_2 =*/ 0,
/*float A5_3 =*/ 0,
/*float A5_4 =*/ 0,
/*float A5_5 =*/ 0,
/*float A5_6 =*/ 0,
/*float A5_7 =*/ 0,
/*float A5_8 =*/ 0,
/*float A5_9 =*/ 0,
/*float A6_0 =*/ 0,
/*float A6_1 =*/ 0,
/*float A6_2 =*/ 0,
/*float A6_3 =*/ 0,
/*float A6_4 =*/ 0,
/*float A6_5 =*/ 0,
/*float A6_6 =*/ 0,
/*float A6_7 =*/ 0,
/*float A6_8 =*/ 0,
/*float A6_9 =*/ 0,
/*float A7_0 =*/ 0,
/*float A7_1 =*/ 0,
/*float A7_2 =*/ 0,
/*float A7_3 =*/ 0,
/*float A7_4 =*/ 0,
/*float A7_5 =*/ 0,
/*float A7_6 =*/ 0,
/*float A7_7 =*/ 0,
/*float A7_8 =*/ 0,
/*float A7_9 =*/ 0,
/*float A7_0 =*/ 0,
/*float A8_1 =*/ 0,
/*float A8_2 =*/ 0,
/*float A8_3 =*/ 0,
/*float A8_4 =*/ 0,
/*float A8_5 =*/ 0,
/*float A8_6 =*/ 0,
/*float A8_7 =*/ 0,
/*float A8_8 =*/ 0,
/*float A8_9 =*/ 0,
/*float A9_0 =*/ 0,
/*float A9_1 =*/ 0,
/*float A9_2 =*/ 0,
/*float A9_3 =*/ 0,
/*float A9_4 =*/ 0,
/*float A9_5 =*/ 0,
/*float A9_6 =*/ 0,
/*float A9_7 =*/ 0,
/*float A9_8 =*/ 0,
/*float A9_9 =*/ 0,
/*float A10_0 =*/ 0,
/*float A10_1 =*/ 0,
/*float A10_2 =*/ 0,
/*float A10_3 =*/ 0,
/*float A10_4 =*/ 0,
/*float A10_5 =*/ 0,
/*float A10_6 =*/ 0,
/*float A10_7 =*/ 0,
/*float A10_8 =*/ 0,
/*float A10_9 =*/ 0
};

struct Descriptor
{
uint8_t HW[2];	
uint8_t FW[2];	// 
uint8_t NameUSO[8];	
uint8_t SN_USO[4];	
uint8_t NameAPS[8];	
uint8_t SN_APS[4];	
uint8_t NameFPS[8];	
uint8_t SN_FPS[4];
uint8_t NameATS[8];	
uint8_t SN_ATS[4];
uint8_t NameCTS_L[8];	
uint8_t SN_CTS_L[4];
uint8_t NameCTS_R[8];	
uint8_t SN_CTS_R[4];
uint8_t NameEGTS_L[8];	
uint8_t SN_EGTS_L[4];
uint8_t NameEGTS_R[8];	
uint8_t SN_EGTS_R[4];
//uint8_t PSWL0[2];	// uint16
//uint8_t PSWL1[2];	// uint16
//uint8_t PSWL2[2];	// uint16
//uint8_t PSWL3[2];	// uint16
} __attribute__((packed)) DS;

#define DSLen sizeof(DS)	// 174 Bytes

struct CorrPolynomFactors
{
float C00;	//   -3.386E-3;
float C01;	// 	 1.124E-5;
float C02;	//   -2.221E-8;
float C03;	// 	 2.250E-10;
float C04;	// 	 1.206E-12;
float C05;	//   -5.377E-15;
float C06;	//   -3.506E-16
float C10;	//   -1.025E-4;		
float C11;	// 	 1.134E-6;
float C12;	// 	-6.393E-9;
float C13;	// 	 4.417E-11;
float C14;	// 	-8.875E-13;
float C15;	// 	-3.558E-15;
float C16;	// 	 1.625E-16;
float C20;	//    4.801E-7;		
float C21;	// 	 8.633E-9;
float C22;	// 	-5.918E-10;
float C23;	// 	 3.648E-12;
float C24;	// 	 1.236E-13;
float C25;	// 	 1.153E-15;
float C26; 	// 	-3.627E-17;
} __attribute__((packed)) CPF =
{
/* float C00;	*/   -3.386E-3,
/* float C01;	*/ 	 1.124E-5,
/* float C02;	*/   -2.221E-8,
/* float C03;	*/ 	 2.250E-10,
/* float C04;	*/ 	 1.206E-12,
/* float C05;	*/   -5.377E-15,
/* float C06;	*/   -3.506E-16,
/* float C10;	*/   -1.025E-4,	
/* float C11;	*/ 	 1.134E-6,
/* float C12;	*/ 	-6.393E-9,
/* float C13;	*/ 	 4.417E-11,
/* float C14;	*/ 	-8.875E-13,
/* float C15;	*/ 	-3.558E-15,
/* float C16;	*/ 	 1.625E-16,
/* float C20;	*/   4.801E-7,		
/* float C21;	*/ 	 8.633E-9,
/* float C22;	*/ 	-5.918E-10,
/* float C23;	*/ 	 3.648E-12,
/* float C24;	*/ 	 1.236E-13,
/* float C25;	*/ 	 1.153E-15,
/* float C26 	*/ 	-3.627E-17	
};

