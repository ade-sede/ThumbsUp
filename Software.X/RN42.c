#include "header.h"
#include "RN42.h"
#include "uart.h"
#include "MPU9150.h"

void	send_movement(struct s_velocity *velocity) {
	//u16 i;
	uart1_transmit_idle(0xFD);
	uart1_transmit_idle(0x5);
	uart1_transmit_idle((0x2));
	uart1_transmit_idle(0xC8);
	uart1_transmit_idle((velocity->velocityX) / 10);
	uart1_transmit_idle((velocity->velocityY) / 10);
	uart1_transmit_idle(0x1);
	//while (i < 10000)
	//	i++;
}

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