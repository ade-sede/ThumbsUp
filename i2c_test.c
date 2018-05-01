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

#define ADDR_READ_MODE(addr) ((addr << 1) | 1)
#define ADDR_WRITE_MODE(addr) (addr << 1)
#define RW_READ 1
#define RW_WRITE 0
#define ACK 0
#define NACK 1
#define ACCEL_XOUT_L 0x3C
#define WHO_AM_I 0x75
#define SLAVE_ADDR 0b1101000
#define LED_OFF LATFbits.LATF1 = 0;
#define LED_ON LATFbits.LATF1 = 1;
#define LED_TOGGLE LATFbits.LATF1 ^= 1;

u8 volatile target;
u8 volatile g_i2c_state = 0;

void __ISR(_TIMER_2_VECTOR, IPL6SRS) handler_timer_2(void){
    LATFbits.LATF1 ^= 1;
    IFS0bits.T2IF = 0;
}

void inline	start_led_toggle() {
    	T2CONbits.ON = 1;
}

void inline	start_master() {
	I2C1CONbits.SEN = 1;// start
}

void inline	restart_master() {
	I2C1CONbits.RSEN = 1;// start
}

void inline	ack_master(unsigned char value) {
	I2C1CONbits.ACKDT = value;
	I2C1CONbits.ACKEN = 1; // initiate Ack seq on SDA,SCL and transmit ACKDT bit
}

void inline	send_master(u8 data) {
	I2C1TRN = data;//set data
}

void inline	receive_master(void) {
    I2C1CONbits.RCEN = 1;
}

void inline	stop_master() {
	I2C1CONbits.PEN = 1;
}

// Interrupt SLAVE

void __ISR (_I2C_1_VECTOR, IPL7SRS) master_interrupt(void){
    IFS0bits.I2C1MIF = 0;
    switch (g_i2c_state) {
    case 0:
	send_master(ADDR_WRITE_MODE(SLAVE_ADDR));
    case 1:
	if (I2C1STATbits.ACKSTAT == NACK)
	    start_led_toggle();
	send_master(target);
    case 2:
	if (I2C1STATbits.ACKSTAT == NACK)
	    start_led_toggle();
	restart_master();
    case 3:
	send_master(ADDR_READ_MODE(SLAVE_ADDR));
    case 4:
	if (I2C1STATbits.ACKSTAT == NACK)
	    start_led_toggle();
	receive_master();
    case 5:
	ack_master(NACK);
    case 6:
	stop_master();
	if (I2C1RCV)
	    LED_TOGGLE;
	g_i2c_state = 0;
    }
    ++g_i2c_state;
}
int main(void) {

	// active led
       TRISFbits.TRISF1 = 0;
       LATFbits.LATF1 = 0;

       IFS0bits.T2IF = 0;
       IPC2bits.T2IP = 6;
       IEC0bits.T2IE = 1;

       IFS0bits.I2C1MIF = 0;
       IEC0bits.I2C1MIE = 1;
       IPC6bits.I2C1IP = 7;


       INTCONbits.MVEC = 1;
       __builtin_enable_interrupts();

	TMR2 = 0;
	PR2 = 62000;
	T2CON = 0;
	T2CONbits.TCKPS = 0b011;

	I2C1CONbits.ON = 1;
	target = WHO_AM_I;
	start_master();
	while(1);
}
