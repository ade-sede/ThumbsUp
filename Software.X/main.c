#include "header.h"
#include "MPU9150.h"
#include "i2c.h"
#include "uart.h"
#include "RN42.h"
#include "movement.h"

s32 g_xbias = 0;
s32 g_ybias = 0;
s32 g_zbias = 0;

/*
 * Function in charge of doing everything that needs to be done ONCE in order for the system to be ready
 */

void	init(void) {
	TRISFbits.TRISF1 = 0;	/* Setting up tri-state */
	TRISDbits.TRISD8 = 1;	// int1 board button
	TRISDbits.TRISD9 = 1;	/* Setting up tri-state int2 input*/
	TRISDbits.TRISD10 = 1;	/* Setting up tri-state int3 input*/
	LATFbits.LATF1 = 0; /* Turn off led on the test board */

	uart2_init((u32)UART2_BAUD_RATE);	/* Console debug */
	uart1_init((u32)RN42_BAUD_RATE);	/* Bluetooth */

	u8 buff[] = "Start\n\r";
	Nop();
	uart2_putstr(buff);
	Nop();
	i2c_config_and_start((u8)I2CBRG);	/* After this line i2c module is running with baud rate I2CBRG */

	MPU9150_write(PWR_MGMT_1, PWR_MGMT_ON_NO_TEMP);  /* Initialisation Power management -> no temp sensor */
	calibration();	/* Accelerometer calibration, in a no movement condition */
}

int main(void) {
	struct s_accel accel[2];
	struct s_velocity velocity[2];
	unsigned int original_priority;


	memset(accel, 0, sizeof(struct s_accel) * 2);
	memset(velocity, 0, sizeof(struct s_velocity) * 2);

	init();
	init_pot();
	set_interrupt();
	while (1) {
		/* Storing priority on entry, jumping to highest */
		original_priority = __builtin_get_isr_state();
		__builtin_set_isr_state(7);
		movement(accel, velocity);
		/* Restoring priority */
		__builtin_set_isr_state(original_priority);
		Nop();
	}
}
