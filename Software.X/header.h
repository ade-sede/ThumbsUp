/* 
 * File:   header.h
 * Author: bocal
 *
 * Created on July 9, 2018, 2:52 PM
 */


#ifndef HEADER_H
# define	HEADER_H

# include <p32xxxx.h>
# include <xc.h>
# include <sys/attribs.h>

/* Extraneous includes for io libraries, used for debug */
# include <stdio.h>
# include <string.h>
# include <stdlib.h>

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned long u32;
typedef signed long s32;

void	set_interrupt();
void    init_pot();
u16      pot_report();

#endif	/* HEADER_H */

