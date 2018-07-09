/* 
 * File:   header.h
 * Author: bocal
 *
 * Created on July 9, 2018, 2:52 PM
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

// VARIABLE I2C
#define RW_READ 1
#define RW_WRITE 0
#define ADDR_READ_MODE(addr) ((addr << 1) | RW_READ)
#define ADDR_WRITE_MODE(addr) (addr << 1)
#define NACK 1
#define ACK 0
#define ACCEL_XOUT_L 0x3C
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_L 0x3E
#define ACCEL_YOUT_H 0x3D
#define WHO_AM_I 0x75
#define PWR_MGMT_1 0x6B
#define SLAVE_ADDR 0x68

// CONFIG I2C
#define PBCLK_DIV 0b010 // 4 soit 10 MHz
#define FSCK 4000 // 400kHz
#define I2CBRG (PBCLK / (2 * FSCK)) - 2

//CONFIG UART
#define SQR_MULT 10

#define SYSCLK (8000000/2*20/1) // = 80Mhz
#define PBCLK (SYSCLK/2) // = 40 Mhz
#define UART_BAUD_RATE ((PBCLK/(115200*16)-1))

void UART_transmit_idle(u8 byte);

#endif	/* HEADER_H */

