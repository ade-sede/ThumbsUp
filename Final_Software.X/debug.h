
#ifndef DEBUG_H 
# define DEBUG_H

# include "MPU9150.h"

inline void print_accel(struct s_accel *sample);
inline void	print_accelR(float g_accelR);

#endif