#include "header.h"
#include "uart.h"
#include "MPU9150.h"

/* bouton n = (1 << n) bouton indexe en 0
**/

/* Create generic report with Mouse data
**/
u8* create_report(button, x_move, y_move) {
	u8 report[7];

	report[0] = 0xFD; // Format mouse raw
	report[1] = 5; // Length
	report[2] = 2; // Data descriptor -> Mouse
	report[3] = button;
	report[4] = u8(x_move / 100);
	report[5] = u8(y_move / 100);
	report[6] = 0; // Wheel
	return (report);
}

/* Send Mouse raw report format to module bluetooth
**/
void send report(u8 *report) {
	u8 i = 0;

	while (i < 7) {
		uart1_transmit_idle(report[i]);
		i++;
	}
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
