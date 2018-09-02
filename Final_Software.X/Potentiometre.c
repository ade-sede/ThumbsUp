#include "header.h"

void    init_pot() {
	TRISBbits.TRISB15 = 1;//ADC AN9 pot IN -> RB15
	AD1CON1bits.FORM = 0;// select format Integer 16-bit
	AD1CON1bits.SSRC = 0;//Clearing SAMP bit ends sampling and starts conversion
	AD1CON2bits.VCFG = 0;//AVDD AVSS
	AD1CON2bits.BUFM = 0;// buff fill mode one big buff
	/* Buffer configured as one 16-word buffer ADC1BUFF-ADC1BUF0 */
	AD1CON2bits.ALTS = 0; // Always MUX Sample A
	AD1CHSbits.CH0SA = 9;// select MUX A sur AN9
	AD1CON3bits.ADRC = 1;// Clock and prescale 1 -> ADC internal RC clock FRC
	AD1CON1bits.ON = 1;//ACD ON
	AD1CON1bits.ASAM = 0; //sampling begins when SAMP bit is set
	AD1CON1bits.SSRC = 0b0000; //clearing SAMP ends sampling and starts conversion
}

u16    pot_report() {
    u16 data;
    u16 i = 0;
	
    data = 0;
    AD1CON1bits.DONE = 0; //conversion not started or in progress
    AD1CON1bits.SAMP = 1; //Echantillonage
    while (i < 10000)
        i++;
    AD1CON1bits.SAMP = 0; //Conversion
    while(AD1CON1bits.DONE == 0)
        Nop();
    data = ADC1BUF0; //value on 16bits
	return (data);
}
