#ifndef MOVEMENT_H
# define MOVEMENT_H

#include "math.h"

/* Defines for the discrimination window. */
# define WINDOW_LOW -800 /* Must be < 0 */
# define WINDOW_HIGH 800 /* Must be > 0 */

/* Transformation de l'accel em m/s2 : 2g * val / valeur_max * aceleration_pesanteur */
#define TRANS_ACCEL_TO_G(x) (x / 16384.0)

void calibration(void);
void movement();

#endif
