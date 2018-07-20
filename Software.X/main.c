#include "header.h"
#include "MPU9150.h"
#include "i2c.h"
#include "uart.h"
#include "movement.h"

s32 g_xbias = 0;
s32 g_ybias = 0;
s32 g_zbias = 0;

/*
 * Function in charge of doing everything that needs to be done ONCE in order for the system to be ready
 */

void	init(void) {
	TRISFbits.TRISF1 = 0;	/* Setting up tri-state */
	LATFbits.LATF1 = 0; /* Turn off led on the test board */

	i2c_config_and_start((u8)I2CBRG);	/* After this line i2c module is running with baud rate I2CBRG */
	uart2_init((u32)UART2_BAUD_RATE);


	MPU9150_write(PWR_MGMT_1, PWR_MGMT_ON_NO_TEMP);  /* Initialisation Power management -> no temp sensor */
	calibration();	/* Accelerometer calibration, in a no movement condition */
}

int main(void) {
	struct s_accel accel[2];
	struct s_velocity velocity[2];

	memset(accel, 0, sizeof(struct s_accel) * 2);
	memset(velocity, 0, sizeof(struct s_velocity) * 2);


	u8 buff[] = "Start\n\r";	
	init();
	
	uart2_putstr(buff);
	while (1) {

		
		movement(accel, velocity);
		Nop();
	}
}
