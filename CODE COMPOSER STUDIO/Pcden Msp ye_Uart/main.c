#include <msp430g2553.h>

#include<stdbool.h>

#define RXD    BIT2

#define		Bit_time	104  // 9600 Baudrate
#define		Bit_time_5	52
// göndereceðim karakterlerin ASCII karþýlýklarý
#define		M		0x4D
#define		S		0x53
#define		P		0x50
unsigned char BitCnt;
unsigned int RXByte;	//gelen karakterlerin ASCII karþýlýklarý bu deðiþkende saklanýyor

unsigned int i;

bool isReceiving; //BIT2 deki herhangi bir deðiþiklikte kontrol edilen flag
bool hasReceived; //RXByte da gönderilecek deðer oluþtuðunu kontrol eden flag

void Receive(void);


void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; //Watchdog timer kapalý

//kristal ayarý
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P1IES|=RXD;
	P1IFG&=~RXD;
	P1IE|=RXD;

	isReceiving = false; //flag sýfýrlandý
	hasReceived = false; //flag sýfýrlandý

	__bis_SR_register(GIE); //kesmeler aktif

	 while(1)
		{
			if (hasReceived) //RXByte da gönderilecek deðer oluþtuðunda Receive fonksiyonunu çaðýrýyor
			{
				Receive();
			}
			if (~(hasReceived))
				__bis_SR_register(CPUOFF + GIE);
		}
	}

void Receive()
{
	hasReceived = false; //kontrol flag ý temizlendi
//Gelen veriyi deðerlendirdiðim bölüm
	switch(RXByte)
	{
	case M:
		P1DIR |= BIT6;
		break;
	case S:
		P1DIR |= BIT0;
		break;
	case P:
		P1DIR &= ~(BIT0 + BIT4 + BIT6);
		break;
	default:;
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	isReceiving = true; //BIT2 de deðiþiklik olduðu için kesmeye girdi ve flag ý true yaptý.(Flag timer kesmesinde deðerlendirilecek)
	P1IE &= ~RXD;	// kesme kapatýldý
	P1IFG &= ~RXD;	// kesme flagý sýfýrlandý
	TACTL = TASSEL_2 + MC_2; // TimerA kontrolü = timer a clock seçimi(SMCLK) + timer a mode kontrolü (continous up)
	CCR0 = TAR; //timerA capture/compare = Timer A
	CCR0 += Bit_time_5; //
	CCTL0 = OUTMOD1 + CCIE;	// timerA0 capture/compare kontrol = output modu 1 + captur/compare kesmesi aktif
	RXByte = 0; //RXByte deðiþkeni temizlendi
	BitCnt = 0x9;
}

//Aþaðýdaki kesme fonksiyonunda RXByte a gönderilen deðer yükleniyor
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	if(isReceiving)
	{
		CCR0 += Bit_time;
		if ( BitCnt == 0)
		{
			TACTL = TASSEL_2;
			CCTL0 &= ~ CCIE ;
			isReceiving = false;//kontrol flag ý sýfýrlandý
                        P1IFG &= ~RXD;
			P1IE |= RXD;
			if ( (RXByte & 0x201) == 0x200)
			{
				RXByte = RXByte >> 1;
				RXByte &= 0xFF;
				hasReceived = true; //RXByte da gönderilecek deðerin bulunduðunu tespit ediyor
                        }
			__bic_SR_register_on_exit(CPUOFF);
		}
		else
		{
			if ( (P1IN & RXD) == RXD)
			RXByte |= 0x400;
			RXByte = RXByte >> 1;
			BitCnt --;
		}
	}
}
