
//#define FINP									/*  Frequency input  */	// has to be commented for Analogue inputs
//#define IWDG									/*   Watch Dog  */	
#define UART2

#define DELAY_LOOP_CYCLES               (9UL)
//#define GET_US_LOOPS(N)                 ((uint32_t)(((float)(N) * FlashFreqMHz )/ DELAY_LOOP_CYCLES))
//#define GET_US_LOOPS(N)                 ((uint32_t)(((float)(N) * FlashFreqFa )/ DELAY_LOOP_CYCLES))	

#define LOOP_US 												(FLASH_PROG_FREQ_MHZ / DELAY_LOOP_CYCLES)
#define GET_US_LOOPS(N)                 ((uint32_t)((float)(N) * LOOP_US))
	
//#define LOOP_US 												(1.0 / DELAY_LOOP_CYCLES)
//#define GET_US_LOOPS(N)                 ((uint32_t)((float)(N) * LOOP_US))

#define TMT          	1000    // The Measurement Time (ms)
#define FTNum 					10
#define F1Num 					20
#define F2Num 					20
#define ATNum 					50
#define A1Num 					50
#define UrefAP       1250.0	
#define Uref				3290.0F//1230.0F//3290.0F//3235.0F

#define Ninp						6
#define NinpAP					6
#define SampleNum				16	
#define SamplePeriod			4

#define PeriodTx1				20
#define PeriodTx2				20
#define PeriodTx3				20
#define PeriodTx4				20
#define PeriodTx5			  20
#define PeriodTx6			  100

#define Tx1	1
#define Tx2	2
#define Tx3	4
#define Tx4	8
#define Tx5	16
#define Tx6	32

#define 		sof				0x01
#define 		eof				0x02
#define 		eof1			0x04
#define 		com				0x08
#define 		chs				0x10
#define 		str				0x100
#define 		SOF				0x3E

enum regInputBufAddr {
	RawP1_L,
	RawP1_H,
	RawP2_L,
	RawP2_H,
	RawTP1_L,
	RawTP1_H,
	RawTP2_L,
	RawTP2_H,
	RawRT1_L,
	RawRT1_H,
	RawRT2_L,
	RawRT2_H,	
	RawRT3_L,
	RawRT3_H,
	RawRT4_L,
	RawRT4_H,	
	RawRT5_L,
	RawRT5_H,
	RawRT6_L,
	RawRT6_H,	
	RawRT7_L,
	RawRT7_H,
	RawRT8_L,
	RawRT8_H,	
	TEMP_L,
	TEMP_H,
	FP_L,
	FP_H,
	AP_L,
	AP_H,
	LCT_L,
	LCT_H,
	RCT_L,
	RCT_H,
	LEGT_L,
	LEGT_H,
	REGT_L,
	REGT_H,
	AT_L,
	AT_H,
	SW_L,				// status word
	SW_H,	
	ProcCntFP_L,
	ProcCntFP_H,
	FailCntFP_L,
	FailCntFP_H,		
	ProcCntAP_L,
	ProcCntAP_H,
	FailCntAP_L,
	FailCntAP_H,
	ProcCntRT1_L,
	ProcCntRT1_H,
	FailCntRT1_L,
	FailCntRT1_H,		
	ProcCntRT2_L,
	ProcCntRT2_H,
	FailCntRT2_L,
	FailCntRT2_H,
	ProcCntRT3_L,
	ProcCntRT3_H,
	FailCntRT3_L,
	FailCntRT3_H,
	ProcCntRT4_L,
	ProcCntRT4_H,
	FailCntRT4_L,
	FailCntRT4_H,	
	ProcCntRT5_L,
	ProcCntRT5_H,
	FailCntRT5_L,
	FailCntRT5_H,	
	ProcCntRT6_L,
	ProcCntRT6_H,
	FailCntRT6_L,
	FailCntRT6_H,	
	ProcCntRT7_L,
	ProcCntRT7_H,
	FailCntRT7_L,
	FailCntRT7_H,	
	ProcCntRT8_L,
	ProcCntRT8_H,
	FailCntRT8_L,
	FailCntRT8_H	
};

enum regHoldingBufAddr {
	MAX_IT,
	P1_TIME,
	P2_TIME,	
	RT1_TIME,
	RT2_TIME,	
	P1_0_L = 7,
	P1_0_H,
	P2_0_L,
	P2_0_H,	
	EP1_L,
	EP1_H,
	EP2_L,
	EP2_H,
	BAUDRATE,
	MB_ADDR,
	TP1_0_L,
	TP1_0_H,
	TP2_0_L,
	TP2_0_H,	
	A1_0_0_L,
	A1_0_0_H,
	A2_0_0_L = 101,	
	A2_3_9_L = 179,
	A2_3_9_H,
	A3_0_L = 181,
	A4_0_L = 201,
	A5_0_L = 221,
	A6_0_L = 241,
	A7_0_L = 261,
	A7_9_H = 280,	
  PGA1,	
	PGA2,
	PGA3,
	PGA4,
	PGA5,
	PGA6,
	END_ADDR, // 287 ///////////////////////////// 
	
