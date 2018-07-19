#include "header.h"
#include "uart.h"

void UART_transmit_byte(u8 byte) {
    while (U2STAbits.UTXBF); // If transmit buffer is not full
    U2TXREG = byte; // Fill transmit buffer;
}

void UART_idle_after_transmit() {
    while (U2STAbits.TRMT != 1); // Transmit idle
}

u8 UART_receive_byte(){
    if (U2STAbits.URXDA)
        return(U2RXREG);
}

void UART_transmit_idle(u8 byte) {
    UART_transmit_byte(byte);
    UART_idle_after_transmit();
}

void UART2_init(u32 BRG){
    U2BRG = BRG; // Target baude rate = 115k
    U2MODEbits.PDSEL = 0; // 8 bit data, no parity
    U2MODEbits.STSEL = 0; // 1 stop bits
    U2STAbits.URXEN = 1; // reception enable
    U2STAbits.UTXEN = 1; // Transmition enabled.
    U2MODEbits.ON = 1;
}

void UART_transmitnbr(u16 data) {
	if (data < 0) {
		UART_transmit_idle('-');
		data = -data;
	}
	if (data == 0)
		UART_transmit_idle('0');
	if (data <= 9 && data > 0)
		UART_transmit_idle(data + 48);
	else if (data > 0) {
		UART_transmitnbr(data / 10);
		UART_transmit_idle(data % 10 + 48);
	}
}

void UART_putstr(u8 *str) {
	while(*str){
		UART_transmit_byte(*str);
		++str;
	}
	UART_idle_after_transmit();
}
