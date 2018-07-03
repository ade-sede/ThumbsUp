/*
 * File:   header.h
 * Author: bocal
 *
 * Created on April 24, 2018, 2:08 PM
 */

#ifndef HEADER_H
#define	HEADER_H

#include <p32xxxx.h>
#include <xc.h>
#include <sys/attribs.h>

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned long u32;
typedef signed long s32;

#define RW_READ 1
#define RW_WRITE 0
#define ADDR_READ_MODE(addr) ((addr << 1) | RW_READ)
#define ADDR_WRITE_MODE(addr) (addr << 1)
#define NACK 1
#define ACK 0
#define ACCEL_XOUT_L 0x3C
#define WHO_AM_I 0x75
#define PWR_MGMT_1 0x6B
#define SLAVE_ADDR 0x68
#define LED_OFF LATFbits.LATF1 = 0
#define LED_ON LATFbits.LATF1 = 1
#define LED_TOGGLE LATFbits.LATF1 ^= 1
#define ACCEL_XOUT_L 0x3C
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_L 0x3E
#define ACCEL_YOUT_H 0x3D

extern u8 g_data_buffer[2];
extern u8 g_target;
extern u8 g_i2c_state;
extern u8 *g_data;
extern u8 g_err;

double c_p(double a, double t);
void shift_cursor(s8 x_stop, s8 y_stop);

void master_send(u8 data);
void master_receive();
void master_answer(u8 value);

void UART_transmit_byte(u8 byte);
void UART_idle_after_transmit();
u8 UART_RECEIVE_byte();
void UART_transmit_idle(u8 byte);
void UART2_init();

#endif	/* HEADER_H */