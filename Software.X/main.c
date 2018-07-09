/* 
 * File:   main.c
 * Author: bocal
 *
 * Created on July 9, 2018, 2:54 PM
 */

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

u8 volatile g_data_buffer[2];
u8 volatile g_target;
u8 volatile g_i2c_state = 0;
u8 volatile *g_data;
u8 volatile g_err = 0;

inline void i2c_master_send(u8 data) {
	I2C1TRN = data;
	while (I2C1STATbits.TRSTAT == 1)
		Nop();
	if (I2C1STATbits.ACKSTAT == NACK)
		g_err = 1;
}

inline void i2c_master_receive(u8 *i) {
	I2C1CONbits.RCEN = 1;
	while (I2C1STATbits.RBF == 0)
		Nop();
	*i = I2C1RCV;
}

inline void i2c_master_answer(u8 value) {
	I2C1CONbits.ACKDT = value;
	I2C1CONbits.ACKEN = 1;
	while (I2C1CONbits.ACKEN == 1)
		Nop();
}

inline void i2c_config() {
	OSCCONbits.PBDIV = PBCLK_DIV;

	I2C1BRG = I2CBRG;
	I2C1CONbits.ON = 1;
}

inline void i2c_init() {
	I2C1CONbits.SEN = 1; // Master start
	while (I2C1CONbits.SEN == 1)
		Nop();
	i2c_master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
	i2c_master_send(PWR_MGMT_1);
	i2c_master_send(0b00001000); // Power management -> Module on, no temp sensor.
	I2C1CONbits.PEN = 1;
	while (I2C1CONbits.PEN == 1)
		Nop();
}

inline u8 i2c_accel() {
	I2C1CONbits.SEN = 1; // Master start
	while (I2C1CONbits.SEN == 1)
		Nop();
	i2c_master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
	i2c_master_send(ACCEL_XOUT_L);
	I2C1CONbits.RSEN = 1;
	while (I2C1CONbits.RSEN == 1)
		Nop();
	i2c_master_send(ADDR_READ_MODE(SLAVE_ADDR));
	i2c_master_receive(&g_data[0]);
	i2c_master_answer(NACK);
	I2C1CONbits.PEN = 1;
	while (I2C1CONbits.PEN == 1)
		Nop();
	I2C1CONbits.SEN = 1; // Master start
	while (I2C1CONbits.SEN == 1)
		Nop();
	i2c_master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
	i2c_master_send(ACCEL_XOUT_H);
	I2C1CONbits.RSEN = 1;
	while (I2C1CONbits.RSEN == 1)
		Nop();
	i2c_master_send(ADDR_READ_MODE(SLAVE_ADDR));
	i2c_master_receive(&g_data[1]);
	i2c_master_answer(NACK);
	I2C1CONbits.PEN = 1;
	while (I2C1CONbits.PEN == 1)
		Nop();
	u8 x = 0;
	x |= g_data[0];
	x |= g_data[1];
	return (x);
}

int main(void) {
	u32 i = 0;
	u8 j = 0;
	u8 x = 0;
	char p[5] = "";
	TRISFbits.TRISF1 = 0;
	LATFbits.LATF1 = 0;

	g_data = &g_data_buffer[0];

	i2c_config();
	i2c_init();
	UART2_init();
	while (1) {
		x = i2c_accel();
		while(i++ < 40000);
		sprintf(p, "%d", x);
		/*while(p[j]){
			UART_transmit_byte(p[j]);
			j++;
		} j = 0;*/
		UART_transmit_byte(' ');

		i = 0;
		Nop();
	}
}