#include "header.h"
#include "RN42.h"

void __ISR (_EXTERNAL_2_VECTOR, IPL6SRS) left_click (void){
	IFS0bits.INT2IF = 0;
	LATFbits.LATF1 ^= 1;
	send_report(create_report(1, 0, 0));
}

void __ISR (_EXTERNAL_3_VECTOR, IPL6SRS) right_click (void){
	IFS0bits.INT2IF = 0;
	LATFbits.LATF1 ^= 1;
	send_report(create_report(2, 0, 0));
}

void set_timer_interrupt() {
	TRISDbits.TRISD8 = 1;	/* Settimg up tri-state*/

	// Interrupt Button left click
	INT2CONbits.INT2EP = 0;
	IPC1bits.INT2IP = 6;
	IFS0bits.INT2IF = 1;
	IEC0bits.INT2IE = 1;

	// Interrupt Button right click
	INT3CONbits.INT3EP = 0;
	IPC1bits.INT3IP = 6;
	IFS0bits.INT3IF = 0;
	IEC0bits.INT3IE = 1;

	INTCONTbits.MVEC = 1;
	__builtin_enable_interrupts();

	T2CONbits.ON = 1;
}
