#include "header.h"
#include "uart.h"
#include "RN42.h"

u32 i = 0;
u16 pot = 0;

void     main(void) {
	set_pps();
        init_pot();                         /* Potentiometre */
        uart1_init((u32)UART1_BAUD_RATE);   /* Bluetooth */
	TRISBbits.TRISB14 = 0;              /* Led Move */
        LATBbits.LATB14 = 1;                /* Led On */

	while(1){
                while (i < 10000)
                    i++;
                i = 0;
                LATBbits.LATB14 ^= 1;
                uart1_putstr("Test \n\r");
	}
}