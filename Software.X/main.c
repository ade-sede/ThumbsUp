#include "header.h"
#include "MPU9150.h"
#include "i2c.h"
#include "uart.h"

extern s16 g_xbias = 0;
extern s16 g_ybias = 0;
extern s16 g_zbias = 0;

/*
 * Function in charge of doing everything that needs to be done ONCE in order for the system to be ready
 */

void	init(void) {
	TRISFbits.TRISF1 = 0;	/* Setting up tri-state */
	LATFbits.LATF1 = 0; /* Turn off led on the test board */

	i2c_config_and_start((u8)I2CBRG);	/* After this line i2c module is running with baud rate I2CBRG */

	MPU9150_write(PWR_MGMT_1, PWR_MGMT_ON_NO_TEMP);  /* Initialisation Power management -> no temp sensor */
	calibration();	/* Accelerometer calibration, in a no movement condition */
}

int main(void) {
#if 0
	u8 buffer[4096]
#endif 

	struct s_accel accel[2];
	struct s_velocity velocity[2];

	memset(&accel, 0, sizeof(struct s_accel) * 2);
	memset(&velocity, 0, sizeof(struct s_velocity) * 2);

	init();

#if 0 /* Re-enable this code when u need to print */
	UART2_init();
	UART_putstr("X                  Y                   Z"NEWLINE);
#endif
	while (1) {
#if 0
		memset(buffer, 0, 4096);
		sprintf(buffer, "%d              %d                %d"NEWLINE,	(s16)data.accelX,\
																		(s16)data.accelY,\
																		(s16)data.accelZ);
		UART_putstr(buffer);
#endif
		Nop();
	}
}
