#include "header.h"
#include "RN42.h"


void __ISR (_TIMER_2_VECTOR, IPL6SRS) led_blink (void){
	T2CONbits.ON = 0;
	TMR2 = 0;
	LATFbits.LATF1 = 0;
	IFS0bits.T2IF = 0;
}
void __ISR (_EXTERNAL_2_VECTOR, IPL7SRS) left_click (void){
	//IEC0bits.INT2IE = 0;
	T2CONbits.ON = 0;
	LATFbits.LATF1 = 1;
	send_report(create_report(1, 0, 0));
	TMR2 = 0;
	T2CONbits.ON = 1;
	IFS0bits.INT2IF = 0;
}

void __ISR (_EXTERNAL_3_VECTOR, IPL7SRS) right_click (void){
	//IEC0bits.INT3IE = 0;
	T2CONbits.ON = 0;
	LATFbits.LATF1 = 1;
	send_report(create_report(2, 0, 0));
	TMR2 = 0;
	T2CONbits.ON = 1;
	IFS0bits.INT3IF = 0;
}

void set_external_interrupt() {

	// Interrupt Button left click
	//IEC0CLR = 0x00008000; // disable INT3
	//INTCONCLR = 0x00000008; // clear the bit for falling edge trigger
	//IFS0CLR = 0x00008000; // clear the interrupt flag
	//IEC0SET = 0x00008000; // enab;e INT3

	INTCONbits.INT2EP = 0;// Falling edge 1-> Rising egde
	IPC2bits.INT2IP = 7;
	IPC2bits.INT2IS = 0;
	IFS0bits.INT2IF = 0;
	IEC0bits.INT2IE = 1;

	// Interrupt Button right click
	INTCONbits.INT3EP = 0;
	IPC3bits.INT3IP = 7;
	IPC3bits.INT3IS = 0;
	IFS0bits.INT3IF = 0;
	IEC0bits.INT3IE = 1;

}

void set_timer() {
	T2CON = 0;//reset
	T2CONbits.TCKPS = 0b110; //1:64
	TMR2 = 0;//set timer 0
	PR2 = 62500;
	IFS0bits.T2IF = 0;
	IPC2bits.T2IP = 6;
	IEC0bits.T2IE = 1;
}

void __ISR (_EXTERNAL_1_VECTOR, IPL6SRS) test_click (void){
	//T2CONbits.ON = 0;
	//IFS0bits.INT1IF = 0;
	LATFbits.LATF1 ^= 1;
	send_report(create_report(2, 0, 0));
	send_report(create_report(0, 0, 0));
	IFS0bits.INT1IF = 0;
	//TMR2 = 0;
	//T2CONbits.ON = 1;
}

void set_interrupt() { //final function
//  Test interrupt with board button uncomment this line and comment set_external & set_timer
//    	INTCONbits.INT1EP = 0;
//	IPC1bits.INT1IP = 6;
//	IFS0bits.INT1IF = 0;
//	IEC0bits.INT1IE = 1;
	set_external_interrupt();
	set_timer();

	INTCONbits.MVEC = 1;
	__builtin_enable_interrupts();
}
