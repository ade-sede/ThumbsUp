#include "header.h"
#include "MPU9150.h"
#include "i2c.h"
#include "uart.h"
#include "RN42.h"
//#include "movement.h"
#include "interrupt.h"

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

	uart1_putstr("Start\n\r");
        
	i2c_config_and_start((u8)I2CBRG);   /* After this line i2c module is running with baud rate I2CBRG */
	MPU9150_write(PWR_MGMT_1, PWR_MGMT_ON_NO_TEMP);  /* Initialisation Power management -> no temp sensor */
}

void     main(void) {
        u32 i = 0;
        u16 pot = 0;
        struct s_accel accel;

        memset(&accel, 0, sizeof(struct s_accel));
        
	set_pps();
        init();
	while(1){
                while (i < 10000)
                    i++;
                i = 0;
                read_accel(&accel);
                send_report(create_report(accel.accelX, accel.accelY));
            Nop();
	}
}