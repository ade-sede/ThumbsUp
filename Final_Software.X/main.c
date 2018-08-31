#include "header.h"

u8 i = 2;

int main(void) {
	set_pps();

	//CNSTATBbits.
	TRISBbits.TRISB14 = 0;
	LATBbits.LATB14 = 1;

	while(1){
		Nop();
	}
}