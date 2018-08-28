#ifndef MPU9150_H
# define MPU9150_H
# include "header.h"


/*
 * Struct used to receive the data from the MPU9150
 * Each field corresponds to the lower part and the
 * higher part of the MPU9150 merged together
 */

struct s_accel {
	s32 accelX;
	s32 accelY;
	s32 accelZ;
};

struct s_velocity {
	s32 velocityX;
	s32 velocityY;
	s32 velocityZ;
};

struct s_gyro {
	s32 gyroX;
	s32 gyroY;
	s32 gyroZ;
};
 /*
  * Those variables represent the acceleration present on each axis, in a no-move condition 
  *	They are mesured during calibration. We have to remove this bias from each measurement
  * so that it is accurate.
  */

s32 g_xbias;
s32 g_ybias;
s32 g_zbias;

s32 g_xctrl;
s32 g_yctrl;
s32 g_zctrl;

/* Config bits */
# define PWR_MGMT_ON_NO_TEMP 0b1000  /* Goes to PWR_MGMT_1. Disable temp sensor */

/*
** Those 2 values are caped to 65335 because the counter used
** to compare is only 16 bit long
*/

# define CALIBRATION_SAMPLE_NUMBER 100 /* Max value is 65335*/
# define AVERAGE_SAMPLE_NUMBER 64 /* Max value is 65335*/

/* Defines for the discrimination window for gyroscope value */
# define WIN_LOW -1000 /* Must be < 0 */
# define WIN_HIGH 1000 /* Must be > 0 */

/* Macro for the window */
/* (value non comprise entre 0 et low) || (value non comprise entre 0 et high)*/
# define OUTSIDE_VALUE(value) (value >= WIN_HIGH || value <= WIN_LOW)

/* Registers and their address */
# define ACCEL_XOUT_L 0x3C
# define ACCEL_XOUT_H 0x3B
# define ACCEL_YOUT_L 0x3E
# define ACCEL_YOUT_H 0x3D
# define ACCEL_ZOUT_L 0x40
# define ACCEL_ZOUT_H 0x3F
# define GYRO_XOUT_L 0x44
# define GYRO_XOUT_H 0x43
# define GYRO_YOUT_L 0x46
# define GYRO_YOUT_H 0x45
# define GYRO_ZOUT_L 0x48
# define GYRO_ZOUT_H 0x47
# define WHO_AM_I 0x75
# define PWR_MGMT_1 0x6B
# define SLAVE_ADDR 0x68
# define MPU9150_ADDR 0x68
# define FIFO_EN 0x23
# define FIFO_COUNTH 0x72
# define FIFO_COUNTL 0x73
# define FIFO_R_W 0x74

void MPU9150_write(u8 register_name, u8 bit_config);
void MPU9150_read(u8 source, u8 *dest);
void read_accel(struct s_accel *accel);
void read_gyro(struct s_gyro *gyro);
void calibration(u8 calibration_sample_number);
void calibration_gyroscope(u8 calibration_sample_number);
void check_gyroscope_position(struct s_gyro *gyro);

#endif	/* MPU9150_H */

