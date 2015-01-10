#include <msp430g2553.h>

#include<stdbool.h>

#define RXD    BIT2

#define		Bit_time	104  // 9600 Baudrate
#define		Bit_time_5	52
// g�nderece�im karakterlerin ASCII kar��l�klar�
#define		M		0x4D
#define		S		0x53
#define		P		0x50
unsigned char BitCnt;
unsigned int RXByte;	//gelen karakterlerin ASCII kar��l�klar� bu de�i�kende saklan�yor

unsigned int i;

bool isReceiving; //BIT2 deki herhangi bir de�i�iklikte kontrol edilen flag
bool hasReceived; //RXByte da g�nderilecek de�er olu�tu�unu kontrol eden flag

void Receive(void);


void main(void)
{
	WDTCTL = WDTPW + WDTHOLD; //Watchdog timer kapal�

//kristal ayar�
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P1IES|=RXD;
	P1IFG&=~RXD;
	P1IE|=RXD;

	isReceiving = false; //flag s�f�rland�
	hasReceived = false; //flag s�f�rland�

	__bis_SR_register(GIE); //kesmeler aktif

	 while(1)
		{
			if (hasReceived) //RXByte da g�nderilecek de�er olu�tu�unda Receive fonksiyonunu �a��r�yor
			{
				Receive();
			}
			if (~(hasReceived))
				__bis_SR_register(CPUOFF + GIE);
		}
	}

void Receive()
{
	hasReceived = false; //kontrol flag � temizlendi
//Gelen veriyi de�erlendirdi�im b�l�m
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
	isReceiving = true; //BIT2 de de�i�iklik oldu�u i�in kesmeye girdi ve flag � true yapt�.(Flag timer kesmesinde de�erlendirilecek)
	P1IE &= ~RXD;	// kesme kapat�ld�
	P1IFG &= ~RXD;	// kesme flag� s�f�rland�
	TACTL = TASSEL_2 + MC_2; // TimerA kontrol� = timer a clock se�imi(SMCLK) + timer a mode kontrol� (continous up)
	CCR0 = TAR; //timerA capture/compare = Timer A
	CCR0 += Bit_time_5; //
	CCTL0 = OUTMOD1 + CCIE;	// timerA0 capture/compare kontrol = output modu 1 + captur/compare kesmesi aktif
	RXByte = 0; //RXByte de�i�keni temizlendi
	BitCnt = 0x9;
}

//A�a��daki kesme fonksiyonunda RXByte a g�nderilen de�er y�kleniyor
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
			isReceiving = false;//kontrol flag � s�f�rland�
                        P1IFG &= ~RXD;
			P1IE |= RXD;
			if ( (RXByte & 0x201) == 0x200)
			{
				RXByte = RXByte >> 1;
				RXByte &= 0xFF;
				hasReceived = true; //RXByte da g�nderilecek de�erin bulundu�unu tespit ediyor
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
