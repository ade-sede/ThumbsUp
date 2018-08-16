#include "header.h"
#include "RN42.h"

#define FALLING 0
#define RISING 1

g_egde_int2 = RISING;
g_egde_int3 = RISING;

void __ISR (_TIMER_2_VECTOR, IPL7SRS) int2_debounce (void){
	// Relaunch interrupt2 after left click debounce
	T2CONbits.ON = 0;
	IFS0bits.T2IF = 0;
	TMR2 = 0;
	LATFbits.LATF1 = 0;
	set_interrupt_left_click();
}

void __ISR (_TIMER_3_VECTOR, IPL7SRS) int3_debounce (void){
	// Relaunch interrupt3 after right click debounce
	T3CONbits.ON = 0;
	IFS0bits.T3IF = 0;
	TMR3 = 0;
	LATFbits.LATF1 = 0;
	set_interrupt_right_click();
}

void __ISR (_EXTERNAL_2_VECTOR, IPL6SRS) left_click (void){
	IEC0bits.INT2IE = 0;
	LATFbits.LATF1 = 1;
	if (g_edge_int2 == FALLING)
		send_report(create_report(1, 0, 0));
	T2CONbits.ON = 1;
}

void __ISR (_EXTERNAL_3_VECTOR, IPL6SRS) right_click (void){
	IEC0bits.INT3IE = 0;
	LATFbits.LATF1 = 1;
	if (g_edge_int3 == FALLING)
		send_report(create_report(2, 0, 0));
	T3CONbits.ON = 1;
}

void set_interrupt_left_click() {
	// Interrupt Button left click
	if (g_edge_int2 == FALLING)
		g_edge_int2 = RISING;
	else if (g_edge_int2 == RISING)
		g_edge_int2 = FALLING;
	INTCONbits.INT2EP = g_edge_int2;

//	INTCONbits.INT2EP ^= 1;
	IPC2bits.INT2IP = 6;
	IPC2bits.INT2IS = 0;
	IFS0bits.INT2IF = 0;
	IEC0bits.INT2IE = 1;
}

void set_interrupt_right_click() {
	// Interrupt Button right click
	if (g_edge_int3 == FALLING)
		g_edge_int3 = RISING;
	else if (g_edge_int3 == RISING)
		g_edge_int3 = FALLING;
	INTCONbits.INT3EP = g_edge_int3;

//	INTCONbits.INT3EP ^= 1;
	IPC3bits.INT3IP = 6;
	IPC3bits.INT3IS = 0;
	IFS0bits.INT3IF = 0;
	IEC0bits.INT3IE = 1;
}

void set_external_interrupt() {
	set_interrupt_right_click();
	set_interrupt_left_click();
}

void set_timer() {
	// Timer 2 link to int2
	T2CON = 0;//reset
	T2CONbits.TCKPS = 0b110; //1:64
	TMR2 = 0;//set timer 0
	PR2 = 1250;//2ms 625000 -> 1s
	IPC2bits.T2IP = 7;
	IFS0bits.T2IF = 0;
	IEC0bits.T2IE = 1;

	// Timer 3 link to int3
	T3CON = 0;//reset
	T3CONbits.TCKPS = 0b110; //1:64
	TMR3 = 0;//set timer 0
	PR3 = 1250;//2ms 625000 -> 1s
	IPC3bits.T3IP = 7;
	IFS0bits.T3IF = 0;
	IEC0bits.T3IE = 1;
}

void __ISR (_EXTERNAL_1_VECTOR, IPL6SRS) test_click (void){
	//T2CONbits.ON = 0;
	//IFS0bits.INT1IF = 0;
	LATFbits.LATF1 ^= 1;
	send_report(create_report(2, 0, 0));
	IFS0bits.INT1IF = 0;
	//TMR2 = 0;
	//T2CONbits.ON = 1;
}

void set_interrupt() { //final function
//  Test interrupt with board button uncomment this line and comment set_external & set_timer
//	INTCONbits.INT1EP = 0;
//	IPC1bits.INT1IP = 6;
//	IFS0bits.INT1IF = 0;
//	IEC0bits.INT1IE = 1;
	set_external_interrupt();
	set_timer();

	INTCONbits.MVEC = 1;
	__builtin_enable_interrupts();
}
