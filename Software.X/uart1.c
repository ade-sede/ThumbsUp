
#include "header.h"
#include "uart.h"

void uart1_transmit_byte(u8 byte) {
    while (U1STAbits.UTXBF); // If transmit buffer is not full
    U1TXREG = byte; // Fill transmit buffer;
}

void uart1_idle_after_transmit() {
    while (U1STAbits.TRMT != 1); // Transmit idle
}

u8 uart1_receive_byte(){
    if (U1STAbits.URXDA)
        return(U1RXREG);
}

void uart1_transmit_idle(u8 byte) {
    uart1_transmit_byte(byte);
    uart1_idle_after_transmit();
}

void uart1_init(u32 BRG){
    U1BRG = BRG; // Target baude rate = 115k
    U1MODEbits.PDSEL = 0; // 8 bit data, no parity
    U1MODEbits.STSEL = 0; // 1 stop bits
    U1STAbits.URXEN = 1; // reception enable
    U1STAbits.UTXEN = 1; // Transmition enabled.
    U1MODEbits.ON = 1;
}

void uart1_putstr(u8 *str) {
	while(*str){
		uart1_transmit_byte(*str);
		++str;
	}
	uart1_idle_after_transmit();
}

