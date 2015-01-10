#include<msp430g2452.h>
#include<stdbool.h>

#define		TXD		BIT1

#define		Bit_time	104
#define		Bit_time_5	52

#define		M		0x4D

unsigned char BitCnt;
unsigned int TXByte;

bool isClicked; //butona basýldýðýný kontrol eden flag
unsigned int txValue = 0;
unsigned int num = 0;

void Transmit(void);

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;

	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P1SEL |= TXD;
	P1DIR |= TXD;
        P1DIR &= ~( BIT3);
	P1IES = ( BIT3 );
	P1IFG &= ~( BIT3 );
	P1IE |= ( BIT3 );

	P1DIR |= BIT0;
	P1OUT &= ~BIT0;

	isClicked = false;
	__bis_SR_register(GIE);

	while(1)
	{
		if(isClicked)
		{
			isClicked = false;
			TXByte = txValue; //Karakteri TXByte a atadýk
			Transmit();
			P1IE |= ( BIT3 );
		}
	}
}

//TXByte da tutulan deðeri göndermek için kullanýlan fonksiyon
void Transmit()
{
	TXByte |= 0x100;
	TXByte = TXByte << 1;
	BitCnt = 0xA;

	CCTL0 = OUT;
	TACTL = TASSEL_2 + MC_2;
	CCR0 = TAR;
	CCR0 += Bit_time;
	CCTL0 =  CCIS0 + OUTMOD0 + CCIE;
	while ( CCTL0 & CCIE );
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	isClicked = true;
	P1IE &= ~(BIT3 );
	P1IFG &= ~(BIT3 );
	if((P1IN & BIT3) == 0){
		txValue = M ; //txValue ye deðeri atandý
}
	__bic_SR_register_on_exit(CPUOFF);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
		CCR0 += Bit_time;
		if ( BitCnt == 0)
		{
			TACTL = TASSEL_2;
			CCTL0 &= ~ CCIE ;
		}
		else
		{
			CCTL0 |=  OUTMOD2;
			if (TXByte & 0x01)
			CCTL0 &= ~ OUTMOD2;
			TXByte = TXByte >> 1;
			BitCnt --;
		}
}
