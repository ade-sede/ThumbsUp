// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 1)
#pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)// DEVCFG2


/* Following this configuration,  SYSCLK is 80MHz and PBCLK is 40MHz */

/* IF ANY OF THOSE DEFINES ARE CHANGED YOU NEED TO ADJUST THE DEFINES IN CLOCK.H */

#undef SYSCLK
#define SYSCLK (8000000 / 2 * 20 / 1)

#undef PBCLK
#define PBCLK (SYSCLK / 2)
