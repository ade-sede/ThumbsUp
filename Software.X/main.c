#include <p32xxxx.h>
#include <xc.h>
#include <sys/attribs.h>
#include "header.h"

u8 g_data_buffer[2];
u8 g_target;
u8 g_i2c_state = 0;
u8 *g_data;
u8 g_err = 0;

int main(void) {
	TRISFbits.TRISF1 = 0;
	LATFbits.LATF1 = 0;

	g_data = &g_data_buffer[0];
	OSCCONbits.PBDIV = 0b011; // PBCLK = SYSCLK / 8 soit 10Mhz

	//        I2C1BRG = 0x0F8;
	I2C1BRG = 0x030;
	I2C1CONbits.ON = 1;

	I2C1CONbits.SEN = 1; // Master start
	while (I2C1CONbits.SEN == 1)
		Nop();
	master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
	master_send(PWR_MGMT_1);
	master_send(0b00001000); // Power management -> Module on, no temp sensor.
	I2C1CONbits.PEN = 1;
	while (I2C1CONbits.PEN == 1);
	while (1) {
		//        while (I2C1STATbits.P == 0) Nop();
		I2C1CONbits.SEN = 1; // Master start
		//            while (I2C1CONbits.SEN == 1)
		//                Nop();
		//            master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
		//            master_send(PWR_MGMT_1);
		//            master_send(0b00001000); // Power management -> Module on, no temp sensor.
		//            I2C1CONbits.PEN = 1;
		//            Nop();
		//            while (I2C1CONbits.PEN == 1);
		//         //   while (I2C1STATbits.P == 0) Nop();
		I2C1CONbits.SEN = 1;
		while (I2C1CONbits.SEN == 1);
		master_send(ADDR_WRITE_MODE(SLAVE_ADDR));
		master_send(WHO_AM_I);
		I2C1CONbits.RSEN = 1;
		while (I2C1CONbits.RSEN == 1);
		master_send(ADDR_READ_MODE(SLAVE_ADDR));
		master_receive();
		master_answer(NACK);
		I2C1CONbits.PEN = 1;
		while (I2C1CONbits.PEN == 1);
		Nop();
	}
	while (1);
}