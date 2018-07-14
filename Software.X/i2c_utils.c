#include "header.h"
#include "i2c.h"

u8 volatile g_err = 0;

void i2c_config_and_start(u8 brg) {
	I2C1BRG = brg;
	I2C1CONbits.ON = 1;
}

void i2c_master_send(u8 data) {
	I2C1TRN = data;
	while (I2C1STATbits.TRSTAT == 1)
		Nop();
	if (I2C1STATbits.ACKSTAT == NACK)
		g_err = 1;
}

void i2c_master_receive(u8 *dest) {
	I2C1CONbits.RCEN = 1;
	while (I2C1STATbits.RBF == 0)
		Nop();
	*dest = I2C1RCV;
}

void i2c_master_answer(u8 value) {
	I2C1CONbits.ACKDT = value;
	I2C1CONbits.ACKEN = 1;
	while (I2C1CONbits.ACKEN == 1)
		Nop();
}

void i2c_config_register(u8 register_name, u8 bit_config) {
	I2C1CONbits.SEN = 1; // Master start
	while (I2C1CONbits.SEN == 1)
		Nop();
	i2c_master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
	i2c_master_send(register_name);
	i2c_master_send(bit_config);
	I2C1CONbits.PEN = 1; // Master stop
	while (I2C1CONbits.PEN == 1)
		Nop();
}