#ifndef RN42_H
# define RN42_H

# include "uart.h"
# define RN42_BAUD_RATE ((PBCLK /(115200 * 16)) - 1)
# define UART1_BAUD_RATE RN42_BAUD_RATE

void	test_run(void);
void	send_report(s16 *report);
s16      *create_report(s32 x_move, s32 y_move);
//void    send_movement(struct s_velocity *velocity);

#endif