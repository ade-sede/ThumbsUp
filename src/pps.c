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
        RPB10Rbits.RPB10R = 0b0010;// RPB10 -> U2TX
	CFGCONbits.IOLOCK = 1;
	TRISBbits.TRISB6 = 1;
	U1RXRbits.U1RXR = 0b0001; // RPB6 -> U1RX
        TRISBbits.TRISB11 = 1;
	U2RXRbits.U2RXR = 0b0011; // RPB11 -> U2RX
}