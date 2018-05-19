
/*
 * File:   main.c
 * Author: amelielallemand
 *
 * Created on April 30, 2018, 5:31 PM
 */

#include <p32xxxx.h>
#include <xc.h>
#include <sys/attribs.h>
#include "header.h"

#define LED_OFF LATFbits.LATF1 = 0
#define LED_ON LATFbits.LATF1 = 1
#define LED_TOGGLE LATFbits.LATF1 ^= 1

u8 volatile g_data_buffer[2];
u8 volatile *g_data;

/** AN3 - RB3 **/

/** Step doc
	AD1PCFG<15:0>;// 0 -> analog ; 1 -> digital
	AD1CHS<32:0>;// select MUX
	FORM<2:0> (AD1CON1<10:8>);// select format
	SSRC<2:0> (AD1CON1<7:5>);// select sample clock source
	VCFG<2:0> (AD1CON2<15:13>);// select voltage
	CSCNA (AD1CON2<10>);// select scan mode
	SMP<3:0> (AD1CON2<5:2>);// number conv / int
	BUFM (AD1CON2<1>); buff fill mode
	BUFS (AD1CON2<1>); 1 -> current fill 0 -> current fill work if BUFM = 1
	ALTS AD1CON2<0>;// MUX
	ADRC (AD1CON3<15>);// Clock and prescale
	SAMC<4:0> (AD1CON3<12:8>);// sample
	ADCS<7:0> (AD1CON3<7:0>);// clock source and prescale
	AD1CON1<15>
*/

int		main(){

	TRISFbits.TRISF1 = 0;
	LED_OFF;

	TRISBbits.TRISB3 = 1;//ADC AN3 pot IN

	// ** ADC CONF
//	AD1PCFG:ADC;
//	AD1CHS0bits.CH0SB = 0b0011;;// select MUX B sur AN3
	AD1CON1bits.FORM = 0;// select format INT16
	AD1CON1bits.SSRC = 0;//Clearing SAMP bit ends sampling and starts conversion
	AD1CON2bits.VCFG = 0;//AVDD AVSS
//	AD1CON2.CSCNA -> 0 no scan input ; 1 -> scan input for MUX A
//	SMP<3:0> (AD1CON2<5:2>);// number conv / int
	AD1CON2bits.BUFM = 0;// buff fill mode one big buff
	AD1CON2bits.ALTS = 0; // Always MUX A
	AD1CHSbits.CH0SA = 0b0011;;// select MUX A sur AN3
	AD1CON3bits.ADRC = 1;// Clock and prescale 1 -> ADC internal RC clock
//	SAMC<4:0> (AD1CON3<12:8>);// sample
//	AD1CON3.ADCS = ;//clock source and prescale 00000000 =TPB • 2 • (ADCS<7:0> + 1) = 2 • TPB = TAD ?
	AD1CON1bits.ON = 1;//ACD ON
	AD1CON1bits.ASAM = 0; //sampling begins when SAMP bit is set
	AD1CON1bits.SSRC = 0b000; //clearing SAMP ends sampling and starts conversion

	while (1) {
	    AD1CON1bits.DONE = 0; //conversion not started or in progress
            AD1CON1bits.SAMP = 1; //Echantillonage
	    AD1CON1bits.SAMP = 0; //Conversion
	    while(AD1CON1bits.DONE == 0) ;
	    g_data = ADC1BUF0;//value on 16bits
	}
}
