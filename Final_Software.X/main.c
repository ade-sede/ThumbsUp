#include "header.h"
#include "MPU9150.h"
#include "i2c.h"
#include "uart.h"
#include "RN42.h"
//#include "movement.h"
#include "interrupt.h"

/* Calibration */
s32 g_xbias = 0;
s32 g_ybias = 0;
s32 g_zbias = 0;

s32 g_xctrl = 0;
s32 g_yctrl = 0;
s32 g_zctrl = 0;

float g_accelR = 0;

struct s_gyro g_cal_gyro;
struct s_gyro g_degres_gyro;
struct s_g g_angle;

u8 g_button = 0;

/*
 * Function in charge of doing everything that needs to be done ONCE in order for the system to be ready
 */

void	init(void) {

		u32 i = 0;
        TRISBbits.TRISB14 = 0;              /* Setting up tri-state led */
        TRISAbits.TRISA2 = 1;               /* Setting up tri-state int2 input*/
		TRISAbits.TRISA3 = 1;               /* Setting up tri-state int3 input*/
        LATBbits.LATB14 = 0;                /* Turn off led on the test board */
		while (i < 100000)					/* Allow the code to re-init properly, i2c will start properly when turning on */
			i++;
		uart1_init((u32)RN42_BAUD_RATE + 1);/* Bluetooth */
        init_pot();                         /* Potentiometre */
        set_interrupt();                    /* Buttons */

	uart1_putstr("Start\n\r");

	i2c_config_and_start((u8)I2CBRG);   /* After this line i2c module is running with baud rate I2CBRG */

	MPU9150_write(PWR_MGMT_1, PWR_MGMT_ON_NO_TEMP);  /* Initialisation Power management -> no temp sensor */
	//calibration();	/* Accelerometer calibration, in a no movement condition durgin 100 cycles */
}

void     main(void) {
        u32 i = 0;
        u16 pot = 0;
        struct s_accel accel[2];
		unsigned int original_priority;

		memset(accel, 0, sizeof(struct s_accel) * 2);
		memset(&g_angle, 0, sizeof(struct s_g));
        
	set_pps();
        init();
	while(1){
                while (i < 10000)
                    i++;
                i = 0;
               // read_accel(&accel);
				original_priority = __builtin_get_isr_state();
				__builtin_set_isr_state(7);
				movement(accel);
//			send_report(create_report(accel.accelX, accel.accelY));
				__builtin_set_isr_state(original_priority);
            Nop();
	}
}