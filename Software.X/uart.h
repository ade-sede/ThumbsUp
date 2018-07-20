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

#define UART2_BAUD_RATE ((PBCLK /(115200 * 16) - 1))
#define NEWLINE "\r\n"


void uart2_transmit_byte(u8 byte);
void uart2_idle_after_transmit();
u8 uart2_receive_byte();
void uart2_transmit_idle(u8 byte);
void uart2_init(u32 BRG);
void uart2_transmitnbr(u16 data);
void uart2_putstr(u8 *str);

void uart1_transmit_byte(u8 byte);
void uart1_idle_after_transmit();
u8 uart1_receive_byte();
void uart1_transmit_idle(u8 byte);
void uart1_init(u32 BRG);
void uart1_transmitnbr(u16 data);
void uart1_putstr(u8 *str);

#endif	/* UART_H */

