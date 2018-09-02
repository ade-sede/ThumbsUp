#ifndef MOVEMENT_H
# define MOVEMENT_H

/* Defines used to refer to the index of structs */
# define PREVIOUS 0
# define CURRENT 1
# define PREV 0
# define CURR 1

/* Defines for the discrimination window. */
# define WINDOW_LOW -800 /* Must be < 0 */
# define WINDOW_HIGH 800 /* Must be > 0 */

/* Macro for the window */
/* (value comprise entre 0 et low) || (value comprise entre 0 et high)*/
# define INVALID_VALUE(value) (value <= WINDOW_HIGH && value >= WINDOW_LOW)
/* (value comprise entre 0 et low) || (value comprise entre 0 et high)*/
# define INVALID_VALUE_GYRO(value) (value <= 100 && value >= -100)
/* The number of no accel in a row before we consider velocity = 0 */
# define NO_ACCEL_TRESHOLD 25
/* Transformation de l'accel em m/s2 : 2g * val / valeur_max * aceleration_pesanteur */
#define TRANS_ACCEL_TO_G(x) (x / 16384.0)

#define TRANS_G_TO_MS2(x) (x * 9.81)


#define TRANS_GYRO_TO_DEGRE(x) (x / 131.0)

void	movement(struct s_accel *accel);
void	movement_gyro(struct s_gyro *gyro);

#endif
