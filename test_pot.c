/** AN3 - RB3 **/

/** Step doc 
 *

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

	TRISBbits.TRIS3 = 1;//ADC AN3 pot IN

	// ** ADC CONF
//	AD1PCFG:ADC;
//	AD1CHS0bits.CH0SB = 0b0011;;// select MUX B sur AN3
	AD1CON1.FORM = 0;// select format INT16
	AD1CON1.SSRC = 0;//Clearing SAMP bit ends sampling and starts conversion
	AD1CON2.VCFG = 0;//AVDD AVSS
//	AD1CON2.CSCNA -> 0 no scan input ; 1 -> scan input for MUX A
//	SMP<3:0> (AD1CON2<5:2>);// number conv / int
	ADC1CON2.BUFM = 0;// buff fill mode one big buff
	AD1CON2bits.ALTS = 0; // Always MUX A
	AD1CHS0bits.CH0SA = 0b0011;;// select MUX A sur AN3
	AD1CON3.ADRC = 1;// Clock and prescale 1 -> ADC internal RC clock
//	SAMC<4:0> (AD1CON3<12:8>);// sample
//	AD1CON3.ADCS = ;//clock source and prescale 00000000 =TPB • 2 • (ADCS<7:0> + 1) = 2 • TPB = TAD ?
	ACD1CON1.ON = 1;//ACD ON


	while (1);
}

/*
void port_init (void)
{
    TRISEbits.TRISE13 = 1; //potentiomètre en entrée
}

void adc_init(void)
{
    //POT sur AN13/RE13
    //DIODE sur RD6
    AD1CHS0bits.CH0SB = 0b01101; //AN13 en analogue sur MUXB
    AD1CON1bits.ADON = 1; //ADC opérationnel
    AD1CON1bits.SSRCG = 0; //implication sur SSRC
    AD1CON1bits.ASAM = 0; //sampling begins when SAMP bit is set
    AD1CON1bits.SSRC = 0b000; //clearing SAMP ends sampling and starts conversion
    AD1CON4bits.ADDMAEN = 0; //result stored in ADC1BUF0 par les registreS ADC1BUFF, on utilise pas le DMA
}

int16_t main(void)
{
    ConfigureOscillator();
    InitApp();


    //POTENTIOMETRE
    int i=0;
    int analog_reading;

    port_init();
    adc_init();

    while(1)
    {
        AD1CON1bits.DONE = 0; //conversion not started or in progress (automatically set when conversion complete)
        __delay_ms(1);
        AD1CON1bits.SAMP = 1; //Echantillonage
        __delay_ms(10);
        AD1CON1bits.SAMP = 0; //Conversion

        while(AD1CON1bits.DONE == 0) //tant que la conversion n'est pas terminée
        {
            i++; //on attend, conversion en cours
        }
        analog_reading = ADC1BUF0;//on récupère la valeur dans le buffer et on la met dans analog_reading, résultat sur 16 bits
    }
}*/
