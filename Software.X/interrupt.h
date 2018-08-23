#ifndef INTERRUPT_H
# define INTERRUPT_H
#include "header.h"

/* Defines used to describe edge of interrupt used */
#define FALLING 0
#define RISING 1

/* This variable is state of button used in report to RN42 module 
* This button define different kind of Mouse click
*/
u8 g_button;

#endif
