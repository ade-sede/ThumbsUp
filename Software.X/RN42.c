#include "header.h"
#include "uart.h"
#include "MPU9150.h"

void	send_movement(struct s_velocity velocity) {
	/* connection lost if x or y == 0*/
	uart1_transmit_idle(0xFD);// Format mouse raw
	uart1_transmit_idle(0x5);
	uart1_transmit_idle((0x2));
	uart1_transmit_idle(0xC8);// Button
	uart1_transmit_idle((velocity.velocityX));// x
	uart1_transmit_idle((velocity.velocityY));// y
	uart1_transmit_idle(0x1);// Wheel
}

/* Function move mouse right and down*/
void	test_run(void) {
	u16 i;

	while (1) {
		i = 0;
		uart1_transmit_idle(0xFD);
		uart1_transmit_idle(0x5);
		uart1_transmit_idle((0x2));
		uart1_transmit_idle(0xC8);
		uart1_transmit_idle(0x2);
		uart1_transmit_idle(0x2);
		uart1_transmit_idle(0x1);
		while (i < 10000)
			i++;
	}
}