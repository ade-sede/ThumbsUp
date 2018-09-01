/* 
 * File:   clock.h
 * Author: bocal
 *
 * Created on July 13, 2018, 4:10 PM
 */

#ifndef CLOCK_H
# define CLOCK_H

/* 
 * Most of the clock is configured in config.c with compiler directive
 * This header is just a security in case pre-processing doesnt happen the way we expected
 */

#define SYSCLK (8000000 / 2 * 20 / 1) // 80Mhz (PLL -> div 2 mul 20 div 1)
#define PBCLK (SYSCLK / 2) // 40 Mhz (because FPBDIV = DIV_2)


#endif	/* CLOCK_H */