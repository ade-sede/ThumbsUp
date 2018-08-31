#include "header.h"

void    init_pot() {
	TRISBbits.TRISB15 = 1;//ADC AN9 pot IN -> RB15
	AD1CON1bits.FORM = 0;// select format INT16
	AD1CON1bits.SSRC = 0;//Clearing SAMP bit ends sampling and starts conversion
	AD1CON2bits.VCFG = 0;//AVDD AVSS
	AD1CON2bits.BUFM = 0;// buff fill mode one big buff
	AD1CON2bits.ALTS = 0; // Always MUX A
	AD1CHSbits.CH0SA = 0b0011;;// select MUX A sur AN3
	AD1CON3bits.ADRC = 1;// Clock and prescale 1 -> ADC internal RC clock
	AD1CON1bits.ON = 1;//ACD ON
	AD1CON1bits.ASAM = 0; //sampling begins when SAMP bit is set
	AD1CON1bits.SSRC = 0b000; //clearing SAMP ends sampling and starts conversion
}

u16    pot_report() {
    u16 data;

    data = 0;
    AD1CON1bits.DONE = 0; //conversion not started or in progress
    AD1CON1bits.SAMP = 1; //Echantillonage
    AD1CON1bits.SAMP = 0; //Conversion
    while(AD1CON1bits.DONE == 0)
        Nop();
    data = ADC1BUF0;//value on 16bits
    return (data);
}