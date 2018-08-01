#include "MPU9150.h"
#include "uart.h"
#include "header.h"
#include "debug.h"

inline void	print_accel(struct s_accel sample) {
	char buff[4096];
	
	sprintf(buff, "%d		%d		%d\n\r", sample.accelX, sample.accelY, sample.accelZ);
	uart2_putstr(buff);
}

inline void	print_velocity(struct s_velocity velocity) {
	char buff[4096];
	
	sprintf(buff, "%d		%d		%d\n\r", velocity.velocityX, velocity.velocityY, velocity.velocityZ);
	uart2_putstr(buff);
}


void	send_movement(struct s_velocity *velocity) {
	u16 i = 0;

        s16 valueX = velocity->velocityX/100;
        s16 valueY = velocity->velocityY/100;

	i = 0;
	uart1_transmit_idle(0xFD);
	uart1_transmit_idle(5);
	uart1_transmit_idle(2);
	uart1_transmit_idle(0);
	uart1_transmit_idle(-valueX);
	uart1_transmit_idle(valueY);
	uart1_transmit_idle(0);
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