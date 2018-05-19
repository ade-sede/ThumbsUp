
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

int		main(){

	TRISFbits.TRISF1 = 0;
	LED_OFF;

	TRISBbits.TRISB3 = 1;//ADC AN3 pot IN
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

	while (1) {
	    AD1CON1bits.DONE = 0; //conversion not started or in progress
            AD1CON1bits.SAMP = 1; //Echantillonage
	    AD1CON1bits.SAMP = 0; //Conversion
	    while(AD1CON1bits.DONE == 0) ;
	    g_data = ADC1BUF0;//value on 16bits
	}
}
