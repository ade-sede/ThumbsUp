#ifndef MOVEMENT_H
# define MOVEMENT_H

#include "math.h"

/* Defines for the discrimination window. */
# define WINDOW_LOW -10 /* Must be < 0 */
# define WINDOW_HIGH 10 /* Must be > 0 */

/* Macro for the window */
/* (value comprise entre 0 et low) || (value comprise entre 0 et high)*/
# define INVALID_VALUE(value) (value <= WINDOW_HIGH && value >= WINDOW_LOW)

/* Transformation de l'accel em m/s2 : 2g * val / valeur_max * aceleration_pesanteur */
#define TRANS_ACCEL_TO_G(x) (x / 16384.0)

void calibration(void);
void movement();

#endif
