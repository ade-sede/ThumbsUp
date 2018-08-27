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
/* The number of no accel in a row before we consider velocity = 0 */
# define NO_ACCEL_TRESHOLD 25

void	movement(struct s_accel *accel, struct s_velocity *velocity, struct s_position *position);

#endif
