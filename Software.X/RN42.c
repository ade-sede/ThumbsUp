#include "header.h"
#include "uart.h"
#include "MPU9150.h"

/* bouton n = (1 << n) bouton indexe en 0
**/

/* Create generic report with Mouse data
**/

s16  *create_report(u8 button, s16 x_move, s16 y_move) {
    s16 report[7];
    u16 pot = pot_report();

	report[0] = (u8)0xFD; // Format mouse raw
	report[1] = 5; // Length
	report[2] = 2; // Data descriptor -> Mouse
	report[3] = button;
	report[4] = (x_move / 100);// * pot;
	report[5] = (y_move / 100);// * pot;
	report[6] = 0; // Wheel
	return (report);
}

/*
** Send Mouse raw report format to module bluetooth
** Sending protocol should never be interrupted ! Therefor, CPU interrupt state
** is jumped to the highest, and restored to its original state when sending is over 
*/

void send_report(s16 *report) {
	u16 i = 0;

	//char buff[4096];
	unsigned int original_priority;

	/* Storing priority on entry, jumping to highest */
	original_priority = __builtin_get_isr_state();
	__builtin_set_isr_state(7);
	
	while (i < 7) {
		uart1_transmit_idle(report[i]);
		i++;
	}
        while (i < 30000)
		++i;
	Nop();

	/* Restoring priority */
	__builtin_set_isr_state(original_priority);
}
