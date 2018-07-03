#include "header.h"

void master_send(u8 data) {
	I2C1TRN = data;
	while (I2C1STATbits.TRSTAT == 1)
		Nop();
	if (I2C1STATbits.ACKSTAT == NACK)
		g_err = 1;
}

void master_receive() {
	I2C1CONbits.RCEN = 1;
	while (I2C1STATbits.RBF == 0) Nop();
	*g_data = I2C1RCV;
}

void master_answer(u8 value) {
	I2C1CONbits.ACKDT = value;
	I2C1CONbits.ACKEN = 1;
	while (I2C1CONbits.ACKEN == 1);
}