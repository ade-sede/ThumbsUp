#include "header.h"
#include "MPU9150.h"
#include "i2c.h"
#include "uart.h"
#include "RN42.h"
#include "movement.h"
#include "interrupt.h"

 /*
  * This variable bias represent the acceleration present on each axis, in a no-move condition
  *	They are mesured during calibration. We have to remove this bias from each measurement
  * so that it is accurate.
  */

struct s_accel g_calibration;
struct s_gravity g_accel;
struct s_gravity g_angle;
float g_accelR = 0;

u8 g_button = 0;

/*
 * Function in charge of doing everything that needs to be done ONCE in order for the system to be ready
 */

void	init(void) {

        TRISBbits.TRISB14 = 0;              /* Setting up tri-state led */
        TRISAbits.TRISA2 = 1;               /* Setting up tri-state int2 input*/
	TRISAbits.TRISA3 = 1;               /* Setting up tri-state int3 input*/
        LATBbits.LATB14 = 0;                /* Turn off led on the test board */

	uart1_init((u32)RN42_BAUD_RATE + 1);/* Bluetooth */
        init_pot();                         /* Potentiometre */
        set_interrupt();                    /* Buttons */

	i2c_config_and_start((u8)I2CBRG);   /* After this line i2c module is running with baud rate I2CBRG */
	MPU9150_write(PWR_MGMT_1, PWR_MGMT_ON_NO_TEMP);  /* Initialisation Power management -> no temp sensor */
   //     calibration();	/* Accelerometer and Gyroscope calibration, in a no movement condition durgin CALIBRATION_SAMPLE_NUMBER cycles */
        T4CONbits.ON = 1; /* Launch movement transmission */
}

int     main(void) {

        u32 a = 0;

        while (a < 100000)
            a++;
        memset(&g_calibration, 0, sizeof(struct s_accel));
        memset(&g_accel, 0 ,sizeof(struct s_gravity));
        memset(&g_angle, 0 ,sizeof(struct s_gravity));

        uart2_putstr("Start MAIN \n\r");
	set_pps();
        init();

	while(1){
		Nop();
	}
}