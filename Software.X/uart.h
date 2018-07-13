/* 
 * File:   uart.h
 * Author: bocal
 *
 * Created on July 13, 2018, 5:07 PM
 */

#ifndef UART_H
# define UART_H

#ifndef PBCLK
# include "clock.h"
#endif
#define SQR_MULT 10

#define UART_BAUD_RATE ((PBCLK /(115200 * 16) - 1))
#define NEWLINE "\r\n"

void UART_transmit_idle(u8 byte);
void UART_transmitnbr(u16 data);

#endif	/* UART_H */

