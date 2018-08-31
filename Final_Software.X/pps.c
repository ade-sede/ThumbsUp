#include "header.h"

/*
** This file is for Periphal Pin Select PPS
*/
void	set_pps() {

	ANSELA = 0;
	ANSELB = 0;
/*
** Interrupt button
*/
	INT2Rbits.INT2R = 0000; // RPA2 -> INT2
	INT1Rbits.INT1R = 0000; // RPA3 -> INT1

/*
** UART
*/
	CFGCONbits.IOLOCK = 0;
	RPB4Rbits.RPB4R = 0b0001; // RPB4 -> U1TX
	CFGCONbits.IOLOCK = 1;
	TRISBbits.TRISB6 = 1;
	U1RXRbits.U1RXR = 0b0001; // RPB6 -> U1RX
}
