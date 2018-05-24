/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on May 24, 2018, 10:18 AM
 */

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)
#pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)

// DEVCFG1
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)

#include <p32xxxx.h>
#define SYSCLK (8000000/2*20/1) // = 80Mhz
#define PBCLK (SYSCLK/2) // = 10 Mhz
#define UART_BAUD_RATE ((PBCLK/115200/16)-1)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef char s8;
typedef short s16;
typedef long s32;



void UART_transmit_byte(u8 byte) {
    while (U2STAbits.UTXBF);// If transmit buffer is not full
    U2TXREG = byte; // Fill transmit buffer;
}

void UART_idle_after_transmit() {
//    while (U2STAbits.TRMT != 1); // Transmit idle
}

u8 UART_RECEIVE_byte()
{
	while(U2STAbits.URXDA)
		return(U2RXREG);
}

void UART_transmit_idle(u8 byte) {
    UART_transmit_byte(byte);
    UART_idle_after_transmit();
}

void shift_cursor(s8 x_stop, s8 y_stop) {
    UART_transmit_idle(0xFD); // Raw package ID
    UART_transmit_idle(5); // Package size
    UART_transmit_idle(2); // Mouse descriptor
    UART_transmit_idle(0); // Buttons
    UART_transmit_idle(x_stop);
    UART_transmit_idle(y_stop);
    UART_transmit_idle(0); // Wheel
}

int main() {

    /* DEVCFG.FPLLIDIV Must be set to divied by 2 */

    /* SYSCLK to 40MHz & PBCLK = SYSCLK*/
    /* FRC (8MHz) * PLLMULT (20) / PLLDIV (2) = 40 MHz*/

   /* OSCCONbits.NOSC = 0b11; // FRC with PLL module
    OSCCONbits.PLLMULT = 0b101; // 40; multi by 20
    OSCCONbits.PLLODIV = 0b010; // Not PLL output divide // divided by 4
    OSCCONbits.PBDIV = 0; // PCBLCK = SYSCLK

    */U2BRG = UART_BAUD_RATE; // Target baude rate = 115k
    U2MODEbits.PDSEL = 0; // 8 bit data, no parity
    U2MODEbits.STSEL = 0; // 1 stop bit
	U2STAbits.URXEN = 1; //reception enable
    U2MODEbits.ON = 1;
    U2STAbits.UTXEN = 1; // Transmition enabled.

	u8 i = 0;
    while (1) {
        shift_cursor(20, 20);
		UART_idle_after_transmit();
		 i = 0;
		//while(i++ < 20);
		//UART_transmit_idle(UART_RECEIVE_byte());
    }
}
