#include <stdio.h>
#include <math.h>
#include "header.h"

double compute_move(double a, double t){
	return((a / 2) * pow(t,2));
}

void shift_cursor(s8 x_stop, s8 y_stop) {
    UART_transmit_idle(0xFD); // Raw package ID
    UART_transmit_idle(5); // Package size
    UART_transmit_idle(2); // Mouse descriptor
    UART_transmit_idle(1); // Buttons
    UART_transmit_idle(x_stop);
    UART_transmit_idle(y_stop);
//    UART_transmit_idle(1); // Wheel
}