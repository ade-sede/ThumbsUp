#include "MPU9150.h"
#include "uart.h"
#include "header.h"
#include "debug.h"

inline void	print_accel(struct s_accel *sample) {
	char buff[4096];
	
	sprintf(buff, "%d		%d		%d\n\r", sample->accelX, sample->accelY, sample->accelZ);
	uart1_putstr(buff);
}

inline void	print_report(s16 *report) {
	char buff[4096];

	sprintf(buff, "%d	%d	%d      %d\n\r", report[3], report[4], report[5], report[6]);
	uart1_putstr(buff);
}