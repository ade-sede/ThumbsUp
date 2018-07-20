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
