#include "header.h"
#include "i2c.h"

u8 volatile g_err = 0;

inline void i2c_config_and_start(u8 brg) {
	I2C1BRG = brg;
	I2C1CONbits.ON = 1;
}

inline void i2c_master_send(u8 data) {
	I2C1TRN = data;
	while (I2C1STATbits.TRSTAT == 1)
		Nop();
	if (I2C1STATbits.ACKSTAT == NACK)
		g_err = 1;
}

inline void i2c_master_receive(u8 *dest) {
	I2C1CONbits.RCEN = 1;
	while (I2C1STATbits.RBF == 0)
		Nop();
	*dest = I2C1RCV;
}

inline void i2c_master_answer(u8 value) {
	I2C1CONbits.ACKDT = value;
	I2C1CONbits.ACKEN = 1;
	while (I2C1CONbits.ACKEN == 1)
		Nop();
}