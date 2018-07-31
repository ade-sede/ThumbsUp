#ifndef RN42_H
# define RN42_H

# include "uart.h" 
# define RN42_BAUD_RATE ((PBCLK /(115200 * 16) - 1))
# define UART1_BAUD_RATE RN42_BAUD_RATE

void	test_run(void);
void	send_report(u8 *report);
u8		*built_report(u8 button, u32 x_move, u32 y_move);

#endif
