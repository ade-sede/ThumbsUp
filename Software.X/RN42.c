#include "header.h"
#include "uart.h"
#include "MPU9150.h"

/* bouton n = (1 << n) bouton indexe en 0 */
void	send_movement(struct s_velocity *velocity) {
    u16 i = 0;
	/* connection lost if x or y == 0*/
    s16 valueY = velocity->velocityY / 100;
    s16 valueX = velocity->velocityX / 100;
    uart1_transmit_idle(0xFD); // Format mouse raw
    uart1_transmit_idle(5);
    uart1_transmit_idle(2);
    uart1_transmit_idle(1); // Button
    uart1_transmit_idle(1); // x
    uart1_transmit_idle(1); // y
    uart1_transmit_idle(1); // Wheel
     while (i < 10000)
                    ++i;
                Nop();
}

/* Function move mouse right and down*/
void	test_run(void) {
	u16 i = 0;

	while (1) {
            i = 0;
		uart1_transmit_idle(0xFD);
                uart1_transmit_idle(5);
		uart1_transmit_idle(2);
                uart1_transmit_idle(0);
		uart1_transmit_idle(0);
		uart1_transmit_idle(1);
		uart1_transmit_idle(0);
                while (i < 10000)
                    ++i;
                Nop();
	}
}