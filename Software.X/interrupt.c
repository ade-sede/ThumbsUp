#include "header.h"
#include "RN42.h"

void __ISR (_EXTERNAL_2_VECTOR, IPL6SRS) left_click (void){
	IFS0bits.INT2IF = 0;
	//LATFbits.LATF1 = 1;
	send_report(create_report(1, 0, 0));
	//LATFbits.LATF1 = 0;
}

void __ISR (_EXTERNAL_3_VECTOR, IPL6SRS) right_click (void){
	IFS0bits.INT3IF = 0;
	//LATFbits.LATF1 ^= 1;
	send_report(create_report(2, 0, 0));
	//LATFbits.LATF1 = 0;
}

void __ISR (_TIMER_2_VECTOR, IPL7SRS) led_blink (void){
	//LATFbits.LATF1 ^= 1;
	IFS0bits.T2IF = 0;
}

void set_external_interrupt() {
    	TRISDbits.TRISD9 = 1;	/* Setting up tri-state int2 */
	TRISDbits.TRISD10 = 1;	/* Setting up tri-state int3*/
        
	// Interrupt Button left click
	INTCONbits.INT2EP = 0;
	IPC2bits.INT2IP = 6;
	IFS0bits.INT2IF = 1;
	IEC0bits.INT2IE = 1;

	// Interrupt Button right click
	INTCONbits.INT3EP = 0;
	IPC3bits.INT3IP = 6;
	IFS0bits.INT3IF = 0;
	IEC0bits.INT3IE = 1;

}

void set_timer() {
	T2CON = 0;//reset
	T2CONbits.TCKPS = 0b110; //1:64
	TMR2 = 0;//set timer 0
	PR2 = 6250;
	T2CONbits.ON = 1;
}

void set_interrupt() {
	set_external_interrupt();
	set_timer();

	INTCONbits.MVEC = 1;
	__builtin_enable_interrupts();
}

