#ifndef MOVEMENT_H
# define MOVEMENT_H

/* Defines used to refer to the index of structs */
# define PREVIOUS 0
# define CURRENT 1
# define PREV 0
# define CURR 1

/* Defines for the discrimination window. */
# define WINDOW_LOW -100 /* Must be < 0 */
# define WINDOW_HIGH 100 /* Must be > 0 */

/* The number of no accel in a row before we consider velocity = 0 */
# define NO_ACCEL_TRESHOLD 25

void	movement(struct s_accel *accel, struct s_velocity *velocity);

#endif
