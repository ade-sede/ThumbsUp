#include "header.h"
#include "RN42.h"
#include "uart.h"

void	send_movement(struct s_velocity *velocity) {
	uart1_transmit_idle(0);
}

void	test_run(void) {
	while (1) {
	uart1_transmit_idle(0xFD);
	uart1_transmit_idle(5);
	uart1_transmit_idle((1));
	uart1_transmit_idle(1);
	uart1_transmit_idle(1);
	}
}