	BAUDRATE_RS485 = 198,
	ADDR_RS485,
	HW = 300,
	FW,
	Name_USO,
	SN_USO = Name_USO +4,
	Name_APS = SN_USO +2,
	SN_APS = Name_APS +4,
	Name_FPS = SN_APS +2,
	SN_FPS = Name_FPS +4,	
	Name_ATS = SN_FPS +2,
	SN_ATS = Name_ATS +4,		
	Name_CTS_L = SN_ATS +2,
	SN_CTS_L = Name_CTS_L +4,		
	Name_CTS_R = SN_CTS_L +2,
	SN_CTS_R = Name_CTS_R +4,	
	Name_EGTS_L = SN_CTS_R +2,
	SN_EGTS_L = Name_EGTS_L +4,		
	Name_EGTS_R = SN_EGTS_L +2,
	SN_EGTS_R = Name_EGTS_R +4,		
	END_ADDR_1 = SN_EGTS_R +2, // 350 //////////////////////////////
	
	A8_0_L = 353,
	A9_0_L = 373,
	A10_0_L = 393,	
	A10_9_H = 412,
	RT1_0_L,
	RT1_0_H,
	RT2_0_L,
	RT2_0_H,
	RT3_0_L,
	RT3_0_H,	
	RT8_0_L = 428,
	ETR1_L,
	ETR1_H,
	ETR2_L,
	ETR2_H,
	ERT1_L,
	ERT8_H =444,
	P1_MAX_H,	
	P1_MIN_L,
	P1_MIN_H,	
	P2_MAX_L,
	P2_MAX_H,	
	P2_MIN_L,
	P2_MIN_H,	
	TP1_MAX_L,
	P1_MAX_L,	
	TP1MAX_H,	
	TP1_MIN_L,
	TP1_MIN_H,	
	TP2_MAX_L,
	TP2MAX_H,	
	TP2_MIN_L,
	TP2_MIN_H,	
	RT1_MAX_L,	
	RT1_MAX_H,	
	RT1_MIN_L,
	RT1_MIN_H,	
	RT2_MAX_L,
	RT2_MAX_H,	
	RT2_MIN_L,
	RT2_MIN_H,
	RT3_MAX_L,	
	RT3_MAX_H,	
	RT3_MIN_L,
	RT3_MIN_H,	
	RT4_MAX_L,
	RT4_MAX_H,	
	RT4_MIN_L,
	RT4_MIN_H,	
	RT5_MAX_L,	
	RT5_MAX_H,	
	RT5_MIN_L,
	RT5_MIN_H,	
	RT6_MAX_L,
	RT6_MAX_H,	
	RT6_MIN_L,
	RT6_MIN_H,
	RT7_MAX_L,	
	RT7_MAX_H,	
	RT7_MIN_L,
	RT7_MIN_H,	
	RT8_MAX_L,
	RT8_MAX_H,	
	RT8_MIN_L,
	RT8_MIN_H,

	CRC16 =511
};

/* InpStatus */
//#define F1OK 		0x0001
//#define F1fail 	0x0002
//#define F2OK 		0x0004
//#define F2fail 	0x0008
//#define FTOK 		0x0010
//#define FTfail 	0x0020
//#define FTReady	0x0040
//#define MBPoll	0x0080
//#define RFI     0x0100   //
//#define ADC_EOC	0x0800
//#define MBSync	0x1000
//#define t1			0x2000
//#define t2			0x4000
//#define ADCinit 0x8000
/* ComReg */
#define ExeSTR  0x0200
#define PEN     0x1000

#define P1OK 		0x0000001
#define P2OK  	0x0000002
#define TP1OK		0x0000004
#define TP2OK 	0x0000008
#define RT1OK 	0x0000010
#define RT2OK 	0x0000020
#define RT3OK 	0x0000040
#define RT4OK 	0x0000080
#define RT5OK 	0x0000100
#define RT6OK 	0x0000200
#define RT7OK 	0x0000400
#define RT8OK 	0x0000800
#define MBPoll	0x0008000
#define RFI     0x0010000   //
#define ADC_EOC	0x0080000
#define MBSync	0x0100000
#define t1			0x0200000
#define t2			0x0400000
#define ADCinit 0x0800000

