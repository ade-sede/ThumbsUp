#include "header.h"
#include "i2c.h"

u8 volatile g_err = 0;

void i2c_config_and_start(u8 brg) {
        /*** DEBUG 9 ROUND CLOCK SCL TO RESET I2C COMMUNICATION ***/
        TRISBbits.TRISB8 = 0;
        u8 i = 0;
        u8 j = 0;
        while (i < 9){
           LATBbits.LATB8 = 1;
           while (j < 4){
               j++;
           }
           j = 0;
           LATBbits.LATB8 = 0;
           while (j < 4){
               j++;
           }
           j = 0;

           i++;
        }
        /*** END ***/
	I2C1BRG = brg;
	I2C1CONbits.ON = 1;
}

/** Creer une interrupt non bloquante **/
void i2c_master_send(u8 data) {
	I2C1TRN = data;
	while (I2C1STATbits.TRSTAT == 1) /// Status register : indicate the state of the master module
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
