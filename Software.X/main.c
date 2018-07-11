/*
 * File:   main.c
 * Author: bocal
 *
 * Created on July 9, 2018, 2:54 PM
 */

#include "header.h"

u8 volatile g_data_buffer[6];
u8 volatile g_target;
u8 volatile g_i2c_state = 0;
u8 volatile *g_data;
u16 volatile *g_accelX;
u16 volatile *g_accelY;
u16 volatile *g_accelZ;
u8 volatile g_err = 0;

inline void i2c_master_send(u8 data) {
	I2C1TRN = data;
	while (I2C1STATbits.TRSTAT == 1)
		Nop();
	if (I2C1STATbits.ACKSTAT == NACK)
		g_err = 1;
}

inline void i2c_master_receive(u8 i) {
	I2C1CONbits.RCEN = 1;
	while (I2C1STATbits.RBF == 0)
		Nop();
	g_data_buffer[i] = I2C1RCV;
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

inline void i2c_pickup_data(u8 addr) {
	I2C1CONbits.SEN = 1; // Master start
	while (I2C1CONbits.SEN == 1)
		Nop();
	i2c_master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
	i2c_master_send(addr);
	I2C1CONbits.RSEN = 1;
	while (I2C1CONbits.RSEN == 1)
		Nop();
	i2c_master_send(ADDR_READ_MODE(SLAVE_ADDR));
	i2c_master_receive(addr - ACCEL_XOUT_H);
	i2c_master_answer(NACK);
	I2C1CONbits.PEN = 1;
	while (I2C1CONbits.PEN == 1)
		Nop();
}

inline void i2c_loop() {
	u8 i;

	i = ACCEL_XOUT_H;
	while (i <= ACCEL_ZOUT_L) {
		i2c_pickup_data(i);
		i++;
	}
}

int main(void) {
	TRISFbits.TRISF1 = 0;
	LATFbits.LATF1 = 0;

	//g_data = &g_data_buffer[0];
	g_accelX = (u16 *)&g_data_buffer[0];
	g_accelY = (u16 *)&g_data_buffer[2];
	g_accelZ = (u16 *)&g_data_buffer[4];

	i2c_config();
	i2c_init();
	UART2_init();
	while (1) {
		i2c_loop();
		UART_transmit_idle(' ');
		if (g_data_buffer[1])
			UART_transmitnbr(*g_accelX);
		else
			UART_transmit_idle(' ');
		Nop();
	}
}