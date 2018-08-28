
#ifndef DEBUG_H 
# define DEBUG_H

# include "MPU9150.h"

inline void print_accel(struct s_accel sample);
inline void print_velocity(struct s_velocity velocity);
inline void print_gyro(struct s_gyro *sample);

#endif