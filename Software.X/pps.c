/*
** This file is for Periphal Pin Select PPS
*/

/*
** Interrupt button
*/
INT2RPPSbits.INT2RPPS = 0000; // RPA2 -> INT2
INT1RPPSbits.INT1RPPS = 0000; // RPA3 -> INT1

/*
** UART
*/
RPB4PPSbits.RPB4PPS = 0001 // RPB4 -> U1TX
U1RXRPPSbits.U1RXRPPS = 0001 // RPB6 -> U1RX
TRISB6 = 1 // Input
