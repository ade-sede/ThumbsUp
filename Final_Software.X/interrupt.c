#include "header.h"
#include "RN42.h"
#include "interrupt.h"

extern u8 g_button;
u8 g_edge_int1 = RISING;
u8 g_edge_int2 = RISING;

void __ISR (_TIMER_2_VECTOR, IPL6SOFT) int2_debounce (void){
	// Relaunch interrupt2 after left click debounce
	T2CONbits.ON = 0;
	IFS0bits.T2IF = 0;
	TMR2 = 0;
	LATBbits.LATB14 = 0;
	set_interrupt_left_click();
}

void __ISR (_TIMER_3_VECTOR, IPL6SOFT) int3_debounce (void){
	// Relaunch interrupt3 after right click debounce
	T3CONbits.ON = 0;
	IFS0bits.T3IF = 0;
	TMR3 = 0;
	LATBbits.LATB14 = 0;
	set_interrupt_right_click();
}

void __ISR (_TIMER_4_VECTOR, IPL7SOFT) launch_movement (void){
	T4CONbits.ON = 0;
	IFS0bits.T4IF = 0;
	TMR4 = 0;
	movement();
	T4CONbits.ON = 1;
}

void __ISR (_EXTERNAL_2_VECTOR, IPL5SOFT) left_click (void){
	IEC0bits.INT2IE = 0;
	LATBbits.LATB14 = 1;
	if (g_edge_int2 == FALLING)
            g_button = 1;
        if (g_edge_int2 == RISING)
            g_button = 0;
	T2CONbits.ON = 1;
}

void __ISR (_EXTERNAL_1_VECTOR, IPL5SOFT) right_click (void){
	IEC0bits.INT1IE = 0;
	LATBbits.LATB14 = 1;
	//if (g_edge_int1 == FALLING)
		g_button = 2;
	T3CONbits.ON = 1;
}

void set_interrupt_left_click() {
	// Interrupt Button left click
	if (g_edge_int2 == FALLING)
		g_edge_int2 = RISING;
	else if (g_edge_int2 == RISING)
		g_edge_int2 = FALLING;
	INTCONbits.INT2EP = g_edge_int2;

	IPC2bits.INT2IP = 5;
	IPC2bits.INT2IS = 0;
	IFS0bits.INT2IF = 0;
	IEC0bits.INT2IE = 1;
}

void set_interrupt_right_click() {
	// Interrupt Button right click
	if (g_edge_int1 == FALLING)
		g_edge_int1 = RISING;
	else if (g_edge_int1 == RISING)
		g_edge_int1 = FALLING;
	INTCONbits.INT1EP = g_edge_int1;

	IPC1bits.INT1IP = 5;
	IPC1bits.INT1IS = 0;
	IFS0bits.INT1IF = 0;
	IEC0bits.INT1IE = 1;
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
	PR2 = 10;//312500 -> 1s
	IPC2bits.T2IP = 6;
	IFS0bits.T2IF = 0;
	IEC0bits.T2IE = 1;

	// Timer 3 link to int3
	T3CON = 0;//reset
	T3CONbits.TCKPS = 0b110; //1:64
	TMR3 = 0;//set timer 0
	PR3 = 250;// 312500 -> 1s
	IPC3bits.T3IP = 6;
	IFS0bits.T3IF = 0;
	IEC0bits.T3IE = 1;

        // Timer 4 link to MPU9150
	T4CON = 0;//reset
	T4CONbits.TCKPS = 0b011; //1:8
	TMR4 = 0;//set timer 0
	PR4 = 312;//1ms 312500 -> 1s
	IPC4bits.T4IP = 7;
	IFS0bits.T4IF = 0;
	IEC0bits.T4IE = 1;
}

void set_interrupt() {
	set_external_interrupt();
	set_timer();

	INTCONbits.MVEC = 1;
	__builtin_enable_interrupts();
}