/* 
 * File:   MPU9150.h
 * Author: bocal
 *
 * Created on July 13, 2018, 3:56 PM
 */

#ifndef MPU9150_H
# define MPU9150_H
# include "header.h"


/*
 * Struct used to receive the data from the MPU9150
 * Each field corresponds to a register
 */
struct s_data {
	u8 accelX_LOW;
	u8 accelX_HIGH;
	u8 accelY_LOW;
	u8 accelY_HIGH;
	u8 accelZ_LOW;
	u8 accelZ_HIGH;
};

/* Local values we use */
#define PWR_MGMT_ON_NO_TEMP 0b00001000  /* Enables power management, disable temp. sensor */



/* Registers and their address */
#define ACCEL_XOUT_L 0x3C
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_L 0x3E
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_L 0x40
#define ACCEL_ZOUT_H 0x3F
#define WHO_AM_I 0x75
#define PWR_MGMT_1 0x6B
#define SLAVE_ADDR 0x68

#endif	/* MPU9150_H */

