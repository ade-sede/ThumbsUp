/*
 * File:   main.c
 * Author: bocal
 *
 * Created on July 9, 2018, 2:54 PM
 */

#include "header.h"
#include "MPU9150.h"
#include "i2c.h"
#include "uart.h"

int main(void) {
	u8 buff[4096];
	struct s_data data;

	memset(buff, 0, 4096);
	memset(&data, 0, sizeof(struct s_data));
	TRISFbits.TRISF1 = 0;
	LATFbits.LATF1 = 0;

	i2c_config_and_start((u8)I2CBRG);
	i2c_init();
	UART2_init();
	UART_putstr("X                  Y                   Z"NEWLINE);
	while (1) {
		i2c_loop(&data);
		sprintf(buff, "%d              %d                %d"NEWLINE,	(s16)((data.accelX_HIGH << 8) | data.accelX_LOW),\
																		(s16)((data.accelY_HIGH << 8) | data.accelY_LOW),\
																		(s16)((data.accelZ_HIGH << 8) | data.accelZ_LOW));
		UART_putstr(buff);
		Nop();
		memset(buff, 0, 4096);
	}
}